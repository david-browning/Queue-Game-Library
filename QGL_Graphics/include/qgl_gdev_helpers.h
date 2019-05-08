#pragma once
#include "qgl_graphics_include.h"
#include "qgl_graphic_config_buffer.h"

namespace qgl::graphics
{
   /*
    Returns true if the dxgi factory supports tearing on the monitor.
    */
   extern LIB_EXPORT bool support_tearing(const winrt::com_ptr<dxgi_factory>& factory_p);

   /*
    Returns true if the monitor supports HDR.
    HDR information is included in "desc".
    */
   extern LIB_EXPORT bool support_hdr(const winrt::com_ptr<IDXGIOutput6>& output,
                                      DXGI_OUTPUT_DESC1& desc);

   /*
    Returns true if the monitor supports stereoscopic rendering.
    */
   extern LIB_EXPORT bool support_stereo();

   /*
    Returns the correct swap chain flag to use for the given graphics config.
    See https://docs.microsoft.com/en-us/windows/desktop/direct3ddxgi/dxgi-present
    */
   extern LIB_EXPORT UINT get_swap_flag(const GRAPHICS_CONFIG_BUFFER& cnfg) noexcept;

   /*
    Returns the synchronization interval to use for the swap chain.
    See https://docs.microsoft.com/en-us/windows/desktop/api/dxgi/nf-dxgi-idxgiswapchain-present
    */
   extern LIB_EXPORT UINT get_sync_interval(const GRAPHICS_CONFIG_BUFFER& cnfg) noexcept;

   /*
    Returns a vector of display modes supported by the monitor and the DXGI_FORMAT.
    Pass true for includeStereo to enumerate stereo modes.
    */
   extern LIB_EXPORT std::vector<DXGI_MODE_DESC1> enum_display_modes(const winrt::com_ptr<IDXGIOutput6>& monitor,
                                                                     DXGI_FORMAT monitorFmt,
                                                                     bool includeInterlacing,
                                                                     bool includeStereo);

   extern LIB_EXPORT std::vector<winrt::com_ptr<IDXGIAdapter4>> enum_hardware_adapters(
      const winrt::com_ptr<dxgi_factory>& factory_p,
      D3D_FEATURE_LEVEL minFeatureLevel);

   extern LIB_EXPORT winrt::com_ptr<d3d_device> make_d3d12_device(
      const winrt::com_ptr<IDXGIAdapter4> adapter_p,
      D3D_FEATURE_LEVEL featureLevel);

   extern LIB_EXPORT winrt::com_ptr<d3d_cmd_queue> make_cmd_queue(
      const D3D12_COMMAND_QUEUE_DESC& desc,
      const winrt::com_ptr<d3d_device>& dev_p);

   extern LIB_EXPORT std::pair<winrt::com_ptr<d3d11_device>, winrt::com_ptr<d3d11_context>> make_d3d11_dev_and_context(
      const winrt::com_ptr<d3d_device>& d3d12Dev_p,
      const std::vector<IUnknown*> queues);

   extern LIB_EXPORT winrt::com_ptr<d2d_device> make_d2d_device(
      const winrt::com_ptr<d3d11_device>& d3d11on12Dev_p,
      const winrt::com_ptr<d2d_factory>& d2dFactory_p);

   extern LIB_EXPORT winrt::com_ptr<d2d_context> make_d2d_context(
      const D2D1_DEVICE_CONTEXT_OPTIONS& opts,
      const winrt::com_ptr<d2d_device>& d2dDev_p);

   extern LIB_EXPORT winrt::com_ptr<d3d_swap_chain> make_swap_chain(
      const GRAPHICS_CONFIG_BUFFER& config,
      DXGI_FORMAT fmt,
      UINT windowWidth,
      UINT windowHeight,
      IUnknown* const unknownWindow,
      const winrt::com_ptr<dxgi_factory>& dxgiFactory_p,
      const winrt::com_ptr<d3d_cmd_queue>& cmdQueue_p);

   extern LIB_EXPORT void resize_swap_chain(UINT bufferCount,
                                            UINT windowWidth,
                                            UINT windowHeight,
                                            winrt::com_ptr<d3d_swap_chain>& swpChain);
}