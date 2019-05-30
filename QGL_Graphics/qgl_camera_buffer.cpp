#include "pch.h"
#include "include/Content/Content-Buffers/qgl_camera_buffer.h"

namespace qgl::graphics::content::buffers
{
   CAMERA_BUFFER::CAMERA_BUFFER() :
      m_reserved1(0),
      m_reserved2(0),
      m_nearPlane(0.01f),
      m_farPlane(100.0f),
      m_fov(1)
   {
      for (int i = 0; i < 4; i++)
      {
         m_position[i] = 0.0f;
         m_up[i] = 0.0f;
         m_lookAt[0] = 0.0f;
      }
   }

   const float* CAMERA_BUFFER::position() const
   {
      return m_position;
   }

   const float* CAMERA_BUFFER::up() const
   {
      return m_up;
   }

   const float* CAMERA_BUFFER::look_at() const
   {
      return m_lookAt;
   }

   float CAMERA_BUFFER::near_plane() const
   {
      return m_nearPlane;
   }

   float CAMERA_BUFFER::far_plane() const
   {
      return m_farPlane;
   }

   float CAMERA_BUFFER::fov() const
   {
      return  m_fov;
   }
}