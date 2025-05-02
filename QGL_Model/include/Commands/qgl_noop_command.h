#pragma once
#include "include/qgl_model_include.h"
#include "include/qgl_guid.h"
#include "include/Commands/qgl_icommand.h"

namespace qgl
{
   /*
    A command that does nothing. The GUID is all 0s.
    */
   template<typename ExecuteArgs>
   class noop_command : public icommand<ExecuteArgs>
   {
      public:
      noop_command() :
         icommand(guid::null_guid())
      {

      }

      virtual ~noop_command() noexcept = default;
      

      virtual void execute(ExecuteArgs&) override
      {

      }

      virtual void undo() override
      {

      }
   };
}