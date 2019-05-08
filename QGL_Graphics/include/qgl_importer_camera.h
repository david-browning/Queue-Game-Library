#pragma once
#include "qgl_graphics_include.h"
#include "qgl_camera_buffer.h"
#include <QGLContent.h>

namespace qgl::content
{
   template<typename ContentCharT = char, typename IDT = content_id>
   using camera_importer = struct_importer<qgl::graphics::CAMERA_BUFFER,
      CONTENT_LOADER_IDS::CONTENT_LOADER_ID_CAMERA,
      RESOURCE_TYPES::RESOURCE_TYPE_CAMERA,
      ContentCharT, IDT>;
}