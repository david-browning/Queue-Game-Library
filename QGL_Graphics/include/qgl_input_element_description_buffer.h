#pragma once
#include "qgl_graphics_include.h"

namespace qgl::graphics
{
   /*
    Describes the layout for a vertex.
    */
   struct LIB_EXPORT INPUT_ELEMENT_DESC_BUFFER
   {
      public:
      INPUT_ELEMENT_DESC_BUFFER();

      INPUT_ELEMENT_DESC_BUFFER(const INPUT_ELEMENT_DESC_BUFFER& r);

      INPUT_ELEMENT_DESC_BUFFER(INPUT_ELEMENT_DESC_BUFFER&& r);


   };
}