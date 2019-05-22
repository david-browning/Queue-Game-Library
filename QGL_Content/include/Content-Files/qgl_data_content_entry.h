#pragma once
#include "include/qgl_content_include.h"

namespace qgl::content
{
   /*
    The content data stored in a content file is a raw byte array.
    The byte array is not interpreted by content files. Instead, it is the
    job of a content importer to derive meaning from the bytes.
    */
   struct LIB_EXPORT DATA_CONTENT_ENTRY
   {
      public:
      /*
       Copies the byte buffer to internal storage.
       */
      DATA_CONTENT_ENTRY(std::vector<uint8_t>& b);

      /*
       Copies bytes from b to internal storage. After construction, b can be
       freed.
       */
      DATA_CONTENT_ENTRY(const void* const b,
                         size_t bytes);

      /*
       Copy Constructor.
       */
      DATA_CONTENT_ENTRY(const DATA_CONTENT_ENTRY&) = default;

      /*
       Move Constructor.
       */
      DATA_CONTENT_ENTRY(DATA_CONTENT_ENTRY&&) = default;

      /*
       Destructor.
       */
      virtual ~DATA_CONTENT_ENTRY() noexcept = default;

      /*
       Returns a pointer to the buffer. The destructor frees the buffer.
       Do not try to free this pointer.
       */
      uint8_t* data() noexcept;

      /*
       Returns a const pointer to the buffer. The destructor frees the buffer.
       Do not try to free this pointer.
       */
      const uint8_t* data() const noexcept;

      /*
       Returns the number of bytes in the buffer.
       */
      size_t size() const noexcept;

      friend void swap(DATA_CONTENT_ENTRY& first,
                       DATA_CONTENT_ENTRY& second) noexcept
      {
         using std::swap;
         swap(first.m_buffer, second.m_buffer);
      }

      /*
       Assignment Operator.
       */
      DATA_CONTENT_ENTRY& operator=(DATA_CONTENT_ENTRY r) noexcept
      {
         swap(*this, r);
         return *this;
      }

      /*
       Equality Operator.
       */
      friend bool operator==(const DATA_CONTENT_ENTRY& r,
                             const DATA_CONTENT_ENTRY& l) noexcept
      {
         return r.m_buffer == l.m_buffer;
      }

      private:
      std::vector<uint8_t> m_buffer;
   };
}