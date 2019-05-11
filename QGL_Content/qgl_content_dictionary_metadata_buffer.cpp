#include "pch.h"
#include "include/Content-Buffers/qgl_content_dictionary_metadata_buffer.h"
using namespace qgl::content;

CONTENT_DICTIONARY_METADATA_BUFFER::CONTENT_DICTIONARY_METADATA_BUFFER() :
    m_count(0),
    m_entrySize(0),
    m_reserved1(0),
    m_flags(DEFAULT_FLAGS)
{
}

CONTENT_DICTIONARY_METADATA_BUFFER::CONTENT_DICTIONARY_METADATA_BUFFER(
    size_t count,
    size_t entrySize,
    uint64_t flags) :
    m_count(count),
    m_entrySize(entrySize),
    m_reserved1(0),
    m_flags(flags)
{
}


