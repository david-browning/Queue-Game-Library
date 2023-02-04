#pragma once
#include "include/qgl_graphics_include.h"
#include "include/GPU/Command-Lists/qgl_icmd_list.h"

namespace qgl::graphics::gpu
{
   class upload_command_list : 
      public icommand_list<D3D12_COMMAND_LIST_TYPE_DIRECT>
   {
      public:
      upload_command_list(graphics_device& dev,
                          gpu::ipso& pso,
                          gpu_idx_t nodeMask = 0,
                          const sys_char* debugName = nullptr) :
         icommand_list(dev, pso, nodeMask, debugName)
      {

      }

      upload_command_list(const upload_command_list&) = delete;

      upload_command_list(upload_command_list&&) = default;

      virtual ~upload_command_list() noexcept = default;

      virtual void begin()
      {
         reset();
      }

      /*
       Throws exception.
       */
      virtual void root_sig(root_signature&)
      {
         throw std::runtime_error(
            "Upload command lists do not support root signatures.");
      }

      /*
       Throws exception.
       */
      virtual void table(descriptor_table&)
      {
         throw std::runtime_error(
            "Upload command lists do not support descriptor tables.");
      }

      void end()
      {
         check_result(get()->Close());
      }
   };
}