#include "pch.h"
#include "include/Content-Buffers/qgl_rectangle_buffer.h"

namespace qgl::content::buffers
{
   RECTANGLE_BUFFER::RECTANGLE_BUFFER() :
      Flags(0),
      reserved1(0),
      reserved2(0),
      TopLeftX(),
      TopLeftY(),
      TopLeftZ(),
      BottomRightX(),
      BottomRightY(),
      BottomRightZ()
   {
   }
}