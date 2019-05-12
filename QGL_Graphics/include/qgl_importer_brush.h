#pragma once
#include "qgl_graphics_include.h"
#include "qgl_brush_buffer.h"
#include <QGLContent.h>

namespace qgl::content
{
   template<typename ContentCharT = char, typename id_t = content_id>
   using brush_importer = struct_importer<qgl::graphics::BRUSH_BUFFER,
      CONTENT_LOADER_IDS::CONTENT_LOADER_ID_BRUSH,
      RESOURCE_TYPES::RESOURCE_TYPE_BRUSH,
      ContentCharT, id_t>;
}