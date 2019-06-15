#pragma once
#include "include/qgl_graphics_include.h"
#include "include/Content/Content-Buffers/qgl_sampler_buffer.h"

namespace qgl::content::loaders
{
   using sampler_buffer_importer = struct_importer<
      buffers::SAMPLER_BUFFER,
      RESOURCE_TYPE_SAMPLER,
      CONTENT_LOADER_ID_STRUCT>;
}