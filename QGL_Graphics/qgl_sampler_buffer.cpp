#include "pch.h"
#include "include/qgl_sampler_buffer.h"

static constexpr float DEFAULT_SAMPLER_BORDER_COLOR[4] =
{
   0.0f, 0.0f, 0.0f, 0.0f
};

qgl::graphics::SAMPLER_BUFFER::SAMPLER_BUFFER() :
   m_mipLODBias(0.0f),
   m_maxLOD(0.0f),
   m_minLOD(0.0f),
   m_maxAnisotropy(0),
   m_filter(0),
   m_addressU(0),
   m_addressV(0),
   m_addressW(0),
   m_comparisonFunction(0),
   m_reserved1(0),
   m_reserved2(0),
   m_reserved3(0)
{
   MemoryCopy<float>(m_borderColor, DEFAULT_SAMPLER_BORDER_COLOR, 4);
}

qgl::graphics::SAMPLER_BUFFER::SAMPLER_BUFFER(const SAMPLER_BUFFER& r) :
   m_mipLODBias(r.m_mipLODBias),
   m_maxLOD(r.m_maxLOD),
   m_minLOD(r.m_minLOD),
   m_maxAnisotropy(r.m_maxAnisotropy),
   m_filter(r.m_filter),
   m_addressU(r.m_addressU),
   m_addressV(r.m_addressV),
   m_addressW(r.m_addressW),
   m_comparisonFunction(r.m_comparisonFunction),
   m_reserved1(r.m_reserved1),
   m_reserved2(r.m_reserved2),
   m_reserved3(r.m_reserved3)
{
   MemoryCopy<float>(m_borderColor, r.m_borderColor, 4);
}

qgl::graphics::SAMPLER_BUFFER::SAMPLER_BUFFER(SAMPLER_BUFFER&& r) :
   m_mipLODBias(r.m_mipLODBias),
   m_maxLOD(r.m_maxLOD),
   m_minLOD(r.m_minLOD),
   m_maxAnisotropy(r.m_maxAnisotropy),
   m_filter(r.m_filter),
   m_addressU(r.m_addressU),
   m_addressV(r.m_addressV),
   m_addressW(r.m_addressW),
   m_comparisonFunction(r.m_comparisonFunction),
   m_reserved1(r.m_reserved1),
   m_reserved2(r.m_reserved2),
   m_reserved3(r.m_reserved3)
{
   MemoryCopy<float>(m_borderColor, r.m_borderColor, 4);
}

qgl::graphics::SAMPLER_BUFFER::~SAMPLER_BUFFER() noexcept
{
}
