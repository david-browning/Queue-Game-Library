#pragma once
#include "include/qgl_content_include.h"
#include "include/Descriptors/qgl_content_metadata.h"

namespace qgl::descriptors
{
   /*
    This is the first bytes of a content file.
    */
#pragma pack(push, 1)
   struct file_header final
   {
      constexpr file_header()
      {

      }

      file_header(const file_header&) = default;

      file_header(file_header&&) noexcept = default;

      ~file_header() noexcept = default;

      friend void swap(file_header& l, file_header& r) noexcept
      {
         using std::swap;
         swap(l.metadata, r.metadata);
         swap(l.offset, r.offset);
      }

      file_header& operator=(file_header r) noexcept
      {
         swap(*this, r);
         return *this;
      }

      content_metadata metadata;

      /*
       Dictionary offset in bytes.
       */
      uint64_t offset = 0;
   };
#pragma pack(pop)
}