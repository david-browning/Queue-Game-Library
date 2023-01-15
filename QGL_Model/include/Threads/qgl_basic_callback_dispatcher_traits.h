#pragma once
#include "include/qgl_model_include.h"

namespace qgl
{
   /*
    Basic traits for a callback dispatcher. This provides methods to start a
    thread, signal an event, and wait.
    */
   struct basic_callback_dispatcher_traits final
   {
      public:
      using thread_entry = typename LPTHREAD_START_ROUTINE;

      basic_callback_dispatcher_traits()
      {

      }

      basic_callback_dispatcher_traits(
         const basic_callback_dispatcher_traits&) = default;

      basic_callback_dispatcher_traits(
         basic_callback_dispatcher_traits&&) noexcept = default;

      ~basic_callback_dispatcher_traits() noexcept = default;

      /*
       Invokes a new thread starting at "entry" and passes "args" to it.
       "args" is not copied. Do not allow it to be destroyed unless the
       invoked thread is done.
       Returns a handle to the thread.
       */
      phandle invoke_thread(thread_entry entry, void* args)
      {
         auto ret = CreateThread(
            nullptr, 0,
            entry, args,
            0, nullptr);

         if (!ret)
         {
            winrt::throw_last_error();
         }

         return phandle{ ret };
      }

      /*
       Waits for the given handle to signal.
       */
      void wait(const phandle& h)
      {
         auto ret = WaitForSingleObject(h.get(), INFINITE);
         if (ret == WAIT_FAILED)
         {
            winrt::throw_last_error();
         }
      }

      /*
       Signals the given handle.
       */
      void signal(const phandle& h)
      {
         winrt::check_bool(SetEvent(h.get()));
      }

      /*
       Returns true if the handle was signaled.
       */
      bool signaled(const phandle& h) const
      {
         auto ret = WaitForSingleObject(h.get(), 0);
         if (ret == WAIT_OBJECT_0)
         {
            return true;
         }
         else if (ret == WAIT_TIMEOUT)
         {
            return false;
         }

         winrt::throw_last_error();
      }
   };
}