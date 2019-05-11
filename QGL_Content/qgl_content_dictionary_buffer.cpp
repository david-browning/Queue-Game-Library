#include "pch.h"
#include "include/qgl_content_dictionary_buffer.h"
using namespace qgl::content;

qgl::content::CONTENT_DICTIONARY_METADATA_BUFFER::CONTENT_DICTIONARY_METADATA_BUFFER() :
   m_count(0),
   m_entrySize(0),
   m_reserved1(0),
   m_flags(0)
{
}

qgl::content::CONTENT_DICTIONARY_METADATA_BUFFER::CONTENT_DICTIONARY_METADATA_BUFFER(size_t count,
                                                                   size_t entrySize, 
                                                                   uint64_t flags) :
   m_count(count),
   m_entrySize(entrySize),
   m_reserved1(0),
   m_flags(flags)
{
}

qgl::content::CONTENT_DICTIONARY_METADATA_BUFFER::CONTENT_DICTIONARY_METADATA_BUFFER(const qgl::content::CONTENT_DICTIONARY_METADATA_BUFFER& r) :
   m_count(r.m_count),
   m_entrySize(r.m_entrySize),
   m_reserved1(r.m_reserved1),
   m_flags(r.m_flags)
{
}

qgl::content::CONTENT_DICTIONARY_METADATA_BUFFER::CONTENT_DICTIONARY_METADATA_BUFFER(qgl::content::CONTENT_DICTIONARY_METADATA_BUFFER&& r) :
   m_count(r.m_count),
   m_entrySize(r.m_entrySize),
   m_reserved1(r.m_reserved1),
   m_flags(r.m_flags)
{
}

