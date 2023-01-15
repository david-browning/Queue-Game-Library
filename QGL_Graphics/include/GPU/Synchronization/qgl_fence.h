#pragma once
#include "include/qgl_graphics_include.h"
#include "include/qgl_graphics_device.h"
#include "include/GPU/Synchronization/qgl_sync_object.h"

namespace qgl::graphics::gpu
{
   /*
    The fence provides a mechanism to synchronize CPU and GPU work.
    This returns and waits on synchronization objects. Use next() to get a
    synchronization object and wait() to wait for the GPU to finish using it.
    This class is thread-safe, so the order in which next() and wait()
    are called will be correct.
    */
   template<typename ValueT>
   class fence final
   {
      public:
      fence(graphics_device& dev,
            icmd_queue* cmdQueue_p) :
         m_cmdQueue(cmdQueue_p),
         m_curSync(1)
      {
         //Create a fence using the d3d device.
         check_result(dev.dev_3d()->CreateFence(
            m_curSync,
            D3D12_FENCE_FLAG_NONE,
            IID_PPV_ARGS(m_fence.put())));

         //Set up an event to use when waiting for the GPU.
         auto evnt = CreateEvent(nullptr, FALSE, FALSE, nullptr);
         if (evnt != nullptr)
         {
            m_waitEvent.attach(evnt);
         }
         else
         {
            winrt::throw_last_error();
         }
      }

      /*
       Do not allow copying this synchronization mechanism.
       */
      fence(const fence& r) = delete;

      /*
       Move Constructor
       */
      fence(fence&& r) noexcept = default;

      /*
        Destructor signals one more time and waits until the event has
        completed before destroying everything.
       */
      ~fence()
      {
         wait(m_curSync.load());

#ifdef DEBUG
         m_curSync.store(static_cast<ValueT>(-1));
#endif
      }

      /*
       Returns the next synchronization object that can be waited on.
       */
      sync_object<ValueT> next()
      {
         return sync_object<ValueT>{ ++m_curSync };
      }

      /*
       Returns the current synchronization object.
       */
      sync_object<ValueT> current()
      {
         return sync_object<ValueT>{ m_curSync.load() };
      }

      /*
       Waits until the fence reaches or passes the synchronization object.
       */
      void wait(const sync_object<ValueT>& waitFor)
      {
         //If the last completed fence is less than what we are waiting for, 
         //then the GPU has not completed that fence yet.
         if (m_fence->GetCompletedValue() < waitFor.value())
         {
            //Wait for the GPU
            check_result(m_fence->SetEventOnCompletion(
               waitFor.value(),
               m_waitEvent.get()));
            WaitForSingleObject(m_waitEvent.get(), INFINITE);
         }
      }

      /*
       Use this to signal that the GPU is done with the sync_object.
       */
      void signal(const sync_object<ValueT>& toSignal)
      {
         check_result(m_cmdQueue->Signal(get(), toSignal.value()));
      }

      /*
       Gets the platform specific fence.
       */
      igpu_fence* get()
      {
         return m_fence.get();
      }

      /*
       Gets the platform specific fence.
       */
      const igpu_fence* get() const
      {
         return m_fence.get();
      }

      /*
       Gets a pointer to the command queue.
       */
      const icmd_queue* queue() const
      {
         return m_cmdQueue;
      }

      /*
       Gets a pointer to the command queue.
       */
      icmd_queue* queue()
      {
         return m_cmdQueue;
      }

      private:
      /*
       Value of the current synchronization value.
       */
      std::atomic<ValueT> m_curSync;
      
      /*
       Fence object.
       */
      pptr<igpu_fence> m_fence;

      /*
       The fence uses this queue.
       */
      icmd_queue* m_cmdQueue;

      /*
       Handle used to wait for the GPU to catch up with the sync object.
       */
      winrt::handle m_waitEvent;
   };
}