#pragma once
#include "include/qgl_content_include.h"
#include "include/Content-Files/qgl_content_file.h"

namespace qgl::content
{
   struct LIB_EXPORT wstring_file_loader
   {
      std::shared_ptr<std::wstring> operator()(const content_file_read& f);
   };
}