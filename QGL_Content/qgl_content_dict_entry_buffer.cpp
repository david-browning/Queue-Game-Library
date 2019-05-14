#include "pch.h"
#include "include/Content-Buffers/qgl_content_dict_entry_buffer.h"
using namespace qgl::content;

CONTENT_DICTIONARY_ENTRY_BUFFER::CONTENT_DICTIONARY_ENTRY_BUFFER() :
   m_offset(-1),
   m_flags(DEFAULT_FLAGS),
   m_size(0),
   m_info()
{
}

CONTENT_DICTIONARY_ENTRY_BUFFER::CONTENT_DICTIONARY_ENTRY_BUFFER(
   size_t entrySize,
   CONTENT_METADATA_BUFFER& entryInfo,
   size_t entryOffset) :
   m_offset(entryOffset),
   m_flags(DEFAULT_FLAGS),
   m_size(entrySize),
   m_info(entryInfo)
{

}