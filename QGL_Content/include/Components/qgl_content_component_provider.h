#pragma once
#include "include/qgl_content_include.h"

namespace qgl
{
   /*
    Gets the component provider for this module.
    */
   extern "C" QGL_CONTENT_API qgl::result_t QGL_CC qgl_get_components(
      qgl_version v, qgl::components::icomponent_provider **out_p) noexcept;
}