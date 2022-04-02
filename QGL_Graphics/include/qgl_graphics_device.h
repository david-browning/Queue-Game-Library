#pragma once
#include "include/qgl_graphics_include.h"
#include "include/qgl_window.h"
#include "include/Content/qgl_device_configuration.h"
#include "include/Helpers/qgl_graphics_device_helpers.h"
#include "include/Helpers/qgl_color_helpers.h"
#include "include/Helpers/qgl_supported_helpers.h"

namespace qgl::graphics
{
   class graphics_device
   {
      public:
      using console_t = typename console<char>;
      using console_ptr = typename std::shared_ptr<console_t>;

      graphics_device(gpu_config&& cfg,
                      std::shared_ptr<window>&& wnd) :
         m_config(std::forward<gpu_config>(cfg)),
         m_wnd_p(std::forward<std::shared_ptr<window>>(wnd))
      {
         make_factories();
         make_devices();
         make_context_sensitive_members();
      }

      graphics_device(gpu_config&& cfg,
                      std::shared_ptr<window>&& wnd,
                      std::initializer_list<console_ptr> cons) :
         m_config(std::forward<gpu_config>(cfg)),
         m_wnd_p(std::forward<std::shared_ptr<window>>(wnd))
      {
         make_factories();
         make_devices(cons);
         make_context_sensitive_members();
      }

      graphics_device(const graphics_device&) = delete;

      graphics_device(graphics_device&&) = default;

      virtual ~graphics_device()
      {
         //Close any contexts that might be executing.
         m_2dContext_p->EndDraw();
         m_2dContext_p->Flush();
         m_d3d11DeviceContext_p->Flush();

         //Order in which com_ptrs are destroyed is imported.
         //Destroy pointers in reverse order they were created.

         for (auto& con_p : m_cons_p)
         {
            m_infoQueue_up->UnregisterMessageCallback(con_p.first);
         }

         m_cons_p.clear();

         m_swapChain_p = nullptr;

         m_infoQueue_up = nullptr;
         m_d2dDeviceContext_p = nullptr;
         m_2dDevice_p = nullptr;
         m_d3d11DeviceContext_p = nullptr;
         m_d3d11On12Device_p = nullptr;
         m_cmdQueues_p.clear();
         m_3dDevice_p = nullptr;

         m_textFactory_p = nullptr;
         m_2dFactory_p = nullptr;
         m_gpuFactory_p = nullptr;
      }

      /*
       Updates the graphics device and swap chain with the new config.
       All render targets must be released before calling this.
       */
      void reset(gpu_config&& cfg)
      {
         auto wasFullScreen = m_config.full_screen();
         m_config = std::forward<gpu_config>(cfg);
         m_2dContext_p->SetTarget(nullptr);
         m_d3d11DeviceContext_p->Flush();

         //Set full-screen / window mode for the window.
         auto fullNow = m_config.full_screen();
         //If we were in full screen and are now not in full screen
         if (wasFullScreen && !fullNow)
         {
            //Exit full screen.
            m_wnd_p->exit_full_screen();
         }
         //Else if we were not in full screen and are not in full screen.
         else if (!wasFullScreen && fullNow)
         {
            //Enter full screen mode.
            m_wnd_p->enter_full_screen();
         }

         helpers::resize_swap_chain(m_config, *m_wnd_p, m_swapChain_p.get());
         set_color_space();

         m_syncInterval = helpers::get_sync_interval(m_config);
         m_swapFlags = helpers::get_swap_flag(m_config);
      }

      const gpu_config& config() const noexcept
      {
         return m_config;
      }

      i3d_device* dev_3d() noexcept
      {
         return m_3dDevice_p.get();
      }

      iswap_chain* swp_chn() noexcept
      {
         return m_swapChain_p.get();
      }

      i2d_context* ctx_2d() noexcept
      {
         return m_2dContext_p.get();
      }

      i3d_bridge_device* dev_back_compat() noexcept
      {
         return m_d3d11On12Device_p.get();
      }

      window* wnd() noexcept
      {
         return m_wnd_p.get();
      }

      const window* wnd() const noexcept
      {
         return m_wnd_p.get();
      }

      private:
      /*
       Creates the factories used to make devices.
       This is not dependent on the graphics configuration so factories are not
       affected by reset().
      */
      void make_factories()
      {
         m_gpuFactory_p = helpers::make_gpu_factory(m_config.console());
         m_2dFactory_p = helpers::make_2d_factory(m_config.console());
         m_textFactory_p = helpers::make_text_factory();
      }

      /*
       Creates the non-context sensitive members like the graphics devices
       and command queue.
       Assume the factories have already been made.
       */
      void make_devices()
      {
         //Enumerate the available adapters.
         auto adapters = helpers::enum_hardware_adapters(m_gpuFactory_p.get(),
            FEATURE_LEVEL);

         //Figure out which adapter is preferred. If the config buffer's 
         //preferred adapter is 0, then there is not preferred adapter. 
         //Use the 0th adapter from the list.
         size_t adapterIdx = 0;
         if (m_config.adapter_devID() != 0)
         {
            //Find the adapter that matches the config's devID.
            for (size_t i = 0; i < adapters.size(); i++)
            {
               DXGI_ADAPTER_DESC3 desc;
               winrt::check_hresult(adapters[i]->GetDesc3(&desc));
               if (desc.DeviceId == m_config.adapter_devID())
               {
                  adapterIdx = i;
                  break;
               }
            }
         }

         m_adapter_p = adapters[adapterIdx];

         //Now that we know which adapter to use, make the 3D device.
         m_3dDevice_p = helpers::make_3d_device(
            m_adapter_p.get(), FEATURE_LEVEL);

         m_infoQueue_up = m_3dDevice_p.as<ID3D12InfoQueue1>();

         //Create the command queue for the device.
         D3D12_COMMAND_QUEUE_DESC desc = {};
         desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
         desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
         auto q1 = helpers::make_cmd_queue(desc, m_3dDevice_p.get());

         //Create the D3D11 Device and context
         m_cmdQueues_p = { q1 };
         make_d3d11_device_and_context();

         //Create the 2d device and context.
         m_2dDevice_p = helpers::make_2d_device(m_d3d11On12Device_p.get(),
                                                m_2dFactory_p.get());

         D2D1_DEVICE_CONTEXT_OPTIONS opts =
            D2D1_DEVICE_CONTEXT_OPTIONS_ENABLE_MULTITHREADED_OPTIMIZATIONS;
         m_d2dDeviceContext_p = helpers::make_2d_context(opts,
                                                          m_2dDevice_p.get());
      }

      void make_devices(std::initializer_list<console_ptr> cons)
      {
         make_devices();
         for (auto& con : cons)
         {
            DWORD cookie;
            //https://microsoft.github.io/DirectX-Specs/d3d/MessageCallback.html
            winrt::check_hresult(m_infoQueue_up->RegisterMessageCallback(
               graphics_device::on_d3d_message,
               D3D12_MESSAGE_CALLBACK_FLAGS::D3D12_MESSAGE_CALLBACK_FLAG_NONE,
               con.get(),
               &cookie));
            if (cookie == 0)
            {
               throw std::system_error{
                  std::make_error_code(std::errc::no_message),
                  "The callback cookie is 0."
               };
            }
            m_cons_p.emplace_back(cookie, std::move(con));
         }
      }

      /*
       DirectX supports two combinations of swapchain pixel formats and
       color spaces for HDR content.
       Option 1: FP16 + DXGI_COLOR_SPACE_RGB_FULL_G22_NONE_P709
       Option 2: R10G10B10A2 + DXGI_COLOR_SPACE_RGB_FULL_G2084_NONE_P2020
         (NOT SUPPORTED ON ALL MONITORS)
       */
      void set_color_space()
      {
         auto colorSpace = helpers::color_space(m_config.hdr_mode());
         if (helpers::color_supported(colorSpace, m_swapChain_p.get()))
         {
            winrt::check_hresult(m_swapChain_p->SetColorSpace1(colorSpace));
         }
         else
         {
            throw std::exception("Unsupported Color Space.");
         }
      }

      /*
       Creates context sensitive members like the swap chain and device
       contexts. If a member requires a context sensitive member in order to
       create it, those non-context sensitive members are created here.
       Ex: D3D11 Device and D2D Device.
       Assume the devices have already been made.
       */
      void make_context_sensitive_members()
      {
         m_output_p = helpers::find_output(m_adapter_p.get(), *m_wnd_p);

         auto fmt = helpers::color_format(m_config.hdr_mode());

         //Create the swap chain. Use the 0th command queue.
         m_swapChain_p = helpers::make_swap_chain(m_config, *m_wnd_p,
            m_gpuFactory_p.get(), m_cmdQueues_p[0].get());

         //Set the swap chain color space.
         set_color_space();

         m_syncInterval = helpers::get_sync_interval(m_config);
         m_swapFlags = helpers::get_swap_flag(m_config);
      }

      void make_d3d11_device_and_context()
      {
         UINT d3d11DeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#ifdef _DEBUG
         {
            d3d11DeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
         }
#endif

         D3D_FEATURE_LEVEL usingFeatureLevel;

         std::vector<IUnknown*> unknowns;
         for (auto& q : m_cmdQueues_p)
         {
            unknowns.push_back(q.get());
         }

         winrt::com_ptr<ID3D11Device> d3d11Dev;
         winrt::check_hresult(D3D11On12CreateDevice(
            m_3dDevice_p.get(),
            d3d11DeviceFlags,
            nullptr, 0,
            unknowns.data(),
            static_cast<UINT>(unknowns.size()),
            0,
            d3d11Dev.put(),
            m_d3d11DeviceContext_p.put(),
            &usingFeatureLevel));

         m_d3d11On12Device_p = std::move(d3d11Dev);
      }

      static void on_d3d_message(D3D12_MESSAGE_CATEGORY cat,
                                 D3D12_MESSAGE_SEVERITY sev,
                                 D3D12_MESSAGE_ID idm,
                                 LPCSTR desc,
                                 void* context_p)
      {
         auto con_p = reinterpret_cast<console_t*>(context_p);
         con_p->cout(desc);
      }

      gpu_config m_config;

      ////////////////////////////////Factories////////////////////////////////
      winrt::com_ptr<igpu_factory> m_gpuFactory_p;
      winrt::com_ptr<i2d_factory> m_2dFactory_p;
      winrt::com_ptr<itext_factory> m_textFactory_p;

      /////////////////////////////////Devices/////////////////////////////////
      winrt::com_ptr<igpu_adapter> m_adapter_p;
      /*
       Used for 3D rendering and work submission.
      */
      winrt::com_ptr<i3d_device> m_3dDevice_p;

      winrt::com_ptr<i2d_device> m_2dDevice_p;

      winrt::com_ptr<i2d_context> m_2dContext_p;

      /*
       Used to provide a D2D device for 2D rendering.
       */
      winrt::com_ptr<i3d_bridge_device> m_d3d11On12Device_p;

      winrt::com_ptr<i3d_context> m_d3d11DeviceContext_p;

      /*
       Device context for the D2D device. Used  for 2D rendering.
       */
      winrt::com_ptr<i2d_context> m_d2dDeviceContext_p;
      /////////////////////////////////////////////////////////////////////////

      std::vector< winrt::com_ptr<icmd_queue>> m_cmdQueues_p;

      winrt::com_ptr<iswap_chain> m_swapChain_p;

      winrt::com_ptr<igpu_output> m_output_p;

      std::shared_ptr<window> m_wnd_p;

      sync_interval_t m_syncInterval;
      swap_flag_t m_swapFlags;

      winrt::com_ptr<ID3D12InfoQueue1> m_infoQueue_up;


      std::vector<std::pair<DWORD, console_ptr>> m_cons_p;

      static constexpr D3D_FEATURE_LEVEL FEATURE_LEVEL = D3D_FEATURE_LEVEL_12_0;
   };

   using graphics_device_ptr = typename std::shared_ptr<graphics_device>;
}