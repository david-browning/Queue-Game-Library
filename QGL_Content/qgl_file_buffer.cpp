#include "pch.h"
#include "include\qgl_file_buffer.h"

namespace qgl::content
{
   file_buffer::file_buffer(const file_handle*)
   {
   }
   file_buffer::file_buffer(file_buffer&&)
   {
   }

   file_buffer::~file_buffer() noexcept
   {
   }

   const void* file_buffer::data() const noexcept
   {
      return nullptr;
   }

   size_t file_buffer::size() const noexcept
   {
      return size_t();
   }
}