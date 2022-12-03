#pragma once
#include "include/qgl_graphics_include.h"
#include "include/GPU/Command-Lists/qgl_icmd_list.h"

namespace qgl::graphics::gpu
{
   class copy_command_list : public icommand_list
   {
      public:
      copy_command_list(graphics_device& dev,
                        gpu::ipso& pso,
                        size_t nodeMask = 0) :
         icommand_list(dev, pso, D3D12_COMMAND_LIST_TYPE_COPY, nodeMask)
      {
      }

      copy_command_list(const copy_command_list&) = delete;

      copy_command_list(copy_command_list&&) = default;

      virtual ~copy_command_list() noexcept = default;

      virtual void begin()
      {
         reset();
      }

      /*
       Throws exception.
       */
      virtual void root_sig(root_signature& sig)
      {
         throw std::runtime_error(
            "Copy command lists do not support root signatures.");
      }

      /*
       Throws exception.
       */
      virtual void table(descriptor_table& tbl)
      {
         throw std::runtime_error(
            "Copy command lists do not support descriptor tables.");
      }

      void end()
      {

      }

      private:
   };
}