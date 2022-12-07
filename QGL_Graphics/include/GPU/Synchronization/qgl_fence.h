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
   class fence
   {
      public:
      fence(graphics_device& dev, 
            winrt::com_ptr<icmd_queue>& cmdQueue_p) :
         m_nextSyncValue(1),
         m_cmdQueue(cmdQueue_p)
      {
         //Create a fence using the d3d device.
         winrt::check_hresult(dev.dev_3d()->CreateFence(
            m_nextSyncValue, 
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
      fence(fence&& r) = default;

      /*
        Destructor signals one more time and waits until the event has
        completed before destroying everything.
       */
      ~fence()
      {
         std::lock_guard m{ m_mutex };

         //Get the next sync value which will be the last.
         auto finalSyncValue = m_nextSyncValue;

         //Signal the last sync
         winrt::check_hresult(m_cmdQueue->Signal(get(), finalSyncValue));

         //Wait for the last sync.
         //Get the last completed fence value.
         auto lastCompletedValue = m_fence->GetCompletedValue();

         //If the last completed fence is less than what we are waiting for, 
         //then the GPU has not  completed that fence yet.
         if (lastCompletedValue < finalSyncValue)
         {
            //Wait for the GPU
            winrt::check_hresult(m_fence->SetEventOnCompletion(
               finalSyncValue,
               m_waitEvent.get()));
            WaitForSingleObject(m_waitEvent.get(), INFINITE);
         }

         //Final Cleanup
         m_nextSyncValue = static_cast<ValueT>(-1);

         //Smart pointers cleaned up now.
      }

      /*
       Returns the next synchronization object that can be waited on.
       */
      sync_object<ValueT> next()
      {
         //Lock the mutex so multiple threads cannot simultaneously 
         //get sync_objects with the same value.
         std::lock_guard m{ m_mutex };
         auto ret = m_nextSyncValue;
         m_nextSyncValue++;
         return sync_object<ValueT>{ret};
      }

      /*
       Returns the last completed synchronization object.
       */
      sync_object<ValueT> current() const
      {
         std::lock_guard m{ m_mutex };
         return sync_object<ValueT>{m_fence->GetCompletedValue()};
      }

      /*
       Waits until the fence reaches or passes the synchronization object.
       */
      void wait(const sync_object<ValueT>& waitFor)
      {
         std::lock_guard m{ m_mutex };

         //Get the last completed fence value.
         auto lastCompletedValue = m_fence->GetCompletedValue();

         //If the last completed fence is less than what we are waiting for, 
         //then the GPU has not completed that fence yet.
         if (lastCompletedValue < waitFor.value())
         {
            //Wait for the GPU
            winrt::check_hresult(m_fence->SetEventOnCompletion(
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
         std::lock_guard m{ m_mutex };

         //Update the fence 
         winrt::check_hresult(m_cmdQueue->Signal(get(), toSignal.value()));
      }

      igpu_fence* get()
      {
         return m_fence.get();
      }

      const igpu_fence* get() const
      {
         return m_fence.get();
      }

      const icmd_queue* queue() const
      {
         return m_cmdQueue.get();
      }

      icmd_queue* queue()
      {
         return m_cmdQueue.get();
      }

      private:
      std::mutex m_mutex;
      ValueT m_nextSyncValue;
      winrt::com_ptr<igpu_fence> m_fence;
      winrt::com_ptr<icmd_queue> m_cmdQueue;
      winrt::handle m_waitEvent;
   };
}