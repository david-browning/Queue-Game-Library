#pragma once
#include "include/qgl_model_include.h"
#include "include/Structures/qgl_ts_uset.h"
#include "include/Threads/qgl_basic_callback_dispatcher_traits.h"

namespace qgl
{
   /*
    These are the parameters passed to a callback dispatcher thread.
    ArgT: Additional parameters that the user defined passed to the dispatcher
    thread.
    */
   template<
      typename CallbackFunctor,
      class ArgT,
      class DispatcherTraits = basic_callback_dispatcher_traits>
   class callback_dispatcher_args final
   {
      public:
      callback_dispatcher_args(DispatcherTraits t = DispatcherTraits{}) :
         m_traits(t)
      {
      }

      callback_dispatcher_args(ArgT& arg,
                               DispatcherTraits t = DispatcherTraits{}) :
         m_traits(t),
         m_args_p(std::make_unique<ArgT>(arg))
      {
      }

      callback_dispatcher_args(const callback_dispatcher_args&) = default;

      callback_dispatcher_args(callback_dispatcher_args&&) noexcept = default;

      ~callback_dispatcher_args() noexcept = default;

      /*
       Begin iterating through the callbacks.
       */
      auto cbegin() const noexcept
      {
         return m_callbacks.cbegin();
      }

      /*
       End iterating through the callbacks.
       */
      auto cend() const noexcept
      {
         return m_callbacks.cend();
      }

      /*
       Locks the set of callbacks so it cannot be modified.
       */
      void lock()
      {
         m_callbacks.lock();
      }

      /*
       Unlocks the set of callbacks so it can be modified.
       */
      void unlock()
      {
         m_callbacks.unlock();
      }

      /*
       Returns true if the dispatcher thread has been requested to terminate.
       */
      bool killed()
      {
         return m_traits.signaled(m_killHandle);
      }

      /*
       Signals to the parent thread that the dispatcher is ready.
       */
      void signal_ready()
      {
         m_traits.signal(m_readyHandle);
      }

      /*
       Returns the handle to the kill event.
       */
      const phandle& kill_handle() const noexcept
      {
         return m_killHandle;
      }

      ArgT& args()
      {
         return *m_args_p;
      }

      const ArgT& args() const
      {
         return *m_args_p;
      }

      /*
       Want this class to access the private members.
       */
      template<typename cbFunctor, class argType, class dispatchtraits>
      friend class callback_dispatcher;

      private:
      /*
       The callbacks to raise from the dispatcher thread.
       */
      ts_uset<CallbackFunctor> m_callbacks;

      /*
       If this handle is signaled, then the dispatcher thread should shut down.
       */
      phandle m_killHandle;

      /*
       The dispatcher thread should use this to signal to the caller thread that
       the dispatcher thread is ready.
       */
      phandle m_readyHandle;

      /*
       Used to signal and wait for events.
       */
      DispatcherTraits m_traits;

      // Wrap the user defined arguments in a pointer so the type does not have
      // to be default constructible. 
      std::unique_ptr<ArgT> m_args_p;
   };
}