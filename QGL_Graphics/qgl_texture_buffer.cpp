#include "pch.h"
#include "include/qgl_texture_buffer.h"

qgl::graphics::TEXTURE_BUFFER::TEXTURE_BUFFER(const std::vector<uint8_t>& data) :
   m_data(data),
   m_subresources()
{
}

qgl::graphics::TEXTURE_BUFFER::TEXTURE_BUFFER(std::vector<uint8_t>&& data) :
   m_data(std::move(data)),
   m_subresources()
{
}

qgl::graphics::TEXTURE_BUFFER::TEXTURE_BUFFER(size_t texSizeBytes) :
   m_data(texSizeBytes),
   m_subresources()
{
}

qgl::graphics::TEXTURE_BUFFER::TEXTURE_BUFFER(const TEXTURE_BUFFER& r) :
   m_data(r.m_data),
   m_subresources(r.m_subresources),
   m_resourceDimension(r.m_resourceDimension),
   m_format(r.m_format),
   m_width(r.m_width),
   m_height(r.m_height),
   m_depth(r.m_depth),
   m_mipCount(r.m_mipCount),
   m_arraySize(r.m_arraySize),
   m_isCubeMap(r.m_isCubeMap)
{
}

qgl::graphics::TEXTURE_BUFFER::TEXTURE_BUFFER(TEXTURE_BUFFER&& r) :
   m_data(std::move(r.m_data)),
   m_subresources(std::move(r.m_subresources)),
   m_resourceDimension(r.m_resourceDimension),
   m_format(r.m_format),
   m_width(r.m_width),
   m_height(r.m_height),
   m_depth(r.m_depth),
   m_mipCount(r.m_mipCount),
   m_arraySize(r.m_arraySize),
   m_isCubeMap(r.m_isCubeMap)
{
}

qgl::graphics::TEXTURE_BUFFER::~TEXTURE_BUFFER()
{
}
