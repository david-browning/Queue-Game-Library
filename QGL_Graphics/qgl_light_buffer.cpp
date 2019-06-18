#include "pch.h"
#include "include\Content\Content-Buffers\qgl_light_buffer.h"

namespace qgl::content::buffers
{
   static constexpr math::rational<int32_t> LIGHT_DEFAULT_POS[4] =
   {
      math::rational<int32_t>(),
      math::rational<int32_t>(),
      math::rational<int32_t>(),
      math::rational<int32_t>()
   };

   static constexpr math::rational<int32_t> LIGHT_DEFAULT_LOOK_AT[4] =
   {
      math::rational<int32_t>(1, 1),
      math::rational<int32_t>(1, 1),
      math::rational<int32_t>(1, 1),
      math::rational<int32_t>(1, 1)
   };

   static constexpr math::rational<int32_t> LIGHT_DEFAULT_COLOR[4] =
   {
      math::rational<int32_t>(),
      math::rational<int32_t>(),
      math::rational<int32_t>(),
      math::rational<int32_t>()
   };

   LIGHT_BUFFER::LIGHT_BUFFER() :
      Flags1(0),
      Flags2(0),
      Intensity(1, 1)
   {
      copy_elements(Position, LIGHT_DEFAULT_POS, 4);
      copy_elements(LookAt, LIGHT_DEFAULT_LOOK_AT, 4);
      copy_elements(Color, LIGHT_DEFAULT_COLOR, 4);
   }
}