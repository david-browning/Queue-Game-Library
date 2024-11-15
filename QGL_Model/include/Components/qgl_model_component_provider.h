#pragma once
#include "include/qgl_model_include.h"
#include "include/Components/qgl_icomponent_provider.h"

namespace qgl::components
{
   /*
    Gets the component provider for this module.
    */
   extern "C" QGL_MODEL_API qgl::result_t QGL_CC qgl_get_components(
      qgl_version v, qgl::components::icomponent_provider **out_p) noexcept;
}