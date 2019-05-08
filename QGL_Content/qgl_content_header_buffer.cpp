#include "pch.h"
#include "include/qgl_content_header_buffer.h"
using namespace qgl::content;

qgl::content::CONTENT_HEADER_BUFFER::CONTENT_HEADER_BUFFER() :
   m_offset(sizeof(*this)),
   m_type()
{
}

qgl::content::CONTENT_HEADER_BUFFER::CONTENT_HEADER_BUFFER(const CONTENT_INFO_BUFFER& contentType) :
   m_offset(sizeof(*this)),
   m_type(contentType)
{
   m_type.content_char_width(sizeof(char));
}

qgl::content::CONTENT_HEADER_BUFFER::CONTENT_HEADER_BUFFER(const CONTENT_HEADER_BUFFER& r) :
   m_offset(r.m_offset),
   m_type(r.m_type)
{
}

qgl::content::CONTENT_HEADER_BUFFER::CONTENT_HEADER_BUFFER(CONTENT_HEADER_BUFFER&& r) :
   m_offset(r.m_offset),
   m_type(std::move(r.m_type))
{
}
