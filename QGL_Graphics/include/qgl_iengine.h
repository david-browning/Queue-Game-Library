#pragma once
#include "qgl_graphics_include.h"
#include "qgl_fence.h"
#include "qgl_sync_object.h"

namespace qgl::graphics
{
   /* 
     Provides a mechanism to submit work to the GPU.
     Queuing, signaling, and waiting are thread safe. Flushing and clearing are not.
     There should be only 1 thread responsible for flushing queued operations. Additional 
     synchronization is required so that the owning thread waits for all queue operations to 
     complete.
    */
   template<typename ValueT>
   class iengine
   {
      public:
      iengine(const std::shared_ptr<fence<ValueT>>& engineFence_p) :
         m_fence_p(engineFence_p)
      {

      }

      /*
       Do not allow copying an engine.
       */
      iengine(const iengine& r) = delete;

      iengine(iengine&& r) :
         m_fence_p(std::move(r.m_fence_p))
      {

      }

      virtual ~iengine()
      {

      }
      
      /*
       Waits for a fence to have a specific value.
       Get's the fence's current value and if that value is greater than the
       synchronization point, waits until the fence has reached the synchronization point.
       */
      void wait(const sync_object<ValueT>& fenceValue)
      {
         m_fence_p->wait(fenceValue);
      }

      /*
       GPU signals that it has completed using the sync_object.
       */
      void signal(const sync_object<ValueT>& fenceValue)
      {
         m_fence_p->signal(fenceValue);
      }

      /*
       Puts the engine in a state so that work can be queued.
       Often, this means the underlying command list needs to be in the recording state.
       */
      virtual void begin() = 0;

      /*
       Flush and execute all pending operations. Most likely this means calling
       m_cmdQueue->ExecuteLists().
       Get the command queue using m_fence_p->queue().
       */
      virtual sync_object<ValueT> flush() = 0;

      protected:

      /*
       Multiple engines will share the same fence so they can synchronize with each other.
       */
      std::shared_ptr<fence<ValueT>> m_fence_p;

      private:
   };
}