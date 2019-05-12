#pragma once
#include "qgl_graphics_include.h"
#include "qgl_depth_stencil_buffer.h"
#include <QGLContent.h>

namespace qgl::content
{
   template<typename ContentCharT = char, typename id_t = content_id>
   using depth_stencil_importer = struct_importer<qgl::graphics::DEPTH_STENCIL_BUFFER,
      CONTENT_LOADER_IDS::CONTENT_LOADER_ID_DEPTH_STENCIL,
      RESOURCE_TYPES::RESOURCE_TYPE_DESCRIPTION,
      ContentCharT, id_t>;
}