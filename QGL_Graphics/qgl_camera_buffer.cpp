#include "pch.h"
#include "include/Content/Content-Buffers/qgl_camera_buffer.h"

namespace qgl::content::buffers
{
   static constexpr float DEFAULT_CAMERA_LOOK_AT[4] =
   {
      1.0f, 1.0f, 1.0f, 1.0f
   };

   static constexpr float DEFAULT_CAMERA_POSITION[4] =
   {
      0.0f, 0.0f, 0.0f, 0.0f
   };

   CAMERA_BUFFER::CAMERA_BUFFER() :
      Reserved1(0),
      Reserved2(0),
      NearPlane(1.0f),
      FarPlane(10000.0f),
      FOV(1.0f)
   {
      MemoryCopy<float>(Position, DEFAULT_CAMERA_POSITION, 4);
      MemoryCopy<float>(LookAt, DEFAULT_CAMERA_LOOK_AT, 4);
      MemoryCopy<float>(Up, DEFAULT_CAMERA_LOOK_AT, 4);
   }

   const float* CAMERA_BUFFER::position() const
   {
      return Position;
   }

   const float* CAMERA_BUFFER::up() const
   {
      return Up;
   }

   const float* CAMERA_BUFFER::look_at() const
   {
      return LookAt;
   }

   float CAMERA_BUFFER::near_plane() const
   {
      return NearPlane;
   }

   float CAMERA_BUFFER::far_plane() const
   {
      return FarPlane;
   }

   float CAMERA_BUFFER::fov() const
   {
      return  FOV;
   }
}