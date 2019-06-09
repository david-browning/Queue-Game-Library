#pragma once
#include "include/qgl_content_include.h"
#include "include/Interfaces/qgl_icontent_file.h"

namespace qgl::content
{
   extern QGL_CONTENT_API std::unique_ptr<content_item> qgl_load_string_file(
      const icontent_file* f);
}