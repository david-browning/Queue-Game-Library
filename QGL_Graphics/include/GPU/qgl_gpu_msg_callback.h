#pragma once
#include "include/qgl_graphics_include.h"

namespace qgl::graphics::impl
{
   struct gpu_msg_callback_params final
   {
      gpu_msg_callback_params() :
         willTerminate(false),
         infoQueue_p(nullptr)
      {

      }

      gpu_msg_callback_params(const gpu_msg_callback_params&) = default;

      gpu_msg_callback_params(gpu_msg_callback_params&& x) noexcept :
         consoles(std::move(x.consoles)),
         infoQueue_p(std::move(x.infoQueue_p)),
         willTerminate(x.willTerminate.load()),
         signal(std::move(x.signal))
      {
         // Signal is detached by it's move constructor.

         x.infoQueue_p = nullptr;
      }

      /*
       Consoles that messages will be outputed to.
       */
      std::list<console_ptr> consoles;

      /*
       Used to terminate the thread.
       */
      std::atomic_bool willTerminate;

      /*
       Raw pointer to the info queue which is used to get GPU messages.
       */
      ID3D12InfoQueue* infoQueue_p;

      winrt::handle signal;
   };

   extern "C" DWORD QGL_GRAPHICS_API msg_callback(LPVOID param_p);
}

namespace qgl::graphics
{
   class  gpu_msg_callback_handler final
   {
      public:
      using thread_code = typename DWORD;
      static constexpr thread_code THREAD_NOT_RUNNING_CODE = 0xAAAAAAAA;
      static constexpr thread_code THREAD_RUNNING_CODE = 0xBBBBBBBB;
      static constexpr thread_code THREAD_KILLED_EXIT_CODE = 0xCCCCCCCC;
      static constexpr thread_code THREAD_COMPLETED_CODE = 0xCCCCCCCC;

      gpu_msg_callback_handler() :
         m_tCode(THREAD_NOT_RUNNING_CODE)
      {
      }

      gpu_msg_callback_handler(gpu_msg_callback_handler&& x) noexcept :
         m_tHandle(std::move(x.m_tHandle)),
         m_callbackParams(std::move(x.m_callbackParams)),
         m_tCode(x.m_tCode)
      {
         // Handle move takes care of detachment. 

         // Tell the other object that the thread closed so it won't try to 
         // stop it when destroyed.
         x.m_tCode = THREAD_KILLED_EXIT_CODE;
      }

      ~gpu_msg_callback_handler() noexcept
      {
         // stop checks if the thread is running.
         stop();
      }

      /*
       Starts a background thread which listens for GPU messages.
       */
      void run(
         ID3D12InfoQueue* param,
         std::initializer_list<console_ptr> cons)
      {
         // If the thread is already running
         if (running())
         {
            // Do nothing.
            return;
         }

         // Set up the parameter that will be passed to the thread.
         m_callbackParams.infoQueue_p = param;
         m_callbackParams.willTerminate.store(false);
         for (const auto& c : cons)
         {
            m_callbackParams.consoles.push_back(c);
         }

         // Create a signal that will be used by the message thread to indicate 
         // that it is running.
         auto signalHndl = CreateEvent(nullptr, true, false, nullptr);
         if (!signalHndl)
         {
            winrt::throw_last_error();
         }

         // Attach the signal to the thread parameters.
         m_callbackParams.signal.attach(signalHndl);

         // https://learn.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-createthread
         // A thread in an executable that calls the C run-time library (CRT)
         // should use the _beginthreadex and _endthreadex functions for thread 
         // management rather than CreateThread and ExitThread; this requires 
         // the use of the multithreaded version of the CRT. If a thread 
         // created using CreateThread calls the CRT, the CRT may terminate the 
         // process in low-memory conditions.
         // https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/beginthread-beginthreadex?view=msvc-170

         // Create thread so it runs immediately
         auto tHandle = CreateThread(
            nullptr, 0, qgl::graphics::impl::msg_callback, &m_callbackParams, 0, nullptr);
         if (!tHandle)
         {
            winrt::throw_last_error();
         }

         // Attach to the RAII object.
         m_tHandle.attach(tHandle);
         m_tCode = THREAD_RUNNING_CODE;

         // Wait for the new thread before returning. This way we cannot close
         // the new thread before it has started.
         auto waitResult = WaitForSingleObject(
            m_callbackParams.signal.get(), INFINITE);
         if (waitResult == WAIT_FAILED)
         {
            winrt::throw_last_error();
         }
      }

      /*
       Signals to the background thread that it should stop checking for GPU
       messages. 
       */
      void stop() noexcept
      {
         if (running())
         {
            // Signal to the thread that it should stop.
            m_callbackParams.willTerminate.store(true);
         }
      }

      bool running() const noexcept
      {
         return m_tCode == THREAD_RUNNING_CODE;
      }

      thread_code exit_code() const noexcept
      {
         return m_tCode;
      }

      private:
      impl::gpu_msg_callback_params m_callbackParams;
      winrt::handle m_tHandle;
      thread_code m_tCode;
   };
}
