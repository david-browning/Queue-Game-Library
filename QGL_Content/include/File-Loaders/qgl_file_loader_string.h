#pragma once
#include "include/qgl_content_include.h"
#include "include/Content-Files/qgl_content_file.h"

namespace qgl::content
{
   struct QGL_CONTENT_API string_file_loader
   {
      std::shared_ptr<std::string> operator()(const content_file& f);
   };
}