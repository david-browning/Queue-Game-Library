#pragma once
#include "include/qgl_graphics_include.h"
#include "include/qgl_window.h"
#include "include/qgl_device_configuration.h"
#include "include/Helpers/qgl_graphics_device_helpers.h"
#include "include/Helpers/qgl_color_helpers.h"
#include "include/Helpers/qgl_supported_helpers.h"
#include "include/GPU/qgl_gpu_msg_dispatchers.h"
#include <QGLModel.h>

namespace qgl::graphics
{
   /*
    Represents a GPU.
    */
   class graphics_device
   {
      public:
      using budget_dispatcher = typename callback_dispatcher<
         gpu_budget_changed_callback, 
         gpu_budget_changed_callback_args,
         basic_callback_dispatcher_traits>;
      
      using msg_dispatcher = typename callback_dispatcher<
         gpu_msg_callback, 
         ID3D12InfoQueue*, 
         basic_callback_dispatcher_traits>;

      graphics_device(const gpu_config& cfg,
                      const std::shared_ptr<window>& wnd) :
         m_config(cfg),
         m_wnd_p(wnd),
         m_budgetDispatcher(impl::gpu_budget_changed_dispatcher),
         m_msgDispatcher(impl::gpu_msg_dispatcher)
      {
         make_factories();
         make_devices();
         make_context_sensitive_members();
      }

      graphics_device(const graphics_device&) = delete;

      graphics_device(graphics_device&&) = default;

      virtual ~graphics_device()
      {
         // Kill the dispatchers first
         m_msgDispatcher.kill();
         m_budgetDispatcher.kill();
         
         //Order in which com_ptrs are destroyed is imported.
         //Destroy pointers in reverse order they were created.
         m_swapChain_p = nullptr;
         m_queue_p = nullptr;
         m_infoQueue_up = nullptr;
         //m_cmdQueueMap.clear();
         m_3dDevice_p = nullptr;

         m_gpuFactory_p = nullptr;

         // GPU callback object's destructor takes care of closing the threads
         // and consoles.
      }

      /*
       Updates the graphics device and swap chain with the new config.
       All render targets must be released before calling this.
       */
      void reset(const gpu_config& cfg)
      {
         auto wasFullScreen = m_config.full_screen();
         m_config = cfg;

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

      void suspend()
      {
         // https://learn.microsoft.com/en-us/windows/win32/api/dxgi1_3/nf-dxgi1_3-idxgidevice3-trim
         auto asDXGI = m_3dDevice_p.as<IDXGIDevice3>();
         asDXGI->Trim();
      }

      /*
       Presents the back buffer.
       */
      void present()
      {
         check_result(swp_chn()->Present(m_syncInterval, m_swapFlags));
      }

      /*
       Requests the OS reserve "bytes" memory for the application.
       This function can easily throw so the caller should inspect the return
       value.
       https://learn.microsoft.com/en-us/windows/win32/api/dxgi1_4/nf-dxgi1_4-idxgiadapter3-setvideomemoryreservation
       */
      result_t reserve(size_t bytes) noexcept
      {
         return adapter()->SetVideoMemoryReservation(
            dev_node(),
            DXGI_MEMORY_SEGMENT_GROUP::DXGI_MEMORY_SEGMENT_GROUP_LOCAL,
            static_cast<UINT64>(bytes));
      }

      /*
       Returns a description of the graphics device. This includes information
       like total memory and GPU name.
       */
      const gpu_desc& desc() const noexcept
      {
         return m_gpuDesc;
      }

      /*
       Gets the index of the current frame.
       This gets incremented whenever the back buffer is presented.
       */
      size_t buffer_idx() const noexcept
      {
         return m_swapChain_p->GetCurrentBackBufferIndex();
      }

      /*
       Returns the node of which graphics device this class represents.
       */
      gpu_idx_t dev_node() const noexcept
      {
         return m_config.adapter_devID();
      }

      /*
       Returns a description of how the GPU is configured.
       */
      const gpu_config& config() const noexcept
      {
         return m_config;
      }

      /*
       Returns the display adapter interface.
       */
      igpu_adapter* adapter() noexcept
      {
         return m_adapter_p.get();
      }

      /*
       Returns the display adapter interface.
       */
      const igpu_adapter* adapter() const noexcept
      {
         return m_adapter_p.get();
      }

      /*
       Returns the 3D device interface.       
       */
      i3d_device* dev_3d() noexcept
      {
         return m_3dDevice_p.get();
      }

      /*
       Returns the swap chain interface.
       */
      iswap_chain* swp_chn() noexcept
      {
         return m_swapChain_p.get();
      }

      /*
       Returns a pointer to the window the graphics device is tied to.
       */
      window* wnd() noexcept
      {
         return m_wnd_p.get();
      }

      /*
       Returns a pointer to the window the graphics device is tied to.
       */
      const window* wnd() const noexcept
      {
         return m_wnd_p.get();
      }

      /*
       Returns the default command queue interface.
       */
      icmd_queue* default_queue() noexcept
      {
         return m_queue_p.get();
      }

      /*
       Returns the default command queue interface.
       */
      const icmd_queue* default_queue() const noexcept
      {
         return m_queue_p.get();
      }

      /*
       Returns a reference to the GPU memory budget dispatcher. Use it to 
       register and revoke callbacks.
       */
      budget_dispatcher& bdgt_dispatcher()
      {
         return m_budgetDispatcher;
      }

      /*
       Returns a reference to the message dispatcher. Use it to register and 
       revoke GPU message callbacks.
       */
      msg_dispatcher& message_dispatcher()
      {
         return m_msgDispatcher;
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
         gpu_idx_t adapterIdx = 0;
         if (m_config.adapter_devID() != 0)
         {
            //Find the adapter that matches the config's devID.
            for (size_t i = 0; i < adapters.size(); i++)
            {
               gpu_desc desc;
               check_result(helpers::get_gpu_desc(adapters[i].get(), &desc));
               if (desc.dev_id == m_config.adapter_devID())
               {
                  adapterIdx = static_cast<gpu_idx_t>(i);
                  break;
               }
            }
         }

         m_adapter_p = adapters[adapterIdx];
         check_result(helpers::get_gpu_desc(m_adapter_p.get(), &m_gpuDesc));

         //Now that we know which adapter to use, make the 3D device.
         m_3dDevice_p = helpers::make_3d_device(
            m_adapter_p.get(), FEATURE_LEVEL);

         if (m_config.console())
         {
            m_infoQueue_up = m_3dDevice_p.as<ID3D12InfoQueue>();
         }

         //Create the command queue for the device.
         D3D12_COMMAND_QUEUE_DESC desc = {};
         desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
         desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
         m_queue_p = helpers::make_cmd_queue(desc, dev_3d());

         gpu_budget_changed_callback_args budgetArgs { 
            m_adapter_p.get(), adapterIdx 
         };
         
         m_budgetDispatcher.start(budgetArgs);
         m_msgDispatcher.start(m_infoQueue_up.get());
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
            check_result(m_swapChain_p->SetColorSpace1(colorSpace));
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
         if (m_config.full_screen())
         {
            m_wnd_p->enter_full_screen();
         }

         m_output_p = helpers::find_output(m_adapter_p.get(), *m_wnd_p);

         //Create the swap chain. Use the 0th command queue.
         m_swapChain_p = helpers::make_swap_chain(m_config, *m_wnd_p,
            m_gpuFactory_p.get(), m_queue_p.get());

         //Set the swap chain color space.
         set_color_space();

         m_syncInterval = helpers::get_sync_interval(m_config);
         m_swapFlags = helpers::get_swap_flag(m_config);
      }

      gpu_config m_config;

      std::shared_ptr<window> m_wnd_p;
      pptr<ID3D12InfoQueue> m_infoQueue_up;
      pptr<iswap_chain> m_swapChain_p;
      pptr<igpu_output> m_output_p;
      pptr<igpu_factory> m_gpuFactory_p;
      pptr<igpu_adapter> m_adapter_p;
      pptr<i3d_device> m_3dDevice_p;
      pptr<icmd_queue> m_queue_p;

      budget_dispatcher m_budgetDispatcher;
      msg_dispatcher m_msgDispatcher;

      ///*
      // Maps handles to command queues.
      // */
      //handle_map<pptr<icmd_queue>> m_cmdQueueMap;

      ///*
      // Handle to the default command queue.
      // */
      //hndlmap_t m_defaultCmdQH;

      sync_interval_t m_syncInterval;
      swap_flag_t m_swapFlags;

      /*
       Description of the GPU adapter.
       */
      gpu_desc m_gpuDesc;

      static constexpr D3D_FEATURE_LEVEL FEATURE_LEVEL = D3D_FEATURE_LEVEL_12_0;
   };
}