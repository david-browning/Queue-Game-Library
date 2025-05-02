#pragma once
#include "include/qgl_model_include.h"
#include "include/qgl_guid.h"
#include "include/Commands/qgl_icommand.h"

namespace qgl
{
   /*
    Stores a command and the time (Total elapsed ticks) the command was executed.
    */
   template<typename TickT>
   struct command_frame final
   {
      constexpr command_frame()
      {

      }

      constexpr command_frame(const guid& commandId, TickT elapsedTime) :
         timestamp(elapsedTime), id(commandId)
      {

      }

      constexpr command_frame(const command_frame&) = default;

      constexpr command_frame(command_frame&&) noexcept = default;

      friend void swap(command_frame& l, command_frame& r) noexcept
      {
         using std::swap;
         swap(l.timestamp, r.timestamp);
         swap(l.id, r.id);
      }

      command_frame& operator=(command_frame r) noexcept
      {
         swap(*this, r);
         return *this;
      }

      TickT timestamp = 0;
      guid id;
   };
}