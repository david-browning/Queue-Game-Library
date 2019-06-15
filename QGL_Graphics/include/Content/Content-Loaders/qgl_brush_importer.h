#pragma once
#include "include/qgl_graphics_include.h"
#include "include/Content/Content-Buffers/qgl_brush_buffer.h"

namespace qgl::content::loaders
{
   using brush_buffer_importer = struct_importer<
      buffers::BRUSH_BUFFER,
      RESOURCE_TYPE_BRUSH,
      CONTENT_LOADER_ID_STRUCT>;
}