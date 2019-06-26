#pragma once
#include "include/qgl_content_include.h"
#include "include/Interfaces/qgl_icontent_file.h"

namespace qgl::content
{
   class wstring_item;
}

namespace qgl::content::loaders
{
   class QGL_CONTENT_API wstring_file_loader
   {
      public:
      std::unique_ptr<wstring_item> operator()(const icontent_file* f,
                                               const id_t newID) const;
   };
}