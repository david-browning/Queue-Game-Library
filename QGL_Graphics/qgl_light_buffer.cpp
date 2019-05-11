#include "pch.h"
#include "include/qgl_light_buffer.h"

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

qgl::graphics::LIGHT_BUFFER::LIGHT_BUFFER() :
   m_intensity(1.0f),
   m_flags1(0),
   m_flags2(0)
{
   MemoryCopy(m_position, LIGHT_DEFAULT_POS, 4);
   MemoryCopy(m_lookAt, LIGHT_DEFAULT_LOOK_AT, 4);
   MemoryCopy(m_color, LIGHT_DEFAULT_COLOR, 4);
}

qgl::graphics::LIGHT_BUFFER::LIGHT_BUFFER(const LIGHT_BUFFER& r) :
   m_intensity(r.intensity()),
   m_flags1(r.flags1()),
   m_flags2(r.flags2())
{
   MemoryCopy(m_position, r.position(), 4);
   MemoryCopy(m_lookAt, r.look_at(), 4);
   MemoryCopy(m_color, r.color(), 4);
}

qgl::graphics::LIGHT_BUFFER::LIGHT_BUFFER(LIGHT_BUFFER&& r) :
   m_intensity(r.intensity()),
   m_flags1(r.flags1()),
   m_flags2(r.flags2())
{
   MemoryCopy(m_position, r.position(), 4);
   MemoryCopy(m_lookAt, r.look_at(), 4);
   MemoryCopy(m_color, r.color(), 4);
}

qgl::graphics::LIGHT_BUFFER::~LIGHT_BUFFER() noexcept
{
}
