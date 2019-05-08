#pragma once
#include "qgl_graphics_include.h"
#include "qgl_window.h"
#include "qgl_graphic_config_buffer.h"

namespace qgl::graphics
{
class LIB_EXPORT graphics_device
   {
      public:
      using appview_pt = std::shared_ptr<winrt::Windows::UI::ViewManagement::ApplicationView>;
      using corewin_pt = std::shared_ptr<winrt::Windows::UI::Core::CoreWindow>;

      /*
       Creates the device using the graphics config and builds a window using the app view and
       core window.
       */
      graphics_device(const GRAPHICS_CONFIG_BUFFER& config,
                      const appview_pt& appView_p,
                      const corewin_pt& coreWindow_p);
      /*
       Creates the device by moving the graphics config and builds a window using the app view
       and core window.
       */
      graphics_device(const GRAPHICS_CONFIG_BUFFER&& config,
                      const appview_pt& appView_p,
                      const corewin_pt& coreWindow_p);

      graphics_device(const GRAPHICS_CONFIG_BUFFER& config,
                      const std::shared_ptr<window>& wnd_p);

      graphics_device(const GRAPHICS_CONFIG_BUFFER&& config,
                      const std::shared_ptr<window>& wnd_p);

      graphics_device(const GRAPHICS_CONFIG_BUFFER& config,
                      const std::shared_ptr<window>&& wnd_p);

      graphics_device(const GRAPHICS_CONFIG_BUFFER&& config,
                      const std::shared_ptr<window>&& wnd_p);

      graphics_device(const graphics_device&) = delete;

      graphics_device(graphics_device&&) = delete;

      virtual ~graphics_device();

      /*
       Updates the graphics device and swap chain with the new config.
       All render targets must be released before calling this.
       */
      _DBC_ void reset(GRAPHICS_CONFIG_BUFFER& newConfig);

      inline UINT frame_index() const
      {
         return m_swapChain_p->GetCurrentBackBufferIndex();
      }

      inline size_t buffers() const
      {
         return m_config.buffers();
      }

      inline GRAPHICS_CONFIG_BUFFER& config()
      {
         return m_config;
      }

      inline const GRAPHICS_CONFIG_BUFFER& config() const
      {
         return m_config;
      }

      inline const window& wnd() const
      {
         return *m_window_p;
      }

      inline window& wnd()
      {
         return *m_window_p;
      }

      inline auto sync_interval() const
      {
         return m_syncInterval;
      }

      inline auto swap_flags() const
      {
         return m_swapFlags;
      }

      inline auto format() const
      {
         return m_fmt;
      }

      #pragma region Device, Factory, Context Getters
      inline const d3d_device* d3d12_device() const
      {
         return m_d3d12Device_p.get();
      }

      inline d3d_device* d3d12_device()
      {
         return m_d3d12Device_p.get();
      }

      inline const winrt::com_ptr<d3d_device>& com_d3d12_device() const
      {
         return m_d3d12Device_p;
      }

      inline winrt::com_ptr<d3d_device>& com_d3d12_device()
      {
         return m_d3d12Device_p;
      }

      inline const d3d11_device* d3d11on12_device() const
      {
         return m_d3d11On12Device_p.get();
      }

      inline d3d11_device* d3d11on12_device()
      {
         return m_d3d11On12Device_p.get();
      }

      inline const winrt::com_ptr<d3d11_device>& com_d3d11on12_device() const
      {
         return m_d3d11On12Device_p;
      }

      inline winrt::com_ptr<d3d11_device>& com_d3d11on12_device()
      {
         return m_d3d11On12Device_p;
      }

      inline const d2d_context* d2d1_context() const
      {
         return m_d2dDeviceContext_p.get();
      }

      inline d2d_context* d2d1_context()
      {
         return m_d2dDeviceContext_p.get();
      }

      inline const winrt::com_ptr<d2d_context>& com_d2d1_context() const
      {
         return m_d2dDeviceContext_p;
      }

      inline winrt::com_ptr<d2d_context>& com_d2d1_context()
      {
         return m_d2dDeviceContext_p;
      }

      inline const d3d11_context* d3d11on12_context() const
      {
         return m_d3d11DeviceContext_p.get();
      }

      inline d3d11_context* d3d11on12_context()
      {
         return m_d3d11DeviceContext_p.get();
      }

      inline const winrt::com_ptr<d3d11_context> com_d3d11on12_context() const
      {
         return m_d3d11DeviceContext_p;
      }

      inline winrt::com_ptr<d3d11_context> com_d3d11on12_context()
      {
         return m_d3d11DeviceContext_p;
      }

      inline const d2d_factory* d2d1_factory() const
      {
         return m_d2dFactory_p.get();
      }

      inline d2d_factory* d2d1_factory()
      {
         return m_d2dFactory_p.get();
      }

      inline const winrt::com_ptr<d2d_factory>& com_d2d1_factory() const
      {
         return m_d2dFactory_p;
      }

      inline winrt::com_ptr<d2d_factory>& com_d2d1_factory()
      {
         return m_d2dFactory_p;
      }

      inline const winrt::com_ptr<d_write_factory>& com_write_factory() const
      {
         return m_dWriteFactory_p;
      }

      inline winrt::com_ptr<d_write_factory>& com_write_factory()
      {
         return m_dWriteFactory_p;
      }

      inline const d3d_cmd_queue* cmd_queue() const
      {
         return m_cmdQueue_p.get();
      }

      inline d3d_cmd_queue* cmd_queue()
      {
         return m_cmdQueue_p.get();
      }

      inline const winrt::com_ptr<d3d_cmd_queue>& com_cmd_queue() const
      {
         return m_cmdQueue_p;
      }

      inline winrt::com_ptr<d3d_cmd_queue>& com_cmd_queue()
      {
         return m_cmdQueue_p;
      }

      inline const d3d_swap_chain* swap_chain() const
      {
         return m_swapChain_p.get();
      }

      inline d3d_swap_chain* swap_chain()
      {
         return m_swapChain_p.get();
      }

      inline const winrt::com_ptr<d3d_swap_chain>& com_swap_chain() const
      {
         return m_swapChain_p;
      }

      inline winrt::com_ptr<d3d_swap_chain>& com_swap_chain()
      {
         return m_swapChain_p;
      }

      inline const dxgi_factory* dx_factory() const
      {
         return m_dxgiFactory_p.get();
      }

      inline dxgi_factory* dx_factory()
      {
         return m_dxgiFactory_p.get();
      }

      inline const winrt::com_ptr<dxgi_factory>& com_dx_factory() const
      {
         return m_dxgiFactory_p;
      }

      inline winrt::com_ptr<dxgi_factory>& com_dx_factory()
      {
         return m_dxgiFactory_p;
      }

      inline const IDXGIAdapter4* adapter() const
      {
         return m_usingAdapter.get();
      }

      inline IDXGIAdapter4* adapter()
      {
         return m_usingAdapter.get();
      }

      inline const winrt::com_ptr<IDXGIAdapter4>& com_adapter() const
      {
         return m_usingAdapter;
      }

      inline winrt::com_ptr<IDXGIAdapter4>& com_adapter()
      {
         return m_usingAdapter;
      }

      #pragma endregion

      protected:

      #pragma warning(push)
      #pragma warning(disable: 4251)
      //////////////////////////////////////////Factories//////////////////////////////////////////
      /*
       Provides the mechanism to create adapters.
       */
      winrt::com_ptr<dxgi_factory> m_dxgiFactory_p;

      /*
       Provides the mechanism to create D2D devices.
       */
      winrt::com_ptr<d2d_factory> m_d2dFactory_p;

      /*
       Provides the mechanism to create text formats for rendering text.
       */
      winrt::com_ptr<d_write_factory> m_dWriteFactory_p;

      ///////////////////////////////////////////Devices///////////////////////////////////////////
      /*
       D3D12 Device. Used for 3D rendering and work submission.
      */
      winrt::com_ptr<d3d_device> m_d3d12Device_p;

      /*
       D3D11 Device. Used to provide a D2D device for 2D rendering.
       */
      winrt::com_ptr<d3d11_device> m_d3d11On12Device_p;

      /*
       Device context for the D3D11 Device.
       */
      winrt::com_ptr<d3d11_context> m_d3d11DeviceContext_p;

      /*
       D2D Device.
       */
      winrt::com_ptr<d2d_device> m_d2dDevice_p;

      /*
       Device context for the D2D device. Used  for 2D rendering.
       */
      winrt::com_ptr<d2d_context> m_d2dDeviceContext_p;
      /////////////////////////////////////////////////////////////////////////////////////////////

      winrt::com_ptr<d3d_cmd_queue> m_cmdQueue_p;

      winrt::com_ptr<d3d_swap_chain> m_swapChain_p;

      std::shared_ptr<window> m_window_p;

      std::vector<winrt::com_ptr<IDXGIAdapter4>> m_adapters;

      winrt::com_ptr<IDXGIAdapter4> m_usingAdapter;

      #pragma warning(pop)

      GRAPHICS_CONFIG_BUFFER m_config;


      UINT m_swapFlags;
      UINT m_syncInterval;

      DXGI_FORMAT m_fmt;

      private:
      /*
       Constructs the class by making the factories, graphics devices, allocators, swap chain,
       and device contexts.
       */
      void construct();

      /*
       Creates the factories used to make devices.
       This is not dependent on the graphics configuration so factories are not
       affected by reset().
      */
      void p_make_factories();

      /*
       Creates the non-context sensitive members like the graphics devices and command
       queue.
       Assume the factories have already been made.
       */
      _DBC_ void p_make_graphics();

      /*
       Creates context sensitive members like the swap chain and device contexts.
       If a member requires a context sensitive member in order to create it, those non-context
       sensitive members are created here. Ex: D3D11 Device and D2D Device.
       Assume the devices have already been made.
       */
      void p_make_context_sensitive_members();

      void p_set_fmt();

      void p_set_color_space();

      bool m_wasFullScreen;

      #pragma warning(push)
      #pragma warning(disable: 4251)
      std::vector<IUnknown*> m_queues;
      #pragma warning(pop)
   };
}