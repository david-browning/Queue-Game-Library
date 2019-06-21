#pragma once
#include "include/qgl_graphics_include.h"
#include "include/Content/Content-Buffers/qgl_light_buffer.h"

namespace qgl::content::loaders
{
   using light_buffer_importer = struct_importer<
      buffers::LIGHT_BUFFER,
      RESOURCE_TYPE_LIGHT,
      CONTENT_LOADER_ID_LIGHT>;
}