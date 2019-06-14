#pragma once
#include "include/qgl_content_include.h"
#include "include/Interfaces/qgl_icontent_file.h"

namespace qgl::content
{
   class wstring_item;
}

namespace qgl::content::loaders
{
   extern QGL_CONTENT_API std::unique_ptr<wstring_item> qgl_load_wstring_file(
      const icontent_file* f,
      const id_t newID);
}