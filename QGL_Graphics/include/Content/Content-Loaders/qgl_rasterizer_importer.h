#pragma once
#include "include/qgl_graphics_include.h"
#include "include/Content/qgl_rasterizer.h"

namespace qgl::content::loaders
{
   using rasterizer_buffer_importer = struct_importer<
      buffers::RASTERIZER_BUFFER,
      RESOURCE_TYPE_RASTERIZER,
      CONTENT_LOADER_ID_RASTERIZER>;
}