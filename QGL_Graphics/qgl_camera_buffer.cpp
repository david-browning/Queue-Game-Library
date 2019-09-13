#include "pch.h"
#include "include/Content/Content-Buffers/qgl_camera_buffer.h"
using namespace qgl::mem;

namespace qgl::content::buffers
{
   static constexpr math::rational<int32_t> DEFAULT_CAMERA_LOOK_AT[4] =
   {
      math::rational<int32_t>(1, 1),
      math::rational<int32_t>(1, 1),
      math::rational<int32_t>(1, 1),
      math::rational<int32_t>(1, 1)
   };

   static constexpr math::rational<int32_t> DEFAULT_CAMERA_POSITION[4] =
   {
      math::rational<int32_t>(),
      math::rational<int32_t>(),
      math::rational<int32_t>(),
      math::rational<int32_t>()
   };

   CAMERA_BUFFER::CAMERA_BUFFER() :
      Reserved1(0),
      Reserved2(0),
      NearPlane(1, 0),
      FarPlane(10000, 0),
      FOV(1, 0)
   {
      copy_elements(Position, DEFAULT_CAMERA_POSITION, 4);
      copy_elements(LookAt, DEFAULT_CAMERA_LOOK_AT, 4);
      copy_elements(Up, DEFAULT_CAMERA_LOOK_AT, 4);
   }
}