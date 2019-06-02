#pragma once
#include "include/qgl_graphics_include.h"

namespace qgl::content::buffers
{
   struct QGL_GRAPHICS_API CAMERA_BUFFER
   {
      public:

      /*
       Default constructor.
       */
      CAMERA_BUFFER();

      /*
       Copy constructor.
       */
      CAMERA_BUFFER(const CAMERA_BUFFER& r) = default;

      /*
       Move constructor.
       */
      CAMERA_BUFFER(CAMERA_BUFFER&& r) = default;

      /*
       Destructor. Do not mark as virtual.
       */
      ~CAMERA_BUFFER() noexcept = default;

      const float* position() const;

      const float* up() const;

      const float* look_at() const;

      float near_plane() const;

      float far_plane() const;

      /*
       Returns the field of view.
       */
      float fov() const;

      friend void swap(CAMERA_BUFFER& l, CAMERA_BUFFER& r) noexcept
      {
         using std::swap;
         swap(l.FarPlane, r.FarPlane);
         swap(l.FOV, r.FOV);
         swap(l.LookAt, r.LookAt);
         swap(l.NearPlane, r.NearPlane);
         swap(l.Position, r.Position);
         swap(l.Reserved1, r.Reserved1);
         swap(l.Reserved2, r.Reserved2);
         swap(l.Up, r.Up);
      }

      CAMERA_BUFFER& operator=(CAMERA_BUFFER r)
      {
         swap(*this, r);
         return *this;
      }
      
      /*
       Spawn location
       */
      float Position[4];

      /*
       Spawn up
       */
      float Up[4];

      /*
       Spawn look at.
       */
      float LookAt[4];

      /*
       Field of view.
       */
      float FOV;

      /*
       How close things can be to be rendered. Anything less than this 
       is not rendered.
       */
      float NearPlane;

      /*
       How far away things can be rendered. Anything greater than this is not
       rendered.
       */
      float FarPlane;

      uint32_t Reserved1;

      uint32_t Reserved2;
   };
}