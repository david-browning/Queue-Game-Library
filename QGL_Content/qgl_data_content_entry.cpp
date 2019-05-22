#include "pch.h"
#include "include/Content-Files/qgl_data_content_entry.h"

using namespace qgl::content;

DATA_CONTENT_ENTRY::DATA_CONTENT_ENTRY(std::vector<uint8_t>& b) :
   m_buffer(b)
{
}

DATA_CONTENT_ENTRY::DATA_CONTENT_ENTRY(const void* const b,
                                       size_t bytes) :
   m_buffer(bytes, 0)
{
   memcpy(m_buffer.data(), b, bytes);
}

uint8_t* DATA_CONTENT_ENTRY::data() noexcept
{
   return m_buffer.data();
}

const uint8_t* DATA_CONTENT_ENTRY::data() const noexcept
{
   return m_buffer.data();
}

size_t DATA_CONTENT_ENTRY::size() const noexcept
{
   return m_buffer.size();
}
