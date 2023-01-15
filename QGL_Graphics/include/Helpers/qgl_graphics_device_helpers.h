#pragma once
#include "include/qgl_graphics_include.h"
#include "include/qgl_device_configuration.h"
#include "include/Helpers/qgl_color_helpers.h"
#include "include/qgl_window.h"

namespace qgl::graphics
{
   using sync_interval_t = typename UINT;
   using swap_flag_t = typename UINT;

   struct gpu_mem_info final
   {
      /*
       Specifies the OS-provided video memory budget, in bytes, that the
       application should target. If CurrentUsage is greater than Budget, the
       application may incur stuttering or performance penalties due to
       background activity by the OS to provide other applications with a fair
       usage of video memory.
       */
      size_t budget = 0;

      /*
       Application’s current video memory usage, in bytes.
       */
      size_t used = 0;

      /*
       The amount of video memory, in bytes, that the application has available
       for reservation.
       */
      size_t available = 0;

      /*
       The amount of video memory, in bytes, that is reserved by the
       application.
       */
      size_t reserved = 0;
   };

   struct gpu_desc
   {
      qgl::sys_str name = { 0 };

      /*
       The number of bytes of dedicated video memory that are not shared with
       the CPU.
       */
      size_t vmem = 0;

      /*
       The number of bytes of dedicated system memory that are not shared with
       the CPU. This memory is allocated from available system memory at boot
       time.
       */
      size_t smem = 0;

      uint32_t dev_id = 0;
   };
}

namespace qgl::graphics::helpers
{
   /*
    Returns the size (in bytes) that an element of type "f" uses.
    */
   extern "C" QGL_GRAPHICS_API size_t QGL_CC format_size(
      DXGI_FORMAT f) noexcept;

   /*
    Gets the application's GPU memory budget.
    */
   extern "C" QGL_GRAPHICS_API result_t QGL_CC app_mem(
      igpu_adapter* dev_p,
      gpu_idx_t node,
      gpu_mem_info* out) noexcept;

   /*
    Gets information about the given adapter.
    */
   extern "C" QGL_GRAPHICS_API result_t QGL_CC get_gpu_desc(
      igpu_adapter* adapter_p, gpu_desc* desc_p) noexcept;

   inline auto make_gpu_factory(bool enableDebug)
   {
      UINT flags = 0;
      //Create a debug interface if compiled in debug mode.
      if (enableDebug)
      {
         //If in debug mode and cannot create a debug layer, then crash.
         pptr<ID3D12Debug> dbgCtrl;
         check_result(D3D12GetDebugInterface(
            IID_PPV_ARGS(dbgCtrl.put())));

         dbgCtrl->EnableDebugLayer();
         flags |= DXGI_CREATE_FACTORY_DEBUG;
      }

      pptr<igpu_factory> ret = nullptr;
      check_result(CreateDXGIFactory2(flags, IID_PPV_ARGS(ret.put())));
      return ret;
   }

   inline auto enum_hardware_adapters(
      igpu_factory* factory_p,
      D3D_FEATURE_LEVEL minFeatureLevel)
   {
      std::vector<pptr<igpu_adapter>> adapters;

      //Temp adapter
      pptr<igpu_adapter> adapter = nullptr;

      //Enumerate adapters until the HRESULT fails.
      for (UINT i = 0;
         factory_p->EnumAdapterByGpuPreference(
            i,
            DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE,
            IID_PPV_ARGS(adapter.put())) == S_OK;
         i++)
      {
         DXGI_ADAPTER_DESC1 desc;
         check_result(adapter->GetDesc1(&desc));

         //Only use the device if it is not the basic render driver.
         if ((desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) !=
            DXGI_ADAPTER_FLAG_SOFTWARE)
         {
            //If we can create a d3d device with the MinimumFeatureLevel, 
            //then add it to the vector.
            if (SUCCEEDED(D3D12CreateDevice(
               adapter.get(), minFeatureLevel,
               __uuidof(ID3D12Device), nullptr)))
            {
               adapters.push_back(adapter);
            }
         }
         
         adapter = nullptr;
      }

      return adapters;
   }

   /*
    Gets a collection of outputs
    */
   inline auto enum_adapter_outputs(igpu_adapter* adptr_p)
   {
      std::vector<pptr<igpu_output>> ret;
      UINT i = 0;
      pptr<IDXGIOutput> curOutput = nullptr;
      while (adptr_p->EnumOutputs(i, curOutput.put()) != DXGI_ERROR_NOT_FOUND)
      {
         ret.push_back(curOutput.as<igpu_output>());
         i++;
      }

      return ret;
   }

   /*
    Finds the output that contains most of this application.

    For example, in a two monitor setup, if more than half of this application
    is displayed on monitor 1, then this will return the DXGI Output for
    monitor 1.

    Apps should check fac_p->IsCurrent() before finding the best output.
    If the factory is not current, it should be recreated.
    */
   inline auto find_output(igpu_adapter* adapter_p, const window& wnd)
   {
      // Get window bounds (in DIP) of the app.
      auto wndBounds = wnd.boundary();

      // Iterate through the DXGI outputs associated with the DXGI adapter,
      // and find the output whose bounds have the greatest overlap with the
      // app window (i.e. the output for which the intersection area is the
      // greatest).

      auto bestIntersectArea = static_cast<dip_t>(-1);
      pptr<igpu_output> ret = nullptr;
      auto outputs = enum_adapter_outputs(adapter_p);
      for (auto& curOutput : outputs)
      {
         // description's desktop coordinates are dip
         DXGI_OUTPUT_DESC desc;
         check_result(curOutput->GetDesc(&desc));
         decltype(wndBounds) outBounds{ desc.DesktopCoordinates };

         if (wndBounds.intersects(outBounds))
         {
            decltype(wndBounds) intersectWnd{
               wndBounds, outBounds
            };

            if (intersectWnd.area() > bestIntersectArea)
            {
               ret = curOutput;
               bestIntersectArea = intersectWnd.area();
            }
         }
      }

      return ret;
   }

   /*
    Returns the correct swap chain flag to use for the given graphics config.
    See https://tinyurl.com/dxgi-present
    */
   inline swap_flag_t get_swap_flag(const gpu_config& cnfg) noexcept
   {
      // TODO: Does only windowed mode support tearing?
      // Variable refresh rate displays: https://tinyurl.com/y4polwfg
      // When using sync interval 0, it is recommended to always pass the 
      // tearing flag when it is supported, even when presenting in windowed 
      // mode. However, this flag cannot be used if the app is in fullscreen 
      // mode as a result of calling SetFullscreenState.
      return cnfg.tearing() ? DXGI_PRESENT_ALLOW_TEARING : 0;
   }

   /*
    Returns the synchronization interval to use for the swap chain.
    See https://tinyurl.com/nf-dxgi-idxgiswapchain-present
    */
   inline sync_interval_t get_sync_interval(const gpu_config& cnfg) noexcept
   {
      return cnfg.tearing() ? 0 : 1;
   }

   /*
    Returns a vector of display modes supported by the monitor and the
    DXGI_FORMAT. Pass true for includeStereo to enumerate stereo modes.
    */
   inline auto enum_display_modes(igpu_output* monitor_p,
                                  DXGI_FORMAT monitorFmt,
                                  bool includeInterlacing,
                                  bool includeStereo)
   {
      //DXGI_ENUM_MODES: https://tinyurl.com/y6fqcnqt
      UINT flgs = DXGI_ENUM_MODES_SCALING;
      if (includeStereo)
      {
         flgs |= DXGI_ENUM_MODES_STEREO;
      }
      if (includeInterlacing)
      {
         flgs |= DXGI_ENUM_MODES_INTERLACED;
      }

      //Get the number of modes.
      UINT numModes = 0;
      check_result(monitor_p->GetDisplayModeList1(
         monitorFmt, flgs, &numModes, nullptr));

      //Fill the list of modes.
      std::vector<DXGI_MODE_DESC1> ret(static_cast<size_t>(numModes));
      check_result(monitor_p->GetDisplayModeList1(
         monitorFmt, flgs, &numModes, ret.data()));

      return ret;
   }

   /*
    Gets the resolution for the swap chain from the given parameters.
    The width is stored in "first" and the height is stored in "second".
    */
   inline std::pair<UINT, UINT> get_resolution(const gpu_config& config,
                                               window& wnd)
   {
      switch (config.resolution_mode())
      {
         case descriptors::resolution_modes::window_resolution:
         {
            return std::make_pair(
               static_cast<UINT>(dip_to_pixels(wnd.width(), wnd.dpi_x())), 
               static_cast<UINT>(dip_to_pixels(wnd.height(), wnd.dpi_y())));
         }
         case descriptors::resolution_modes::static_resolution:
         {
            return std::make_pair<UINT, UINT>(config.width(), config.height());
         }
         default:
         {
            throw std::runtime_error{ "Not Implemented" };
         }
      }
   }

   inline pptr<icmd_queue> make_cmd_queue(
      const D3D12_COMMAND_QUEUE_DESC& desc,
      i3d_device* dev_p)
   {
      pptr<icmd_queue> ret;
      check_result(dev_p->CreateCommandQueue(
         &desc, IID_PPV_ARGS(ret.put())));
      return ret;
   }

   inline auto make_3d_device(igpu_adapter* adapter_p,
                              D3D_FEATURE_LEVEL ftrLvl)
   {
      pptr<i3d_device> ret;
      check_result(D3D12CreateDevice(
         adapter_p, ftrLvl, IID_PPV_ARGS(ret.put())));
      name_d3d(ret.get(), L"D3D 12 Device");
      return ret;
   }

   inline auto make_swap_chain(const gpu_config& config,
                               window& wnd,
                               igpu_factory* dxgiFactory_p,
                               icmd_queue* cmdQueue_p)
   {
      auto res = get_resolution(config, wnd);

      // Need a down-casted pointer for now.
      pptr<IDXGISwapChain1> swapChain;
      DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
      swapChainDesc.BufferCount = static_cast<UINT>(config.buffers());
      swapChainDesc.Width = res.first;
      swapChainDesc.Height = res.second;
      swapChainDesc.Format = color_format(config.hdr_mode());
      swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
      //DXGI_SWAP_EFFECT Enumeration: https://tinyurl.com/yypnyzu7
      swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;

      swapChainDesc.SampleDesc.Count = 1;
      swapChainDesc.SampleDesc.Quality = 0;
      swapChainDesc.Scaling = DXGI_SCALING_ASPECT_RATIO_STRETCH;

      //DXGI_SWAP_CHAIN_FLAG Enumeration: https://tinyurl.com/yxq7s75x
      UINT swapChainFlags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
      if (config.tearing())
      {
         swapChainFlags |= DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING;
      }

      swapChainDesc.Flags = swapChainFlags;

      check_result(dxgiFactory_p->CreateSwapChainForCoreWindow(
         cmdQueue_p,
         wnd.unknown(),
         &swapChainDesc,
         nullptr,
         swapChain.put()));

      auto ret = swapChain.as<iswap_chain>();
      set_hdr(config, ret.get());
      return ret;
   }

   inline void resize_swap_chain(const gpu_config& config,
                                 const window& wnd,
                                 iswap_chain* swpChain_p)
   {
      //Use the old swap chain description to resize swpChain.
      DXGI_SWAP_CHAIN_DESC1 swapChainDesc;
      check_result(swpChain_p->GetDesc1(&swapChainDesc));

      check_result(swpChain_p->ResizeBuffers(
         static_cast<UINT>(config.buffers()),
         static_cast<UINT>(dip_to_pixels(wnd.width(), wnd.dpi_x())),
         static_cast<UINT>(dip_to_pixels(wnd.height(), wnd.dpi_y())),
         swapChainDesc.Format,
         swapChainDesc.Flags));
   }

   /*
    Calculates how many bytes are needed to store an element of DXGI_FORMAT "f".
    Most component formats are supported. This will throw std::invalid_argument
    if the format is not supported.
    */
   constexpr size_t fmt_size(DXGI_FORMAT f)
   {
      switch (f)
      {
         case DXGI_FORMAT_R32G32B32A32_FLOAT:
         case DXGI_FORMAT_R32G32B32A32_SINT:
         case DXGI_FORMAT_R32G32B32A32_TYPELESS:
         case DXGI_FORMAT_R32G32B32A32_UINT:
         {
            return sizeof(float) * 4;
         }
         case DXGI_FORMAT_R32G32B32_FLOAT:
         case DXGI_FORMAT_R32G32B32_SINT:
         case DXGI_FORMAT_R32G32B32_TYPELESS:
         case DXGI_FORMAT_R32G32B32_UINT:
         {
            return sizeof(float) * 3;
         }
         case DXGI_FORMAT_R32G32_FLOAT:
         case DXGI_FORMAT_R32G32_SINT:
         case DXGI_FORMAT_R32G32_TYPELESS:
         case DXGI_FORMAT_R32G32_UINT:
         case DXGI_FORMAT_R16G16B16A16_TYPELESS:
         case DXGI_FORMAT_R16G16B16A16_FLOAT:
         case DXGI_FORMAT_R16G16B16A16_UNORM:
         case DXGI_FORMAT_R16G16B16A16_UINT:
         case DXGI_FORMAT_R16G16B16A16_SNORM:
         case DXGI_FORMAT_R16G16B16A16_SINT:
         {
            return sizeof(float) * 2;
         }
         case DXGI_FORMAT_R10G10B10A2_TYPELESS:
         case DXGI_FORMAT_R10G10B10A2_UNORM:
         case DXGI_FORMAT_R10G10B10A2_UINT:
         case DXGI_FORMAT_R11G11B10_FLOAT:
         case DXGI_FORMAT_R8G8B8A8_TYPELESS:
         case DXGI_FORMAT_R8G8B8A8_UNORM:
         case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
         case DXGI_FORMAT_R8G8B8A8_UINT:
         case DXGI_FORMAT_R8G8B8A8_SNORM:
         case DXGI_FORMAT_R8G8B8A8_SINT:
         case DXGI_FORMAT_R16G16_TYPELESS:
         case DXGI_FORMAT_R16G16_FLOAT:
         case DXGI_FORMAT_R16G16_UNORM:
         case DXGI_FORMAT_R16G16_UINT:
         case DXGI_FORMAT_R16G16_SNORM:
         case DXGI_FORMAT_R16G16_SINT:
         case DXGI_FORMAT_R32_TYPELESS:
         case DXGI_FORMAT_D32_FLOAT:
         case DXGI_FORMAT_R32_FLOAT:
         case DXGI_FORMAT_R32_UINT:
         case DXGI_FORMAT_R32_SINT:
         case DXGI_FORMAT_R24G8_TYPELESS:
         case DXGI_FORMAT_D24_UNORM_S8_UINT:
         case DXGI_FORMAT_R24_UNORM_X8_TYPELESS:
         case DXGI_FORMAT_X24_TYPELESS_G8_UINT:
         {
            return sizeof(float);
         }
         case DXGI_FORMAT_R8G8_TYPELESS:
         case DXGI_FORMAT_R8G8_UNORM:
         case DXGI_FORMAT_R8G8_UINT:
         case DXGI_FORMAT_R8G8_SNORM:
         case DXGI_FORMAT_R8G8_SINT:
         case DXGI_FORMAT_R16_TYPELESS:
         case DXGI_FORMAT_R16_FLOAT:
         case DXGI_FORMAT_D16_UNORM:
         case DXGI_FORMAT_R16_UNORM:
         case DXGI_FORMAT_R16_UINT:
         case DXGI_FORMAT_R16_SNORM:
         case DXGI_FORMAT_R16_SINT:
         {
            return 2;
         }
         case DXGI_FORMAT_R8_TYPELESS:
         case DXGI_FORMAT_R8_UNORM:
         case DXGI_FORMAT_R8_UINT:
         case DXGI_FORMAT_R8_SNORM:
         case DXGI_FORMAT_R8_SINT:
         case DXGI_FORMAT_A8_UNORM:
         {
            return 1;
         }
         default:
         {
            throw std::invalid_argument("Unknown DXGI_FORMAT size.");
         }
      }
   }
}