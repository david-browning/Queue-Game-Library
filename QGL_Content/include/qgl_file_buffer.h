#pragma once
#include "include/qgl_content_include.h"

namespace qgl::content
{
   class file_handle;

   class QGL_CONTENT_API file_buffer
   {
      public:
      file_buffer(const file_handle*);

      file_buffer(const file_buffer&) = delete;

      file_buffer(file_buffer&&);

      ~file_buffer() noexcept;

      const void* data() const noexcept;

      size_t size() const noexcept;

      private:
      struct impl;
      impl* m_impl_p;
   };
}