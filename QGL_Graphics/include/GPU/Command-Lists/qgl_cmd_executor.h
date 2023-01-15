#pragma once
#include "include/qgl_graphics_include.h"
#include "include/GPU/Command-Lists/qgl_icmd_list.h"

namespace qgl::graphics::gpu
{
   /*
    Used to queue up and execute command lists.
    Queuing and clearing queued command lists are thread-safe.
    */
   class cmd_executor final
   {
      public:
      /*
       Constructor. Holds a weak reference to the given command queue. Do not
       free the command queue without destroying this first.
       */
      cmd_executor(icmd_queue* cmdQueue_p) :
         m_queue(cmdQueue_p)
      {

      }

      cmd_executor(const cmd_executor&) = delete;

      /*
       Default is OK because this does not own the pointers.
       */
      cmd_executor(cmd_executor&&) noexcept = default;

      /*
       Default is OK because this does not own the pointers.
       */
      ~cmd_executor() noexcept = default;

      /*
       Clears the queue of command lists. The queue is not cleared once 
       "execute" is called.
       */
      void clear() noexcept
      {
         m_lists_p.clear();
      }

      /*
       Adds the command list to the execution queue.
       */
      void queue(icommand_list* cmdList_p) noexcept
      {
         m_lists_p.push_back(cmdList_p->get());
      }
      
      /*
       Adds the command list to the execution queue.
       */
      void queue(icommand_list& cmdList) noexcept
      {
         m_lists_p.push_back(cmdList.get());
      }

      /*
       Adds the range of command lists to the queue. When dereferenced, first 
       must point to some form of icommand_list or icmd_list;
       */
      template<class CmdListForwardIt>
      void queue(CmdListForwardIt first, CmdListForwardIt last) noexcept
      {
         for (; first != last; last++)
         {
            m_lists_p.push_back(get_cmdp(*first));
         }
      }

      /*
       Adds numLists command lists to the queue.
       */
      void queue(icommand_list** lists, size_t numLists) noexcept
      {
         for (size_t i = 0; i < numLists; i++)
         {
            m_lists_p.push_back(lists[i]->get());
         }
      }

      /*
       Executes all queued command lists.
       */
      void execute() noexcept
      {
         // ExecuteCommandLists doesn't accept classes that derive from 
         // ID3D12CommandList. The cast tells the ExecuteCommandLists call that
         // the list of icmd_list pointers is a list of ID3D12CommandList*
         m_queue->ExecuteCommandLists(
            static_cast<UINT>(m_lists_p.size()), 
            (ID3D12CommandList**)m_lists_p.data());
      }

      private:
      icmd_list* get_cmdp(icommand_list* p) noexcept
      {
         return p->get();
      }

      icmd_list* get_cmdp(icommand_list& p) noexcept
      {
         return p.get();
      }

      icmd_list* get_cmdp(
         const std::reference_wrapper<icommand_list>& p) noexcept
      {
         return p.get().get();
      }

      icmd_list* get_cmdp(
         const std::unique_ptr<icommand_list>& p) noexcept
      {
         return p->get();
      }

      icmd_list* get_cmdp(
         const std::shared_ptr<icommand_list>& p) noexcept
      {
         return p->get();
      }

      icmd_list* get_cmdp(icmd_list* p) noexcept
      {
         return p;
      }

      icmd_list* get_cmdp(icmd_list& p) noexcept
      {
         return std::addressof(p);
      }

      icmd_list* get_cmdp(std::reference_wrapper<icmd_list>& p) noexcept
      {
         return std::addressof(p.get());
      }

      /*
       The command lists to execute.
       */
      ts_vector<icmd_list*> m_lists_p;

      /* 
       The command queue used to execute the lists.
       */
      icmd_queue* m_queue;
   };
}