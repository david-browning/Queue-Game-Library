#pragma once
#include "include/qgl_graphics_include.h"

namespace qgl::content
{
   class text_format;
}

namespace qgl::content::loaders
{
   class QGL_GRAPHICS_API text_format_file_loader
   {
      public:
      text_format_file_loader(IDWriteFactory* factory_p);

      std::unique_ptr<text_format> operator()(const icontent_file* f,
                                              const id_t newID);

      private:
      IDWriteFactory* m_factory_p;
   };
}