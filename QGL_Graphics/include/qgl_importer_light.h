#pragma once
#include "qgl_graphics_include.h"
#include "qgl_light_buffer.h"
#include <QGLContent.h>

namespace qgl::content
{
   template<typename ContentCharT = char, typename IDT = content_id>
   using light_importer = struct_importer<qgl::graphics::LIGHT_BUFFER,
      CONTENT_LOADER_IDS::CONTENT_LOADER_ID_LIGHT,
      RESOURCE_TYPES::RESOURCE_TYPE_LIGHT,
      ContentCharT, IDT>;
}