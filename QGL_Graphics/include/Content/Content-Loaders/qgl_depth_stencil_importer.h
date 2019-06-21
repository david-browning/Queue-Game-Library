#pragma once
#include "include/qgl_graphics_include.h"
#include "include/Content/Content-Buffers/qgl_depth_stencil_buffer.h"

namespace qgl::content::loaders
{
   using depth_stencil_buffer_importer = struct_importer<
      buffers::DEPTH_STENCIL_BUFFER,
      RESOURCE_TYPE_DEPTH_STENCIL,
      CONTENT_LOADER_ID_DEPTH_STENCIL>;
}