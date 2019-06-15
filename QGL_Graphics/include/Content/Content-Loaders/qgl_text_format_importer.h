#pragma once
#include "include/qgl_graphics_include.h"
#include "include/Content/Content-Buffers/qgl_text_format_buffer.h"

namespace qgl::content::loaders
{
   using text_format_buffer_importer = struct_importer<
      buffers::TEXT_FORMAT_BUFFER,
      RESOURCE_TYPE_TEXT_FORMAT,
      CONTENT_LOADER_ID_STRUCT>;
}