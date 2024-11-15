#pragma once
#include "include/qgl_content_include.h"
#include "include/Loaders/qgl_iloader_provider.h"

namespace qgl
{
   /*
    Gets the loader provider for this module.
    */
   extern "C" QGL_CONTENT_API qgl::result_t QGL_CC qgl_get_loaders(
      qgl_version v, qgl::content::iloader_provider **out_p) noexcept;
}