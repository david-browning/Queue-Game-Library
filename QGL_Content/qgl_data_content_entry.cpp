#include "pch.h"
#include "include/Content-Files/qgl_data_content_entry.h"

namespace qgl::content::entries
{
   data_content_entry::data_content_entry()
   {
   }

   data_content_entry::data_content_entry(const void* const b,
                                          size_t bytes)
   {
      m_buffer.resize(bytes);
      memcpy(m_buffer.data(), b, bytes);
   }

   data_content_entry::data_content_entry(const std::vector<uint8_t>& bytes) :
      m_buffer(bytes)
   {
   }

   const void* data_content_entry::data() const noexcept
   {
      return m_buffer.data();
   }

   size_t data_content_entry::size() const noexcept
   {
      return m_buffer.size();
   }
}
