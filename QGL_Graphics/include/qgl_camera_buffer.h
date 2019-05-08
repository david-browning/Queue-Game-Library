#pragma once
#include "qgl_graphics_include.h"

namespace qgl::graphics
{
   struct LIB_EXPORT CAMERA_BUFFER
   {
      public:
      CAMERA_BUFFER();

      CAMERA_BUFFER(const CAMERA_BUFFER& r);

      CAMERA_BUFFER(CAMERA_BUFFER&& r);

      ~CAMERA_BUFFER() noexcept;

      inline const float* position() const
      {
         return m_position;
      }

      inline const float* up() const
      {
         return m_up;
      }

      inline const float* look_at() const
      {
         return m_lookAt;
      }

      inline float near_plane() const
      {
         return m_nearPlane;
      }

      inline float far_plane() const
      {
         return m_farPlane;
      }

      inline float fov() const
      {
         return m_fov;
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

      float m_fov;

      float m_nearPlane;
      float m_farPlane;
   };
}