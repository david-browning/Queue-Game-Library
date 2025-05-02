#pragma once
#include "include/qgl_model_include.h"
#include "include/Threads/qgl_callback_dispatcher_args.h"

namespace qgl
{
   /*
      A template-based asynchronous dispatcher that runs callbacks on a background
      thread. The dispatcher allows callbacks to be registered and revoked, and
      uses a kill-and-wait mechanism to control the lifecycle of the thread.

      Key Features:
      - Supports dynamic registration and revocation of callbacks of type CallbackFunctor.
      - Launches a dispatcher thread that receives arguments of type ArgT.
      - Dispatcher lifecycle is managed via waitable synchronization handles.
      - DispatcherTraits provides policy/customization points for thread creation, waiting,
        signaling, and handle management.

      Usage:
      - Call `start()` to begin dispatching on a background thread.
      - Call `kill()` to terminate the dispatcher cleanly.
      - Register or revoke callbacks while the dispatcher is running or idle.
      - Pass arguments into the dispatcher using `start(args)` if the thread needs context.

      Thread entry logic is defined externally via a DispatcherTraits::thread_entry
      function, which should loop, dispatch callbacks, and observe kill signals.

      Example Use Case:
      - A background event listener or input polling system that notifies registered
        callbacks when data becomes available.
      - A queue processor that runs jobs off a shared queue in a dedicated thread.
   */
   template<
      typename CallbackFunctor, 
      class ArgT, 
      class DispatcherTraits = basic_callback_dispatcher_traits>
   class callback_dispatcher final
   {
      public:
      using thread_entry = typename DispatcherTraits::thread_entry;

      callback_dispatcher(thread_entry dispatcher,
                          DispatcherTraits traits = DispatcherTraits{}) :
         m_entry(dispatcher),
         m_args(traits)
      {

      }

      callback_dispatcher(thread_entry dispatcher,
                          std::initializer_list<CallbackFunctor> functors,
                          DispatcherTraits traits = DispatcherTraits{}) :
         m_entry(dispatcher),
         m_args(traits)
      {
         for (auto f : functors)
         {
            m_args.m_callbacks.insert(f);
         }
      }

      /*
       Do not allow multiple classes to manage the same dispatcher.
       */
      callback_dispatcher(const callback_dispatcher&) = delete;

      /*
       Move constructor.
       */
      callback_dispatcher(callback_dispatcher&& r) noexcept :
         m_entry(r.m_entry),
         m_args(std::move(r.m_args)),
         m_dispatchHandle(std::move(r.m_dispatchHandle))
      {
         r.m_dispatchHandle.detach();
         r.m_entry = nullptr;
      }

      /*
       Sends the kill message to the dispatcher and waits for termination.
       */
      ~callback_dispatcher() noexcept
      {
         kill();
      }

      void register_callback(CallbackFunctor f)
      {
         m_args.m_callbacks.insert(f);
      }

      void revoke_callback(CallbackFunctor f) noexcept
      {
         m_args.m_callbacks.erase(f);
      }

      /*
       Starts the dispatcher on a background thread.
       */
      void start()
      {
         if (running())
         {
            return;
         }

         m_args.m_killHandle = make_waitable();
         m_args.m_readyHandle = make_waitable();

         // Start the thread.
         m_dispatchHandle = traits().invoke_thread(m_entry, &m_args);

         // Wait for the thread to say it's ready.
         traits().wait(m_args.m_readyHandle);
      }

      /*
       Starts the dispatcher on a background thread with the given args.
       */
      void start(const ArgT& args)
      {
         m_args.m_args_p.release();
         m_args.m_args_p = std::make_unique<ArgT>(args);
         start();
      }

      /*
       Sends the "kill" message to the background dispatcher and waits for it
       to finish.
       */
      void kill()
      {
         if (!running())
         {
            return;
         }

         // Tell the dispatcher to kill.
         traits().signal(m_args.m_killHandle);

         // Wait for the dispatcher thread to finish.
         traits().wait(m_dispatchHandle);
         m_dispatchHandle.close();
      }

      /*
       Returns true if the dispatcher thread is running.
       */
      bool running() const noexcept
      {
         // Running if the ready event was triggered but the killed event was 
         // not.
         if (!m_args.m_readyHandle || !m_args.m_killHandle)
         {
            return false;
         }

         return traits().signaled(m_args.m_readyHandle) &&
            !traits().signaled(m_args.m_killHandle);
      }

      private:
      DispatcherTraits& traits() noexcept
      {
         return m_args.m_traits;
      }

      const DispatcherTraits& traits() const noexcept
      {
         return m_args.m_traits;
      }

      /*
       Entry point for the dispatcher thread.
       */
      thread_entry m_entry;

      /*
       Handle to the dispatcher thread.
       */
      phandle m_dispatchHandle;

      /*
       This is the data that will be passed to the callback dispatcher thread.
       */
      callback_dispatcher_args<CallbackFunctor, ArgT, DispatcherTraits> m_args;
   };
}