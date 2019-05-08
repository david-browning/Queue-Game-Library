#pragma once
#include "include/qgl_content_include.h"
#include "include/qgl_content_info_buffer.h"

namespace qgl::content
{
   struct LIB_EXPORT CONTENT_HEADER_BUFFER
   {
      public:
      CONTENT_HEADER_BUFFER();

      CONTENT_HEADER_BUFFER(const CONTENT_INFO_BUFFER& contentType);

      CONTENT_HEADER_BUFFER(const CONTENT_HEADER_BUFFER& r);

      CONTENT_HEADER_BUFFER(CONTENT_HEADER_BUFFER&& r);

      inline const CONTENT_INFO_BUFFER& info() const
      {
         return m_type;
      }

      friend void swap(CONTENT_HEADER_BUFFER& first,
                       CONTENT_HEADER_BUFFER& second) noexcept
      {
         using std::swap;
         swap(first.m_offset, second.m_offset);
         swap(first.m_type, second.m_type);
      }

      CONTENT_HEADER_BUFFER& operator=(CONTENT_HEADER_BUFFER r)  noexcept
      {
         swap(*this, r);
         return *this;
      }

      private:
      CONTENT_INFO_BUFFER m_type;

      //Offset to the dictionary.
      uint64_t m_offset;
   };
}