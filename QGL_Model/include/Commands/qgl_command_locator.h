#pragma once
#include "include/qgl_model_include.h"
#include "include/qgl_guid.h"
#include "include/Commands/qgl_icommand.h"

namespace qgl
{
   template<typename ExecuteArgs>
   class command_locator final
   {
      public:
      command_locator()
      {

      }

      command_locator(const command_locator& r) = default;

      command_locator(command_locator&&) = default;

      ~command_locator() noexcept = default;

      friend void swap(command_locator& l, command_locator& r) noexcept
      {
         using std::swap;
         swp(l.m_cmds, r.m_cmds);
      }

      command_locator& operator=(command_locator r) noexcept
      {
         swap(*this, r);
         return *this;
      }

      void add(const icommand<ExecuteArgs>* cmd_p)
      {
         m_cmds[cmd_p->guid()] = cmd_p;
      }

      bool contains(const guid& id) const noexcept
      {
         return m_cmds.count(id) > 0;
      }

      void erase(const guid& id)
      {
         m_cmds.erase(id);
      }

      const icommand<ExecuteArgs>* get(const guid& id) const
      {
         return m_cmds.at(id);
      }

      void clear()
      {
         m_cmds.clear();
      }

      private:
      std::unordered_map<guid, const icommand<ExecuteArgs>*> m_cmds;
   };
}