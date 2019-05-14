#pragma once
#include "include/qgl_content_include.h"
#include "include/Content-Buffers/qgl_content_metadata_buffer.h"

namespace qgl::content
{
    /*
     The file header is the first bytes of any content file. QGL inspects the 
     header.
     */
   struct LIB_EXPORT CONTENT_FILE_HEADER_BUFFER
   {
      public:

      /*
       Default constructor.
       */
      CONTENT_FILE_HEADER_BUFFER();

      /*
       Creates a file header using the given metadata.
       */
      CONTENT_FILE_HEADER_BUFFER(const CONTENT_METADATA_BUFFER& metadata);

      /*
       Copy constructor.
       */
      CONTENT_FILE_HEADER_BUFFER(
          const CONTENT_FILE_HEADER_BUFFER& r) = default;

      /*
       Move constructor.
       */
      CONTENT_FILE_HEADER_BUFFER(CONTENT_FILE_HEADER_BUFFER&& r) = default;

      /*
       Destructor. Do not make virtual.
       */
      ~CONTENT_FILE_HEADER_BUFFER() = default;

      /*
       Returns a const reference to the metadata.
       */
      inline const CONTENT_METADATA_BUFFER& metadata() const
      {
         return m_metadata;
      }

      auto dictionary_offset() const
      {
         return m_offset;
      }

      friend void swap(CONTENT_FILE_HEADER_BUFFER& first,
                       CONTENT_FILE_HEADER_BUFFER& second) noexcept
      {
         using std::swap;
         swap(first.m_offset, second.m_offset);
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
            r.m_offset == l.m_offset;
      }

      private:
      CONTENT_METADATA_BUFFER m_metadata;

      //Offset to the dictionary.
      uint64_t m_offset;
   };
}