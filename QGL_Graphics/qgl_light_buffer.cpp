#include "pch.h"
#include "include\Content\Content-Buffers\qgl_light_buffer.h"

namespace qgl::content::buffers
{
   static constexpr float LIGHT_DEFAULT_POS[4] =
   {
      0.0f, 0.0f, 0.0f, 0.0f
   };

   static constexpr float LIGHT_DEFAULT_LOOK_AT[4] =
   {
      0.0f, 0.0f, 0.0f, 0.0f
   };

   static constexpr float LIGHT_DEFAULT_COLOR[4] =
   {
      1.0f, 1.0f, 1.0f, 1.0f
   };

	LIGHT_BUFFER::LIGHT_BUFFER() :
      Flags1(0),
      Flags2(0),
      Intensity(1.0f)
	{
      MemoryCopy(Position, LIGHT_DEFAULT_POS, 4);
      MemoryCopy(LookAt, LIGHT_DEFAULT_LOOK_AT, 4);
      MemoryCopy(Color, LIGHT_DEFAULT_COLOR, 4);
	}

   const float* LIGHT_BUFFER::color() const noexcept
   {
      return Color;
   }

   const float* LIGHT_BUFFER::look_at() const noexcept
   {
      return LookAt;
   }

   const float* LIGHT_BUFFER::position() const noexcept
   {
      return Position;
   }

   float LIGHT_BUFFER::intensity() const noexcept
   {
      return Intensity;
   }
}