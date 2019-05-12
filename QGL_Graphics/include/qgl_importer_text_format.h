#pragma once
#include "qgl_graphics_include.h"
#include "qgl_text_format_buffer.h"
#include <QGLContent.h>

namespace qgl::content
{
   template<typename ContentCharT = char, typename id_t = content_id>
   using text_format_importer = struct_importer<qgl::graphics::TEXT_FORMAT_BUFFER,
      CONTENT_LOADER_IDS::CONTENT_LOADER_ID_TEXT_FORMAT,
      RESOURCE_TYPES::RESOURCE_TYPE_TEXT_FORMAT,
      ContentCharT, id_t>;
}