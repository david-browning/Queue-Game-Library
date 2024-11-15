#pragma once
#include "include/qgl_content_include.h"
#include "include/Loaders/qgl_iloader_metadata.h"
#include "include/Loaders/qgl_loader_guids.h"

namespace qgl::content
{
   class string_loader_1_0 : public iloader_metadata
   {
      public:
      string_loader_1_0()
      {

      }

      virtual ~string_loader_1_0() noexcept = default;

      virtual void release() noexcept
      {
         delete this;
      }

      virtual iqgl* duplicate() const noexcept
      {
         return new (std::nothrow)string_loader_1_0();
      }

      virtual void id(qgl::guid* g) const noexcept
      {
         *g = STRING_LOADER_GUID;
      }

      virtual result_t input(uint64_t bytes,
                             const void* fileData_p,
                             uint64_t* size_p,
                             void* out_p) noexcept
      {
         // Be sure to include the null terminator.
         // +1 to include the null terminator.
         auto sz = strnlen_s(static_cast<const char*>(fileData_p), 
                             static_cast<size_t>(bytes)) + 1;

         if (!out_p)
         {
            *size_p = sz;
            return S_OK;
         }

         // Size includes the null terminator so only copy size - 1 bytes.
         memcpy(out_p, fileData_p, *size_p - 1);

         // Add the null terminator to the end of the buffer.
         auto charBuffer = static_cast<char*>(out_p);
         charBuffer[*size_p - 1] = '\0';

         return S_OK;
      }

      virtual result_t output(uint64_t bytes,
                             const void* object_p,
                             uint64_t* size_p,
                             void* fileData_p) noexcept
      {
         // Calculating how many bytes are in the string and writing it to the 
         // file buffer is the same logic as calculating how many bytes the 
         // file buffer MAY have and writing it to a null terminated string.
         return input(bytes, object_p, size_p, fileData_p);
      }
   };
}