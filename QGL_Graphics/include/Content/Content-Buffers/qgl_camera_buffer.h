#pragma once
#include "include/qgl_graphics_include.h"

namespace qgl::graphics::content::buffers
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
         swap(l.m_farPlane, r.m_farPlane);
         swap(l.m_fov, r.m_fov);
         swap(l.m_lookAt, r.m_lookAt);
         swap(l.m_nearPlane, r.m_nearPlane);
         swap(l.m_position, r.m_position);
         swap(l.m_reserved1, r.m_reserved1);
         swap(l.m_reserved2, r.m_reserved2);
         swap(l.m_up, r.m_up);
      }

      CAMERA_BUFFER& operator=(CAMERA_BUFFER r)
      {
         swap(*this, r);
         return *this;
      }
      
      private:
      /*
       Spawn location
       */
      float m_position[4];

      /*
       Spawn up
       */
      float m_up[4];

      /*
       Spawn look at.
       */
      float m_lookAt[4];

      /*
       Field of view.
       */
      float m_fov;

      /*
       How close things can be to be rendered. Anything less than this 
       is not rendered.
       */
      float m_nearPlane;

      /*
       How far away things can be rendered. Anything greater than this is not
       rendered.
       */
      float m_farPlane;

      uint32_t m_reserved1;

      uint32_t m_reserved2;
   };
}