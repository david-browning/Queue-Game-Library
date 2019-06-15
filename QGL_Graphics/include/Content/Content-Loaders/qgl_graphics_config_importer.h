#pragma once
#include "include/qgl_graphics_include.h"
#include "include/Content/Content-Buffers/qgl_graphic_config_buffer.h"

namespace qgl::content::loaders
{
   using graphics_config_buffer_importer = struct_importer<
      buffers::GRAPHICS_CONFIG_BUFFER,
      RESOURCE_TYPE_GRAPHICS_CONFIG,
      CONTENT_LOADER_ID_STRUCT>;
}