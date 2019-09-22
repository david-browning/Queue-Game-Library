#pragma once
#include "include/qgl_model_include.h"
#include "include/String/qgl_strings.h"

namespace qgl
{
   /*
    Writes the null-terminated string to the debug console.
    */
   extern QGL_MODEL_API void debug_message(const sys_char* const msg) noexcept;
}