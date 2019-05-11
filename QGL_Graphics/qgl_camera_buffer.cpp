#include "pch.h"
#include "include/qgl_camera_buffer.h"

static constexpr float DEFAULT_CAMERA_LOOK_AT[4] =
{
   1.0f, 1.0f, 1.0f, 1.0f
};

static constexpr float DEFAULT_CAMERA_POSITION[4] =
{
   0.0f, 0.0f, 0.0f, 0.0f
};

qgl::graphics::CAMERA_BUFFER::CAMERA_BUFFER() :
   m_fov(1.0f),
   m_farPlane(10000.0f),
   m_nearPlane(1.0f)
{
   MemoryCopy<float>(m_position, DEFAULT_CAMERA_POSITION, 4);
   MemoryCopy<float>(m_lookAt, DEFAULT_CAMERA_LOOK_AT, 4);
   MemoryCopy<float>(m_up, DEFAULT_CAMERA_LOOK_AT, 4);
}

qgl::graphics::CAMERA_BUFFER::CAMERA_BUFFER(const CAMERA_BUFFER& r) :
   m_fov(r.fov()),
   m_farPlane(r.far_plane()),
   m_nearPlane(r.near_plane())
{
   MemoryCopy<float>(m_position, r.position(), 4);
   MemoryCopy<float>(m_lookAt, r.look_at(), 4);
   MemoryCopy<float>(m_up, r.up(), 4);
}

qgl::graphics::CAMERA_BUFFER::CAMERA_BUFFER(CAMERA_BUFFER&& r) :
   m_fov(r.fov()),
   m_farPlane(r.far_plane()),
   m_nearPlane(r.near_plane())
{
   MemoryCopy<float>(m_position, r.position(), 4);
   MemoryCopy<float>(m_lookAt, r.look_at(), 4);
   MemoryCopy<float>(m_up, r.up(), 4);
}

qgl::graphics::CAMERA_BUFFER::~CAMERA_BUFFER() noexcept
{
}
