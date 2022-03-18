#pragma once
#include "include/qgl_graphics_include.h"

namespace qgl::graphics
{
   class iwindow;
   class device_configuration;
   namespace descriptors
   {
      class engine_descriptor;
   }

   QGL_INTERFACE igraphics_device : public iqgl
   {
      public:
      /*
       Updates the graphics device and swap chain with the new config.
       All render targets must be released before calling this.
       */
      virtual void reset(const descriptors::engine_descriptor* newConfig) = 0;

      virtual const device_configuration* config() const noexcept = 0;

      virtual d3d_device* d3d12_device() noexcept = 0;

      virtual d3d11_device* d3d11on12_device() noexcept = 0;

      virtual d2d_context* d2d1_context() noexcept = 0;

      virtual d3d11_context* d3d11on12_context() noexcept = 0;

      virtual d2d_factory* d2d1_factory() noexcept = 0;

      virtual d3d_cmd_queue* cmd_queue() noexcept = 0;

      virtual d3d_swap_chain* swap_chain() noexcept = 0;

      virtual dxgi_factory* dx_factory() noexcept = 0;

      virtual IDXGIAdapter4* adapter() noexcept = 0;
   };

   using graphics_device_ptr = typename std::shared_ptr<igraphics_device>;

   /*
    wnd: Pointer to a window the graphics device wraps.
    */
   extern "C" QGL_GRAPHICS_API HRESULT make_graphics_device(
      const descriptors::engine_descriptor* config,
      iwindow* wnd,
      qgl_version_t v,
      igraphics_device** out_p);
}