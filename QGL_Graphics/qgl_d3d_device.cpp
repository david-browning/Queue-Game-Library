#include "pch.h"
#include "include/qgl_d3d_device.h"
#include "include/qgl_gdev_helpers.h"

static constexpr DXGI_FORMAT FORMAT_16_BIT_HDR = DXGI_FORMAT_R16G16B16A16_FLOAT;
static constexpr DXGI_FORMAT FORMAT_10_BIT_HDR = DXGI_FORMAT_R10G10B10A2_UNORM;
static constexpr DXGI_FORMAT FORMAT_8_BIT_NO_HDR = DXGI_FORMAT_R8G8B8A8_UNORM;

qgl::graphics::graphics_device::graphics_device(const GRAPHICS_CONFIG_BUFFER& config,
                                                const appview_pt& appView_p,
                                                const corewin_pt& coreWindow_p) :
   m_config(config),
   m_wasFullScreen(false),
   m_window_p(std::make_shared<window>(appView_p, coreWindow_p))
{
   construct();
}

qgl::graphics::graphics_device::graphics_device(const GRAPHICS_CONFIG_BUFFER&& config,
                                                const appview_pt& appView_p,
                                                const corewin_pt& coreWindow_p) :
   m_config(std::move(config)),
   m_wasFullScreen(false),
   m_window_p(std::make_shared<window>(appView_p, coreWindow_p))
{
   construct();
}

qgl::graphics::graphics_device::graphics_device(const GRAPHICS_CONFIG_BUFFER& config,
                                                const std::shared_ptr<window>& wnd_p) :
   m_config(config),
   m_wasFullScreen(false),
   m_window_p(wnd_p)
{
   construct();
}

qgl::graphics::graphics_device::graphics_device(const GRAPHICS_CONFIG_BUFFER&& config,
                                                const std::shared_ptr<window>& wnd_p) :
   m_config(std::move(config)),
   m_wasFullScreen(false),
   m_window_p(wnd_p)
{
   construct();
}

qgl::graphics::graphics_device::graphics_device(const GRAPHICS_CONFIG_BUFFER& config,
                                                const std::shared_ptr<window>&& wnd_p) :
   m_config(config),
   m_wasFullScreen(false),
   m_window_p(std::move(wnd_p))
{
   construct();
}

qgl::graphics::graphics_device::graphics_device(const GRAPHICS_CONFIG_BUFFER&& config,
                                                const std::shared_ptr<window>&& wnd_p) :
   m_config(std::move(config)),
   m_wasFullScreen(false),
   m_window_p(std::move(wnd_p))
{
   construct();
}

qgl::graphics::graphics_device::~graphics_device()
{
   //Close any contexts that might be executing.
   m_d2dDeviceContext_p->EndDraw();
   m_d2dDeviceContext_p->Flush();
   m_d3d11DeviceContext_p->Flush();

   //Order in which com_ptrs are destroyed is imported.
   //Destroy pointers in reverse order they were created.
   m_d2dDeviceContext_p = nullptr;
   m_d2dDevice_p = nullptr;
   m_d3d11DeviceContext_p = nullptr;
   m_d3d11On12Device_p = nullptr;
   m_cmdQueue_p = nullptr;
   m_d3d12Device_p = nullptr;

   m_usingAdapter = nullptr;
   for (auto& adptr : m_adapters)
   {
      adptr = nullptr;
   }
}

void qgl::graphics::graphics_device::reset(GRAPHICS_CONFIG_BUFFER& newConfig)
{
   m_wasFullScreen = m_config.full_screen();
   m_config = newConfig;




   m_d2dDeviceContext_p->SetTarget(nullptr);
   m_d3d11DeviceContext_p->Flush();


   //Set full-screen / window mode for the window.
   auto fullNow = m_config.full_screen();
   //If we were in full screen and are now not in full screen
   if (m_wasFullScreen && !fullNow)
   {
      //Exit full screen.
      m_window_p->exit_full_screen();
   }
   //Else if we were not in full screen and are not in full screen.
   else if (!m_wasFullScreen && fullNow)
   {
      //Enter full screen mode.
      m_window_p->enter_full_screen();
   }
   m_wasFullScreen = fullNow;

   p_set_fmt();
   resize_swap_chain(m_config.buffers(),
                     m_window_p->width(),
                     m_window_p->height(),
                     m_swapChain_p);
   p_set_color_space();

   m_syncInterval = get_sync_interval(m_config);
   m_swapFlags = get_swap_flag(m_config);
}

void qgl::graphics::graphics_device::p_make_factories()
{
   m_dxgiFactory_p = nullptr;
   m_d2dFactory_p = nullptr;
   m_dWriteFactory_p = nullptr;

   UINT dxgiFactoryFlags = 0;

   //Create a debug interface if compiled in debug mode.
   #ifdef _DEBUG
   {
      winrt::com_ptr<ID3D12Debug> debugController;
      auto hr = D3D12GetDebugInterface(IID_PPV_ARGS(debugController.put()));

      //If in debug mode and cannot create a debug layer, then crash.
      winrt::check_hresult(hr);

      debugController->EnableDebugLayer();

      dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
   }
   #endif

   winrt::check_hresult(CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(m_dxgiFactory_p.put())));

   D2D1_FACTORY_OPTIONS d2dFactoryOptions = {};
   #ifdef _DEBUG
   d2dFactoryOptions.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
   #endif

   winrt::check_hresult(D2D1CreateFactory(D2D1_FACTORY_TYPE::D2D1_FACTORY_TYPE_SINGLE_THREADED, __uuidof(d2d_factory),
                                          &d2dFactoryOptions, m_d2dFactory_p.put_void()));

   winrt::check_hresult(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED,
                                            __uuidof(d_write_factory),
                                            reinterpret_cast<IUnknown**>(m_dWriteFactory_p.put())));
}

void qgl::graphics::graphics_device::construct()
{
   p_make_factories();
   p_make_graphics();
   p_make_context_sensitive_members();
}

void qgl::graphics::graphics_device::p_make_graphics()
{
   m_d3d12Device_p = nullptr;
   m_d3d11On12Device_p = nullptr;
   m_d3d11DeviceContext_p = nullptr;
   m_d2dDevice_p = nullptr;
   m_d2dDeviceContext_p = nullptr;
   m_cmdQueue_p = nullptr;

   //Enumerate the available adapters.
   constexpr D3D_FEATURE_LEVEL featLevel = D3D_FEATURE_LEVEL_12_0;
   m_adapters = enum_hardware_adapters(m_dxgiFactory_p, featLevel);

   //Figure out which adapter is preferred. If the config buffer's preferred adapter is 0,
   //then there is not preferred adapter. Use the 0th adapter from the list.
   size_t adapterIdx = 0;
   if (m_config.adapter_devID() != 0)
   {
      //Find the adapter that matches the config's devID.
      for (size_t i = 0; i < m_adapters.size(); i++)
      {
         DXGI_ADAPTER_DESC3 desc;
         winrt::check_hresult(m_adapters[i]->GetDesc3(&desc));
         if (desc.DeviceId == m_config.adapter_devID())
         {
            adapterIdx = i;
            break;
         }
      }
   }

   m_usingAdapter = m_adapters[adapterIdx];

   //Now that we know which adapter to use, make the d3d12 device.
   m_d3d12Device_p = make_d3d12_device(m_usingAdapter, featLevel);

   //Create the command queue for the device.
   D3D12_COMMAND_QUEUE_DESC desc = {};
   desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
   desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
   m_cmdQueue_p = make_cmd_queue(desc, m_d3d12Device_p);
   name_d3d(m_cmdQueue_p.get(), L"Command Queue");

   //Create the D3D11 Device and context
   m_queues = { m_cmdQueue_p.get() };
   auto d3d11Objects = make_d3d11_dev_and_context(m_d3d12Device_p, m_queues);
   m_d3d11On12Device_p = d3d11Objects.first;
   m_d3d11DeviceContext_p = d3d11Objects.second;

   //Create the d2d device and context.
   m_d2dDevice_p = make_d2d_device(m_d3d11On12Device_p, m_d2dFactory_p);
   D2D1_DEVICE_CONTEXT_OPTIONS opts = D2D1_DEVICE_CONTEXT_OPTIONS_ENABLE_MULTITHREADED_OPTIMIZATIONS;
   m_d2dDeviceContext_p = make_d2d_context(opts, m_d2dDevice_p);
}

void qgl::graphics::graphics_device::p_make_context_sensitive_members()
{
   m_swapChain_p = nullptr;

   //Set the swap chain format.
   p_set_fmt();

   //Create the swap chain.
   m_swapChain_p = make_swap_chain(m_config,
                                   m_fmt,
                                   m_window_p->width(),
                                   m_window_p->height(),
                                   m_window_p->unknown(),
                                   m_dxgiFactory_p,
                                   m_cmdQueue_p);

   //Set the swap chain color space.
   p_set_color_space();

   m_syncInterval = get_sync_interval(m_config);
   m_swapFlags = get_swap_flag(m_config);
}

void qgl::graphics::graphics_device::p_set_fmt()
{
   m_fmt = FORMAT_8_BIT_NO_HDR;
   if (m_config.hdr())
   {
      //See https://docs.microsoft.com/en-us/windows/desktop/direct3ddxgi/high-dynamic-range-and-wide-color-gamut
      //for HDR info.

      //16 bit is supported on my non-hdr monitors.
      bool use16BitHDR = true;
      if (use16BitHDR)
      {
         m_fmt = FORMAT_16_BIT_HDR;
      }
      else
      {
         m_fmt = FORMAT_10_BIT_HDR;
      }
   }
}

// DirectX supports two combinations of swapchain pixel formats and colorspaces for HDR content.
// Option 1: FP16 + DXGI_COLOR_SPACE_RGB_FULL_G22_NONE_P709
// Option 2: R10G10B10A2 + DXGI_COLOR_SPACE_RGB_FULL_G2084_NONE_P2020 (NOT SUPPORTED ON ALL MONITORS)
void qgl::graphics::graphics_device::p_set_color_space()
{
   //Default color space for 8 and 16 bit DXGI formats.
   DXGI_COLOR_SPACE_TYPE colorSpace = DXGI_COLOR_SPACE_RGB_FULL_G22_NONE_P709;

   switch (m_fmt)
   {
      case FORMAT_8_BIT_NO_HDR:
      {
         colorSpace = DXGI_COLOR_SPACE_RGB_FULL_G22_NONE_P709;
         break;
      }
      case FORMAT_10_BIT_HDR:
      {
         colorSpace = DXGI_COLOR_SPACE_RGB_FULL_G2084_NONE_P2020;
         break;
      }
      case FORMAT_16_BIT_HDR:
      {
         colorSpace = DXGI_COLOR_SPACE_RGB_FULL_G22_NONE_P709;
         break;
      }
      default:
      {
         throw std::exception("Unsupported");
      }
   }

   UINT colorSpaceSupport = 0;
   if (SUCCEEDED(m_swapChain_p->CheckColorSpaceSupport(colorSpace, &colorSpaceSupport)) &&
      ((colorSpaceSupport & DXGI_SWAP_CHAIN_COLOR_SPACE_SUPPORT_FLAG_PRESENT) == DXGI_SWAP_CHAIN_COLOR_SPACE_SUPPORT_FLAG_PRESENT))
   {
      winrt::check_hresult(m_swapChain_p->SetColorSpace1(colorSpace));
   }
   else
   {
      throw std::exception("Unsupported Color Space.");
   }
}

