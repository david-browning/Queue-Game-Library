#pragma once
#include "include/qgl_graphics_include.h"
#include "include/GPU/qgl_graphics_device.h"

namespace qgl::graphics::engine
{
   template<D3D12_COMMAND_LIST_TYPE listType>
   class icommand_list
   {
      icommand_list(const graphics_device* gdev);

      icommand_list(const icommand_list&) = delete;

      icommand_list(icommand_list&&);

      virtual ~icommand_list();
   };
}