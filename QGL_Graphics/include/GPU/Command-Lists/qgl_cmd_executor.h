#pragma once
#include "include/qgl_graphics_include.h"
#include "include/qgl_graphics_device.h"
#include "include/GPU/Command-Lists/qgl_icmd_list.h"

namespace qgl::graphics::gpu
{
   /*
    Used to queue up and execute command lists.
    Queuing and clearing queued command lists are thread-safe.
    */
   template<D3D12_COMMAND_LIST_TYPE ListT, class SRWTraits = qgl::srw_traits>
   class cmd_executor final
   {
      public:
      /*
       Constructor. Holds a weak reference to the given command queue. Do not
       free the command queue without destroying this first.
       */
      cmd_executor(icmd_queue* q_p)
      {
         m_rawQueue_p = q_p;
      }

      cmd_executor(graphics_device& dev_p,
                   const D3D12_COMMAND_QUEUE_DESC& desc)
      {
         m_queue_p = helpers::make_cmd_queue(desc, dev_p.dev_3d());
      }

      cmd_executor(graphics_device& dev_p,
                   D3D12_COMMAND_LIST_TYPE type)
      {
         D3D12_COMMAND_QUEUE_DESC desc = {};
         desc.Type = type;
         m_queue_p = helpers::make_cmd_queue(desc, dev_p.dev_3d());
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
      void queue(icommand_list<ListT>* cmdList_p) noexcept
      {
         m_lists_p.push_back(cmdList_p->get());
      }
      
      /*
       Adds the command list to the execution queue.
       */
      void queue(icommand_list<ListT>& cmdList) noexcept
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
      void queue(icommand_list<ListT>** lists, size_t numLists) noexcept
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
         get()->ExecuteCommandLists(
            static_cast<UINT>(m_lists_p.size()), 
            (ID3D12CommandList**)m_lists_p.data());
      }

      const icmd_queue* get() const noexcept
      {
         if (m_queue_p.has_value())
         {
            return m_queue_p.value().get();
         }
         else
         {
            return m_rawQueue_p.value();
         }
      }

      icmd_queue* get() noexcept
      {
         if (m_queue_p.has_value())
         {
            return m_queue_p.value().get();
         }
         else
         {
            return m_rawQueue_p.value();
         }
      }

      private:
      icmd_list* get_cmdp(icommand_list<ListT>* p) noexcept
      {
         return p->get();
      }

      icmd_list* get_cmdp(icommand_list<ListT>& p) noexcept
      {
         return p.get();
      }

      icmd_list* get_cmdp(
         const std::reference_wrapper<icommand_list<ListT>>& p) noexcept
      {
         return p.get().get();
      }

      icmd_list* get_cmdp(
         const std::unique_ptr<icommand_list<ListT>>& p) noexcept
      {
         return p->get();
      }

      icmd_list* get_cmdp(
         const std::shared_ptr<icommand_list<ListT>>& p) noexcept
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
      slim_vector<icmd_list*, SRWTraits> m_lists_p;

      std::optional<pptr<icmd_queue>> m_queue_p;
      std::optional<icmd_queue*> m_rawQueue_p;
   };
}