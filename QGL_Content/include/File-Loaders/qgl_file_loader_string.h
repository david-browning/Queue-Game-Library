#pragma once
#include "include/qgl_content_include.h"
#include "include/Interfaces/qgl_icontent_file.h"

namespace qgl::content
{
   struct QGL_CONTENT_API string_file_loader
   {
      std::shared_ptr<std::string> operator()(const icontent_file* f);
   };
}