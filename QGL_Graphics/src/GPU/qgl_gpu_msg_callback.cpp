#include "pch.h"
#include "include/GPU/qgl_gpu_msg_callback.h"

namespace qgl::graphics::impl
{
   DWORD WINAPI msg_callback(LPVOID param_p)
   {
      auto p = reinterpret_cast<gpu_msg_callback_params*>(param_p);

      // Tell the spawning thread that this thread has been started.      
      SetEvent(p->signal.get());

      auto queue = p->infoQueue_p;

      while (!p->willTerminate.load())
      {
         auto msgs = queue->GetNumStoredMessages();
         if (msgs > 0)
         {
            for (uint64_t i = 0; i < msgs; i++)
            {
               size_t msgLen = 0;

               // TODO: It might be better to swallow these exceptions.

               // Get how many bytes are needed for the message.
               winrt::check_hresult(queue->GetMessage(i, nullptr, &msgLen));

               // Alloc a buffer for the number of bytes.
               std::vector<std::byte> msgData{ msgLen, std::byte{0} };

               // Get the message and put it in the buffer.
               winrt::check_hresult(queue->GetMessage(
                  i, reinterpret_cast<D3D12_MESSAGE*>(msgData.data()), &msgLen));

               auto msg_p = reinterpret_cast<D3D12_MESSAGE*>(msgData.data());
               std::stringstream msgStream;
               msgStream << "Cat: " << msg_p->Category <<
                  " Sev: " << msg_p->Severity <<
                  " ID: " << msg_p->ID << ": " << msg_p->pDescription << 
                  std::endl;
               auto msgStr = msgStream.str();

               for (auto& con : p->consoles)
               {
                  con->cout(msgStr);
               }
            }

            queue->ClearStoredMessages();
         }
      }

      return gpu_msg_callback_handler::THREAD_COMPLETED_CODE;
   }
}