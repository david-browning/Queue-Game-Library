#include "include/qgl_graphics_include.h"
#include "include/Content/Content-Buffers/qgl_graphic_config_buffer.h"
#include "include/qgl_window.h"

namespace qgl::graphics
{
   class QGL_GRAPHICS_API graphics_device
   {
      public:
      graphics_device(const content::buffers::GRAPHICS_CONFIG_BUFFER* config,
                      const std::shared_ptr<window>& wnd_p);


      graphics_device(const graphics_device&) = delete;

      graphics_device(graphics_device&&) = delete;

      virtual ~graphics_device();

      /*
       Updates the graphics device and swap chain with the new config.
       All render targets must be released before calling this.
       */
      _DBC_ void reset(
         const content::buffers::GRAPHICS_CONFIG_BUFFER* newConfig);

      UINT frame_index() const;

      /*
       Returns the number of render target buffers.
       */
      size_t buffers() const noexcept;

      const content::buffers::GRAPHICS_CONFIG_BUFFER* config() const noexcept;

      UINT sync_interval() const noexcept;

      UINT swap_flags() const noexcept;

      DXGI_FORMAT format() const noexcept;

      const d3d_device* d3d12_device() const noexcept;

      d3d_device* d3d12_device() noexcept;

      const d3d11_device* d3d11on12_device() const noexcept;

      d3d11_device* d3d11on12_device() noexcept;

      const d2d_context* d2d1_context() const noexcept;

      d2d_context* d2d1_context() noexcept;

      const d3d11_context* d3d11on12_context() const noexcept;

      d3d11_context* d3d11on12_context() noexcept;

      const d2d_factory* d2d1_factory() const noexcept;

      d2d_factory* d2d1_factory() noexcept;

      const d3d_cmd_queue* cmd_queue() const noexcept;

      d3d_cmd_queue* cmd_queue() noexcept;

      const d3d_swap_chain* swap_chain() const noexcept;

      d3d_swap_chain* swap_chain() noexcept;

      const dxgi_factory* dx_factory() const noexcept;

      dxgi_factory* dx_factory() noexcept;

      const IDXGIAdapter4* adapter() const noexcept;

      IDXGIAdapter4* adapter() noexcept;

      private:
      struct impl;
      impl* m_impl_p;
   };
}