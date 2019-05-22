#include "pch.h"
#include "include/Content-Buffers/qgl_content_file_header_buffer.h"
using namespace qgl::content;

qgl::content::CONTENT_FILE_HEADER_BUFFER::CONTENT_FILE_HEADER_BUFFER() :
   m_offset(sizeof(*this)),
   m_reserved1(0),
   m_reserved2(0),
   m_reserved3(0)
{
}

qgl::content::CONTENT_FILE_HEADER_BUFFER::CONTENT_FILE_HEADER_BUFFER(
   const CONTENT_METADATA_BUFFER& metadata) :
   m_offset(sizeof(*this)),
   m_metadata(metadata),
   m_reserved1(0),
   m_reserved2(0),
   m_reserved3(0)
{
}
