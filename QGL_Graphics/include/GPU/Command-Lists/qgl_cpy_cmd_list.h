#pragma once
#include "include/qgl_graphics_include.h"
#include "include/GPU/Command-Lists/qgl_icmd_list.h"

namespace qgl::graphics::gpu
{
   class copy_command_list : public icommand_list
   {
      public:
      copy_command_list(d3d_device* dev_p,
                            pipeline_state* pipelineState_p,
                            UINT nodeMask = 0);

      copy_command_list(const copy_command_list&) = delete;

      copy_command_list(copy_command_list&&);

      virtual ~copy_command_list();

      /*
       Throws exception.
       */
      virtual void root_sig(root_signature* sig);

      /*
       Throws exception.
       */
      virtual void table(descriptor_table* tbl);

      virtual void begin();

      void end();

      private:
      struct impl;
      impl* m_impl_p;
   };
}