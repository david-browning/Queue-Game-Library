#pragma once
#include "include/qgl_content_include.h"
#include "include/Content-Buffers/qgl_content_metadata_buffer.h"

namespace qgl::content
{
    /*
     The file header is the first bytes of any content file. QGL inspects the 
     header.
     */
   #pragma pack(push, 1)
   struct QGL_CONTENT_API CONTENT_FILE_HEADER_BUFFER final
   {
      public:

      /*
       Default constructor.
       Sets the dictionary offset to sizeof(*this) and the metadata uses 
       its default constructor.
       */
      CONTENT_FILE_HEADER_BUFFER();

      /*
       Creates a file header using the given metadata.
       */
      CONTENT_FILE_HEADER_BUFFER(const CONTENT_METADATA_BUFFER* metadata);

      /*
       Copy constructor.
       */
      CONTENT_FILE_HEADER_BUFFER(const CONTENT_FILE_HEADER_BUFFER&) = default;

      /*
       Move constructor.
       */
      CONTENT_FILE_HEADER_BUFFER(CONTENT_FILE_HEADER_BUFFER&&) = default;

      /*
       Destructor. Do not make virtual.
       */
      ~CONTENT_FILE_HEADER_BUFFER() = default;

      /*
       Returns a const reference to the metadata.
       */
      const CONTENT_METADATA_BUFFER* metadata() const noexcept;

      /*
       The dictionary always comes after the file header. This should always
       be sizeof(*this)
       */
      uint64_t dictionary_offset() const noexcept;

      friend void swap(CONTENT_FILE_HEADER_BUFFER& first,
                       CONTENT_FILE_HEADER_BUFFER& second) noexcept
      {
         using std::swap;
         swap(first.m_offset, second.m_offset);
         swap(first.m_reserved1, second.m_reserved1);
         swap(first.m_reserved2, second.m_reserved2);
         swap(first.m_reserved3, second.m_reserved3);
         swap(first.m_metadata, second.m_metadata);
      }

      CONTENT_FILE_HEADER_BUFFER& operator=(
          CONTENT_FILE_HEADER_BUFFER r)  noexcept
      {
         swap(*this, r);
         return *this;
      }

      friend bool operator==(const CONTENT_FILE_HEADER_BUFFER& r,
                             const CONTENT_FILE_HEADER_BUFFER& l) noexcept
      {
         return r.m_metadata == l.m_metadata &&
            r.m_reserved1 == r.m_reserved1 &&
            r.m_reserved2 == r.m_reserved2 &&
            r.m_reserved3 == r.m_reserved3 &&
            r.m_offset == l.m_offset;
      }

      private:
      CONTENT_METADATA_BUFFER m_metadata;

      uint64_t m_reserved1;
      uint64_t m_reserved2;
      uint64_t m_reserved3;

      //Offset to the dictionary.
      uint64_t m_offset;
   };
   #pragma pack(pop)
}