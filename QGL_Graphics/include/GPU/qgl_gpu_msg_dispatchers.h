#pragma once
#include "include/qgl_graphics_include.h"
#include "include/Helpers/qgl_graphics_device_helpers.h"

namespace qgl::graphics
{
   typedef void (*gpu_budget_changed_callback)(const gpu_mem_info&);
   typedef void (*gpu_msg_callback)(const std::string&);
}

namespace qgl::graphics
{
   struct gpu_budget_changed_callback_args
   {
      gpu_budget_changed_callback_args(igpu_adapter* adptr_p, gpu_idx_t gpuIdx)
      {
         adapter_p = adptr_p;
         adapterIdx = gpuIdx;
         budgetHandle = make_waitable();

         // Register the budget changed callback.
         check_result(
            adapter_p->RegisterVideoMemoryBudgetChangeNotificationEvent(
               budgetHandle.get(), &revokeToken));
      }

      gpu_budget_changed_callback_args(
         const gpu_budget_changed_callback_args& r)
      {
         adapterIdx = r.adapterIdx;
         adapter_p = r.adapter_p;
         budgetHandle = make_waitable();

         // Register the budget changed callback.
         check_result(
            adapter_p->RegisterVideoMemoryBudgetChangeNotificationEvent(
               budgetHandle.get(), &revokeToken));

      }

      gpu_budget_changed_callback_args(
         gpu_budget_changed_callback_args&& r) noexcept :
         budgetHandle(std::move(r.budgetHandle))
      {
         adapterIdx = r.adapterIdx;
         adapter_p = r.adapter_p;
         revokeToken = r.revokeToken;

         r.adapter_p = nullptr;
      }

      ~gpu_budget_changed_callback_args() noexcept
      {
         // Only revoke 
         if (revokeToken > 0 && budgetHandle)
         {

         }
      }

      friend void swap(
         gpu_budget_changed_callback_args& l,
         gpu_budget_changed_callback_args& r) noexcept
      {
         using std::swap;
         swap(l.adapterIdx, r.adapterIdx);
         swap(l.adapter_p, r.adapter_p);
         swap(l.budgetHandle, r.budgetHandle);
         swap(l.revokeToken, r.revokeToken);
      }

      gpu_budget_changed_callback_args& operator=(
         gpu_budget_changed_callback_args r) noexcept
      {
         swap(*this, r);
         return *this;
      }

      gpu_idx_t adapterIdx;
      igpu_adapter* adapter_p;
      phandle budgetHandle;
      DWORD revokeToken = 0;
   };
}

namespace qgl::graphics::impl
{
   inline DWORD WINAPI gpu_budget_changed_dispatcher(LPVOID param_p)
   {
      auto p = reinterpret_cast<callback_dispatcher_args<
         gpu_budget_changed_callback,
         gpu_budget_changed_callback_args,
         basic_callback_dispatcher_traits>*>(param_p);

      HANDLE handles[] = { p->kill_handle().get(), p->args().budgetHandle.get() };
      bool kill = false;
      p->signal_ready();

      while (!kill)
      {
         auto waitResult = WaitForMultipleObjects(2, handles, false, INFINITE);
         if (waitResult == WAIT_FAILED)
         {
            winrt::throw_last_error();
         }
         else if (waitResult == WAIT_OBJECT_0)
         {
            // The kill handle signaled.
            kill = true;
         }
         else
         {
            gpu_mem_info info;
            check_result(helpers::app_mem(
               p->args().adapter_p, p->args().adapterIdx, &info));

            // Lock the set so the callbacks cannot be altered while 
            // iterating.
            p->lock();
            // Fire off the callbacks.
            for (auto it = p->cbegin(); it != p->cend(); it++)
            {
               std::invoke(*it, info);
            }
            p->unlock();

            ResetEvent(p->args().budgetHandle.get());
         }
      }

      // Unregister the wait handle.
      p->args().adapter_p->UnregisterVideoMemoryBudgetChangeNotification(
         p->args().revokeToken);
      p->args().budgetHandle.close();

      return 0;
   }

   inline DWORD WINAPI gpu_msg_dispatcher(LPVOID param_p)
   {
      auto p = reinterpret_cast<callback_dispatcher_args<
         gpu_msg_callback,
         ID3D12InfoQueue*,
         basic_callback_dispatcher_traits>*>(param_p);

      auto queue_p = p->args();
      p->signal_ready();
      while (!p->killed())
      {
         auto msgs = queue_p->GetNumStoredMessages();
         if (msgs > 0)
         {
            for (uint64_t i = 0; i < msgs; i++)
            {
               size_t msgLen = 0;

               // TODO: It might be better to swallow these exceptions.

               // Get how many bytes are needed for the message.
               check_result(queue_p->GetMessage(i, nullptr, &msgLen));

               // Alloc a buffer for the number of bytes.
               std::vector<std::byte> msgData{ msgLen, std::byte{0} };

               // Get the message and put it in the buffer.
               check_result(queue_p->GetMessage(
                  i, reinterpret_cast<D3D12_MESSAGE*>(msgData.data()), &msgLen));

               auto msg_p = reinterpret_cast<D3D12_MESSAGE*>(msgData.data());
               std::stringstream msgStream;
               msgStream << "Cat: " << msg_p->Category <<
                  " Sev: " << msg_p->Severity <<
                  " ID: " << msg_p->ID << ": " << msg_p->pDescription <<
                  std::endl;
               auto msgStr = msgStream.str();

               // Lock the set so the callbacks cannot be altered while 
               // iterating.
               p->lock();
               for (auto it = p->cbegin(); it != p->cend(); it++)
               {
                  std::invoke(*it, msgStr);
               }
               p->unlock();
            }

            queue_p->ClearStoredMessages();
         }
      }

      return 0;
   }
}
