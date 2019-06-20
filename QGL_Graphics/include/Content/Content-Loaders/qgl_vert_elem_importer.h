#pragma once
#include "include/qgl_graphics_include.h"
#include "include/Content/Content-Buffers/qgl_vert_elem_buffer.h"

namespace qgl::content::loaders
{
   using vert_elem_importer = struct_importer<
      buffers::VERTEX_ELEMENT_BUFFER,
      RESOURCE_TYPE_DESCRIPTION,
      CONTENT_LOADER_ID_VERTEX_ELEMENT>;
}