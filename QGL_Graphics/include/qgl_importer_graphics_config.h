#pragma once
#include "qgl_graphics_include.h"
#include "qgl_graphic_config_buffer.h"
#include <QGLContent.h>

namespace qgl::content
{
   template<typename ContentCharT = char, typename id_t = content_id>
   using graphics_config_importer = struct_importer<qgl::graphics::GRAPHICS_CONFIG_BUFFER,
      CONTENT_LOADER_IDS::CONTENT_LOADER_ID_GRAPHICS_CONFIG,
      RESOURCE_TYPES::RESOURCE_TYPE_DESCRIPTION,
      ContentCharT, id_t>;
}