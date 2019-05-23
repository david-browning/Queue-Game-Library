#include "pch.h"
#include "include/Content-Files/qgl_data_content_entry.h"

using namespace qgl::content;

DATA_CONTENT_ENTRY::DATA_CONTENT_ENTRY(std::vector<uint8_t>& b)
{
   m_buffSize = b.size();
   m_buffer = new uint8_t[m_buffSize];
   memcpy(m_buffer, b.data(), m_buffSize);
}

DATA_CONTENT_ENTRY::DATA_CONTENT_ENTRY(const void* const b,
                                       size_t bytes) 
{
   m_buffSize = bytes;
   m_buffer = new uint8_t[bytes];
   memcpy(m_buffer, b, bytes);
}

DATA_CONTENT_ENTRY::DATA_CONTENT_ENTRY(const DATA_CONTENT_ENTRY& c)
{
   m_buffSize = c.m_buffSize;
   m_buffer = new uint8_t[m_buffSize];
   memcpy(m_buffer, c.m_buffer, m_buffSize);
}

qgl::content::DATA_CONTENT_ENTRY::DATA_CONTENT_ENTRY(DATA_CONTENT_ENTRY&& m)
{
   m_buffSize = m.m_buffSize;
   m_buffer = new uint8_t[m_buffSize];
   memcpy(m_buffer, m.m_buffer, m_buffSize);

   m.m_buffSize = 0;
   delete[] m.m_buffer;
   m.m_buffer = nullptr;
}

qgl::content::DATA_CONTENT_ENTRY::~DATA_CONTENT_ENTRY() noexcept
{
   delete[] m_buffer;
   #ifdef DEBUG
   m_buffer = nullptr;
   m_buffSize = -1;
   #endif
}

void* DATA_CONTENT_ENTRY::data() noexcept
{
   return m_buffer;
}

const void* DATA_CONTENT_ENTRY::data() const noexcept
{
   return m_buffer;
}

size_t DATA_CONTENT_ENTRY::size() const noexcept
{
   return m_buffSize;
}
