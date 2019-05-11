#pragma once
#include "qgl_graphics_include.h"
#include "qgl_sync_object.h"
#include <atomic>

namespace qgl::graphics
{
   /*
    The fence provides a mechanism to synchronize CPU and GPU work.
    This returns and waits on synchronization objects. Use next() to get a
    synchronization object and wait() to wait for the GPU to finish using it.
    This class is thread-safe, so the order in which next() and wait() are called will
    be correct.
    */
   template<typename ValueT>
   class LIB_EXPORT fence
   {
      public:
      fence(winrt::com_ptr<d3d_device>& dev_p,
            winrt::com_ptr<d3d_cmd_queue>& cmdQueue_p) :
         m_nextSyncValue(1),
         m_cmdQueue(cmdQueue_p),
         m_mutex(),
         m_waitEvent()
      {
         //Create a fence using the d3d device.
         winrt::check_hresult(dev_p->CreateFence(m_nextSyncValue,
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
        Destructor signals one more time and waits until the event has completed
        before destroying everything.
       */
      virtual ~fence()
      {
         //Lock the mutex.
         m_mutex.lock();

         //Get the next sync value which will be the last.
         auto finalSyncValue = m_nextSyncValue;

         //Signal the last sync
         winrt::check_hresult(m_cmdQueue->Signal(get(), finalSyncValue));

         //Wait for the last sync.
         //Get the last completed fence value.
         auto lastCompletedValue = p_current();

         //If the last completed fence is less than what we are waiting for, then the GPU has not 
         //completed that fence yet.
         if (lastCompletedValue < finalSyncValue)
         {
            //Wait for the GPU
            winrt::check_hresult(m_fence->SetEventOnCompletion(finalSyncValue, m_waitEvent.get()));
            WaitForSingleObject(m_waitEvent.get(), INFINITE);
         }

         //Unlock the mutex.
         m_mutex.unlock();

         //Final Cleanup
         m_nextSyncValue = static_cast<ValueT>(-1);

         //Smart pointers cleaned up now.
      }

      /*
       Returns the next synchronization object that can be waited on.
       */
      sync_object<ValueT> next()
      {
         //Lock the mutex so multiple threads cannot simultaneously get sync_objects with
         //the same value.
         m_mutex.lock();
         auto ret = m_nextSyncValue;
         m_nextSyncValue++;
         m_mutex.unlock();
         return sync_object<ValueT>(ret);
      }

      /*
       Returns the last completed synchronization object.
       */
      sync_object<ValueT> current() const
      {
         m_mutex.lock();
         auto lastCompletedValue = p_current();
         m_mutex.unlock();
         return sync_object<ValueT>(lastCompletedValue);
      }

      /*
       Waits until the fence reaches or passes the synchronization object.
       */
      void wait(const sync_object<ValueT>& waitFor)
      {
         //Lock the mutex.
         m_mutex.lock();

         //Get the last completed fence value.
         auto lastCompletedValue = p_current();

         //If the last completed fence is less than what we are waiting for, then the GPU has not 
         //completed that fence yet.
         if (lastCompletedValue < waitFor.value())
         {
            //Wait for the GPU
            winrt::check_hresult(m_fence->SetEventOnCompletion(waitFor.value(), m_waitEvent.get()));
            WaitForSingleObject(m_waitEvent.get(), INFINITE);
         }

         //Unlock the mutex.
         m_mutex.unlock();
      }

      /*
       Use this to signal that the GPU is done with the sync_object.
       */
      void signal(const sync_object<ValueT>& toSignal)
      {
         //Lock the mutex.
         m_mutex.lock();

         //Update the fence 
         winrt::check_hresult(m_cmdQueue->Signal(get(), toSignal.value()));

         //Unlock the mutex.
         m_mutex.unlock();
      }

      #pragma region D3D Fence Getters
      inline d3d_fence* get()
      {
         return m_fence.get();
      }

      inline const d3d_fence* get() const
      {
         return m_fence.get();
      }

      inline winrt::com_ptr<d3d_fence>& com_get()
      {
         return m_fence;
      }

      inline const winrt::com_ptr<d3d_fence>& com_get() const
      {
         return m_fence;
      }
      #pragma endregion

      inline const d3d_cmd_queue* queue() const
      {
         return m_cmdQueue.get();
      }

      inline d3d_cmd_queue* queue()
      {
         return m_cmdQueue.get();
      }

      inline const winrt::com_ptr<d3d_cmd_queue>& com_queue() const
      {
         return m_cmdQueue;
      }

      inline winrt::com_ptr<d3d_cmd_queue>& com_queue()
      {
         return m_cmdQueue;
      }

      private:

      ValueT p_current() const
      {
         return static_cast<ValueT>(m_fence->GetCompletedValue());
      }

      mutable std::mutex m_mutex;
      ValueT m_nextSyncValue;
      winrt::com_ptr<d3d_fence> m_fence;
      winrt::com_ptr<d3d_cmd_queue> m_cmdQueue;
      winrt::handle m_waitEvent;
   };
}