#pragma once
#include "qgl_graphics_include.h"
#include "qgl_sampler_buffer.h"
#include <QGLContent.h>

namespace qgl::content
{
   template<typename ContentCharT = char, typename IDT = content_id>
   using sampler_importer = struct_importer<qgl::graphics::SAMPLER_BUFFER,
      CONTENT_LOADER_IDS::CONTENT_LOADER_ID_SAMPLER,
      RESOURCE_TYPES::RESOURCE_TYPE_DESCRIPTION,
      ContentCharT, IDT>;
}