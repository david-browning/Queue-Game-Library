#include "pch.h"
#include "include/GPU/qgl_graphics_device_helpers.h"

namespace qgl::graphics::helpers
{
   bool support_tearing(const winrt::com_ptr<dxgi_factory>& factory_p)
   {
      BOOL supported = false;
      winrt::check_hresult(factory_p->CheckFeatureSupport(
         DXGI_FEATURE_PRESENT_ALLOW_TEARING,
         &supported,
         sizeof(supported)));

      return static_cast<bool>(supported);
   }

   bool support_hdr(const winrt::com_ptr<IDXGIOutput6>& output,
                    DXGI_OUTPUT_DESC1& desc)
   {
      winrt::check_hresult(output->GetDesc1(&desc));
      return (desc.ColorSpace == DXGI_COLOR_SPACE_RGB_FULL_G2084_NONE_P2020);
   }

   bool support_stereo()
   {
      //TODO: Check for stereo support.
      return false;
   }

   UINT get_swap_flag(
      const content::buffers::GRAPHICS_CONFIG_BUFFER& cnfg) noexcept
   {
      //TODO: Does only windowed mode support tearing?
      //Variable refresh rate displays: https://tinyurl.com/y4polwfg
      //When using sync interval 0, it is recommended to always pass the 
      //tearing flag when it is supported, even when presenting in windowed 
      //mode. However, this flag cannot be used if the app is in fullscreen 
      //mode as a result of calling SetFullscreenState.
      return cnfg.tearing() ? DXGI_PRESENT_ALLOW_TEARING : 0;
   }

   UINT get_sync_interval(
      const content::buffers::GRAPHICS_CONFIG_BUFFER& cnfg) noexcept
   {
      return cnfg.tearing() ? 0 : 1;
   }

   std::vector<DXGI_MODE_DESC1> enum_display_modes(
      const winrt::com_ptr<IDXGIOutput6>& monitor,
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
      winrt::check_hresult(monitor->GetDisplayModeList1(monitorFmt,
                                                        flgs,
                                                        &numModes,
                                                        nullptr));

      //Fill the list of modes.
      std::vector<DXGI_MODE_DESC1> ret(static_cast<size_t>(numModes));
      winrt::check_hresult(monitor->GetDisplayModeList1(monitorFmt,
                                                       flgs,
                                                       &numModes,
                                                       ret.data()));

      return ret;
   }

   std::vector<winrt::com_ptr<IDXGIAdapter4>> enum_hardware_adapters(
      const winrt::com_ptr<dxgi_factory>& factory_p,
      D3D_FEATURE_LEVEL minFeatureLevel)
   {
      std::vector<winrt::com_ptr<IDXGIAdapter4>> adapters;

      //Temp adapter
      winrt::com_ptr<IDXGIAdapter4> adapter = nullptr;

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
            if (SUCCEEDED(D3D12CreateDevice(adapter.get(),
                                            minFeatureLevel,
                                            __uuidof(ID3D12Device),
                                            nullptr)))
            {
               adapters.push_back(adapter);
            }
         }

         adapter = nullptr;
      }

      return adapters;
   }

   winrt::com_ptr<d3d_device> make_d3d12_device(
      const winrt::com_ptr<IDXGIAdapter4> adapter_p,
      D3D_FEATURE_LEVEL featureLevel)
   {
      winrt::com_ptr<d3d_device> ret;

      //Create the device
      winrt::check_hresult(D3D12CreateDevice(adapter_p.get(),
                                             featureLevel,
                                             IID_PPV_ARGS(ret.put())));
      name_d3d(ret.get(), L"D3D 12 Device");

      return ret;
   }

   winrt::com_ptr<d3d_cmd_queue> make_cmd_queue(
      const D3D12_COMMAND_QUEUE_DESC& desc,
      const winrt::com_ptr<d3d_device>& dev_p)
   {
      winrt::com_ptr<d3d_cmd_queue> ret;
      winrt::check_hresult(dev_p->CreateCommandQueue(&desc,
                                                     IID_PPV_ARGS(ret.put())));
      return ret;
   }

   d3d11_ret_t make_d3d11_dev_and_context(
      const winrt::com_ptr<d3d_device>& d3d12Dev_p,
      const std::vector<IUnknown*>& queues)
   {
      UINT d3d11DeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
      #ifdef _DEBUG
      {
         d3d11DeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
      }
      #endif

      D3D_FEATURE_LEVEL usingFeatureLevel;

      winrt::com_ptr<ID3D11Device> d3d11Dev;
      winrt::com_ptr<d3d11_context> d3d11Context;

      winrt::check_hresult(D3D11On12CreateDevice(
         d3d12Dev_p.get(),
         d3d11DeviceFlags,
         nullptr, 0,
         queues.data(),
         static_cast<UINT>(queues.size()),
         0,
         d3d11Dev.put(),
         d3d11Context.put(),
         &usingFeatureLevel));

      winrt::com_ptr<d3d11_device> d3d11on12Dev =
         d3d11Dev.as<ID3D11On12Device>();
      return std::make_pair(d3d11on12Dev, d3d11Context);
   }

   winrt::com_ptr<d2d_device> make_d2d_device(
      const winrt::com_ptr<d3d11_device>& d3d11on12Dev_p,
      const winrt::com_ptr<d2d_factory>& d2dFactory_p)
   {
      winrt::com_ptr<d2d_device> d2dDev_p;
      winrt::check_hresult(d2dFactory_p->CreateDevice(
         d3d11on12Dev_p.as<IDXGIDevice>().get(),
         d2dDev_p.put()));

      return d2dDev_p;
   }

   winrt::com_ptr<d2d_context> make_d2d_context(
      const D2D1_DEVICE_CONTEXT_OPTIONS& opts,
      const winrt::com_ptr<d2d_device>& d2dDev_p)
   {
      winrt::com_ptr<d2d_context> d2dContext_p;
      winrt::check_hresult(d2dDev_p->CreateDeviceContext(opts,
                                                         d2dContext_p.put()));
      return d2dContext_p;
   }

   winrt::com_ptr<d3d_swap_chain> make_swap_chain(
      const content::buffers::GRAPHICS_CONFIG_BUFFER& config,
      DXGI_FORMAT fmt,
      UINT windowWidth,
      UINT windowHeight,
      IUnknown* const unknownWindow,
      const winrt::com_ptr<dxgi_factory>& dxgiFactory_p,
      const winrt::com_ptr<d3d_cmd_queue>& cmdQueue_p)
   {
      winrt::com_ptr<IDXGISwapChain1> swapChain;
      DXGI_SCALING scaling = config.high_resolution() ?
         DXGI_SCALING_NONE : DXGI_SCALING_ASPECT_RATIO_STRETCH;

      DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
      swapChainDesc.BufferCount = config.buffers();
      swapChainDesc.Width = windowWidth;
      swapChainDesc.Height = windowHeight;
      swapChainDesc.Format = fmt;
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
         cmdQueue_p.get(),
         unknownWindow,
         &swapChainDesc,
         nullptr,
         swapChain.put()));

      return swapChain.as<d3d_swap_chain>();
   }

   void resize_swap_chain(UINT bufferCount,
                          UINT windowWidth,
                          UINT windowHeight,
                          winrt::com_ptr<d3d_swap_chain>& swpChain)
   {
      //Use the old swap chain description to resize swpChain.
      DXGI_SWAP_CHAIN_DESC1 swapChainDesc;
      winrt::check_hresult(swpChain->GetDesc1(&swapChainDesc));

      auto hr = swpChain->ResizeBuffers(bufferCount,
                                        windowWidth,
                                        windowHeight,
                                        swapChainDesc.Format,
                                        swapChainDesc.Flags);
      winrt::check_hresult(hr);
   }
}