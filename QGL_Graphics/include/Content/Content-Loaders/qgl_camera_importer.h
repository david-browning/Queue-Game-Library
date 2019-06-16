#pragma once
#include "include/qgl_graphics_include.h"
#include "include/Content/Content-Buffers/qgl_camera_buffer.h"

namespace qgl::content::loaders
{
   using camera_buffer_importer = struct_importer<
      buffers::CAMERA_BUFFER,
      RESOURCE_TYPE_CAMERA,
      CONTENT_LOADER_ID_CAMERA>;
}