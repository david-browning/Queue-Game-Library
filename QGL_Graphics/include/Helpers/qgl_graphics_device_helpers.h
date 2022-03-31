#pragma once
#include "include/qgl_graphics_include.h"
#include "include/Content/qgl_device_configuration.h"
#include "include/Helpers/qgl_color_helpers.h"
#include "include/qgl_window.h"

namespace qgl::graphics
{
   using sync_interval_t = typename uint8_t;
   using swap_flag_t = typename size_t;
}

namespace qgl::graphics::helpers
{
   /*
    Returns the size (in bytes) that an element of type "f" uses.
    */
   extern QGL_GRAPHICS_API size_t format_size(DXGI_FORMAT f) noexcept;

   inline auto make_gpu_factory()
   {
      UINT flags = 0;
      //Create a debug interface if compiled in debug mode.
#ifdef _DEBUG
         //If in debug mode and cannot create a debug layer, then crash.
      winrt::com_ptr<ID3D12Debug> dbgCtrl;
      winrt::check_hresult(D3D12GetDebugInterface(IID_PPV_ARGS(dbgCtrl.put())));

      dbgCtrl->EnableDebugLayer();
      flags |= DXGI_CREATE_FACTORY_DEBUG;
#endif

      winrt::com_ptr<igpu_factory> ret = nullptr;
      winrt::check_hresult(CreateDXGIFactory2(flags, IID_PPV_ARGS(ret.put())));
      return ret;
   }

   inline auto make_2d_factory()
   {
      D2D1_FACTORY_OPTIONS d2dFactoryOptions = {};
#ifdef _DEBUG
      d2dFactoryOptions.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
#endif

      winrt::com_ptr<i2d_factory> ret = nullptr;
      winrt::check_hresult(D2D1CreateFactory(
         D2D1_FACTORY_TYPE::D2D1_FACTORY_TYPE_SINGLE_THREADED,
         IID_PPV_ARGS(ret.put())));

      return ret;
   }

   inline auto make_text_factory()
   {
      winrt::com_ptr<itext_factory> ret;
      winrt::check_hresult(DWriteCreateFactory(
         DWRITE_FACTORY_TYPE::DWRITE_FACTORY_TYPE_SHARED,
         __uuidof(itext_factory),
         reinterpret_cast<IUnknown**>(ret.put())));

      return ret;
   }

   inline auto enum_hardware_adapters(
      igpu_factory* factory_p,
      D3D_FEATURE_LEVEL minFeatureLevel)
   {
      std::vector<winrt::com_ptr<igpu_adapter>> adapters;

      //Temp adapter
      winrt::com_ptr<igpu_adapter> adapter = nullptr;

      //Enumerate adapters until the HRESULT fails.
      for (UINT i = 0;
         factory_p->EnumAdapterByGpuPreference(
            i,
            DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE,
            IID_PPV_ARGS(adapter.put())) == S_OK;
         i++)
      {
         DXGI_ADAPTER_DESC1 desc;
         winrt::check_hresult(adapter->GetDesc1(&desc));

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
      std::vector<winrt::com_ptr<igpu_output>> ret;
      UINT i = 0;
      winrt::com_ptr<IDXGIOutput> curOutput = nullptr;
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
      // Get window bound of the app
      qgl::math::boundary<LONG, true> wndBounds{
         wnd.left<LONG>(),
         wnd.top<LONG>(),
         wnd.left<LONG>() + wnd.width<LONG>(),
         wnd.top<LONG>() + wnd.height<LONG>()
      };

      // Iterate through the DXGI outputs associated with the DXGI adapter,
      // and find the output whose bounds have the greatest overlap with the
      // app window (i.e. the output for which the intersection area is the
      // greatest).

      LONG bestIntersectArea = -1;
      winrt::com_ptr<igpu_output> ret = nullptr;
      auto outputs = enum_adapter_outputs(adapter_p);
      for (auto& curOutput : outputs)
      {
         DXGI_OUTPUT_DESC desc;
         winrt::check_hresult(curOutput->GetDesc(&desc));
         qgl::math::boundary<LONG, true> outBounds{ desc.DesktopCoordinates };

         if (wndBounds.intersects(outBounds))
         {
            qgl::math::boundary<LONG, true> intersectWnd{
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

   extern QGL_GRAPHICS_API bool support_tiling(i3d_device* dev_p) noexcept;

   /*
    Returns true if the GPU factory supports tearing on the monitor.
    */
   extern QGL_GRAPHICS_API bool support_tearing(igpu_factory* fac_p);

   /*
    Returns true if the GPU factory supports tearing on the monitor.
    */
   inline bool support_tearing(const igpu_factory* fac_p)
   {
      return support_tearing(fac_p);
   }

   /*
    Returns true if the monitor supports stereoscopic rendering.
    */
   extern QGL_GRAPHICS_API bool support_stereo();

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
   inline std::vector<DXGI_MODE_DESC1> enum_display_modes(
      igpu_output* monitor_p,
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
      winrt::check_hresult(monitor_p->GetDisplayModeList1(
         monitorFmt, flgs, &numModes, nullptr));

      //Fill the list of modes.
      std::vector<DXGI_MODE_DESC1> ret(static_cast<size_t>(numModes));
      winrt::check_hresult(monitor_p->GetDisplayModeList1(
         monitorFmt, flgs, &numModes, ret.data()));

      return ret;
   }

   inline winrt::com_ptr<icmd_queue> make_cmd_queue(
      const D3D12_COMMAND_QUEUE_DESC& desc,
      i3d_device* dev_p)
   {
      winrt::com_ptr<icmd_queue> ret;
      winrt::check_hresult(dev_p->CreateCommandQueue(
         &desc, IID_PPV_ARGS(ret.put())));
      return ret;
   }

   inline auto make_3d_device(igpu_adapter* adapter_p, D3D_FEATURE_LEVEL ftrLvl)
   {
      winrt::com_ptr<i3d_device> ret;
      winrt::check_hresult(D3D12CreateDevice(
         adapter_p, ftrLvl, IID_PPV_ARGS(ret.put())));
      name_d3d(ret.get(), L"D3D 12 Device");
      return ret;
   }

   inline auto make_2d_device(i3d_bridge_device* d3d11on12Dev_p,
                              i2d_factory* d2dFactory_p)
   {
      winrt::com_ptr<i2d_device> d2dDev_p;
      winrt::check_hresult(d2dFactory_p->CreateDevice(
         dynamic_cast<IDXGIDevice*>(d3d11on12Dev_p),
         d2dDev_p.put()));

      return d2dDev_p;
   }

   inline auto make_2d_context(const D2D1_DEVICE_CONTEXT_OPTIONS& opts,
                               i2d_device* d2dDev_p)
   {
      winrt::com_ptr<i2d_context> ret;
      winrt::check_hresult(d2dDev_p->CreateDeviceContext(opts, ret.put()));
      return ret;
   }

   inline auto make_swap_chain(const gpu_config& config,
                               window& wnd,
                               igpu_factory* dxgiFactory_p,
                               icmd_queue* cmdQueue_p)
   {
      // Need a down-casted pointer for now.
      winrt::com_ptr<IDXGISwapChain1> swapChain;
      DXGI_SCALING scaling = config.high_resolution() ?
         DXGI_SCALING_NONE : DXGI_SCALING_ASPECT_RATIO_STRETCH;

      DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
      swapChainDesc.BufferCount = static_cast<UINT>(config.buffers());
      swapChainDesc.Width = wnd.width<UINT>();
      swapChainDesc.Height = wnd.height<UINT>();
      swapChainDesc.Format = color_format(config.hdr_mode());
      swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
      //DXGI_SWAP_EFFECT Enumeration: https://tinyurl.com/yypnyzu7
      swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;

      swapChainDesc.SampleDesc.Count = 1;
      //swapChainDesc.SampleDesc.Quality = 0;
      swapChainDesc.Stereo = config.stereo();
      swapChainDesc.Scaling = scaling;

      //D2D1_ALPHA_MODE Enumeration: https://tinyurl.com/y4xjkw4q
      swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_PREMULTIPLIED;

      //DXGI_SWAP_CHAIN_FLAG Enumeration: https://tinyurl.com/yxq7s75x
      UINT swapChainFlags = DXGI_SWAP_CHAIN_FLAG_FOREGROUND_LAYER |
         DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
      if (config.tearing())
      {
         swapChainFlags |= DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING;
      }

      swapChainDesc.Flags = swapChainFlags;

      winrt::check_hresult(dxgiFactory_p->CreateSwapChainForCoreWindow(
         dynamic_cast<IUnknown*>(cmdQueue_p),
         wnd.unknown(),
         &swapChainDesc,
         nullptr,
         swapChain.put()));


      auto ret = swapChain.as<iswap_chain>();
      set_hdr(config, ret.get());
      return ret;
   }

   inline void resize_swap_chain(
      const gpu_config& config,
      const window& wnd,
      iswap_chain* swpChain_p)
   {
      //Use the old swap chain description to resize swpChain.
      DXGI_SWAP_CHAIN_DESC1 swapChainDesc;
      winrt::check_hresult(swpChain_p->GetDesc1(&swapChainDesc));

      winrt::check_hresult(swpChain_p->ResizeBuffers(
         static_cast<UINT>(config.buffers()),
         wnd.width<UINT>(),
         wnd.height<UINT>(),
         swapChainDesc.Format,
         swapChainDesc.Flags));
   }
}