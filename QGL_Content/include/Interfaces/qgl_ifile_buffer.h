#pragma once
#include "include/qgl_content_include.h"

namespace qgl::content
{
   class file_handle;

   extern "C"
   {
      /*
       Wraps a unique pointer to a file and its data. The file is opened for
       read access only.
       */
      struct ifile_buffer : public iqgl
      {
         /*
          Returns a const reference to the file data.
          */
         virtual const void* const_data() const noexcept = 0;

         /*
          Returns the number of bytes in the file.
          */
         virtual size_t size() const noexcept = 0;

         /*
          Returns a const reference to the file handle. This is automatically
          closed when the calling release().
          */
         virtual const file_handle* handle() const noexcept = 0;
      };
   }

   extern "C"[[nodiscard]] QGL_CONTENT_API HRESULT QGL_CC
      qgl_open_file_buffer(
         const wchar_t* filePath,
         qgl::qgl_version_t v,
         ifile_buffer** out_p) noexcept;
}