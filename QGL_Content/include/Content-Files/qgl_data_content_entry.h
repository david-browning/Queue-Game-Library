#pragma once
#include "include/qgl_content_include.h"

namespace qgl::content
{
   /*
    The content data stored in a content file is a raw byte array.
    The byte array is not interpreted by content files. Instead, it is the
    job of a content importer to derive meaning from the bytes.
    */
   struct QGL_CONTENT_API DATA_CONTENT_ENTRY
   {
      public:
      constexpr DATA_CONTENT_ENTRY();

      /*
       Copies bytes from b to internal storage. After construction, b can be
       freed.
       */
      DATA_CONTENT_ENTRY(const void* const b,
                         size_t bytes);

      /*
       Copy Constructor.
       */
      DATA_CONTENT_ENTRY(const DATA_CONTENT_ENTRY&);

      /*
       Move Constructor.
       */
      DATA_CONTENT_ENTRY(DATA_CONTENT_ENTRY&&);

      /*
       Destructor.
       */
      virtual ~DATA_CONTENT_ENTRY() noexcept;

      /*
       Returns a pointer to the buffer. The destructor frees the buffer.
       Do not try to free this pointer.
       */
      void* data() noexcept;

      /*
       Returns a const pointer to the buffer. The destructor frees the buffer.
       Do not try to free this pointer.
       */
      const void* data() const noexcept;

      /*
       Returns the number of bytes in the buffer.
       */
      size_t size() const noexcept;

      friend void swap(DATA_CONTENT_ENTRY& first,
                       DATA_CONTENT_ENTRY& second) noexcept
      {
         using std::swap;
         swap(first.m_buffer, second.m_buffer);
         swap(first.m_buffSize, second.m_buffSize);
      }

      /*
       Assignment Operator.
       */
      DATA_CONTENT_ENTRY& operator=(DATA_CONTENT_ENTRY r) noexcept;

      /*
       Equality Operator.
       */
      friend bool operator==(const DATA_CONTENT_ENTRY& r,
                             const DATA_CONTENT_ENTRY& l) noexcept
      {
         return memcmp(r.m_buffer, l.m_buffer, r.m_buffSize) == 0;
      }

      private:
      void* m_buffer;
      size_t m_buffSize;
   };
}