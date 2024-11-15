#pragma once
#include "include/qgl_content_include.h"
#include "include/Loaders/qgl_iloader_metadata.h"
#include "include/Loaders/qgl_loader_guids.h"

namespace qgl::content
{
   class wstring_loader_1_0 : public iloader_metadata
   {
      public:
      wstring_loader_1_0()
      {

      }

      virtual ~wstring_loader_1_0() noexcept = default;

      virtual void release() noexcept
      {
         delete this;
      }

      virtual iqgl* duplicate() const noexcept
      {
         return new (std::nothrow)wstring_loader_1_0();
      }

      virtual void id(qgl::guid* g) const noexcept
      {
         *g = WSTRING_LOADER_GUID;
      }

      virtual result_t input(uint64_t bytes,
                             const void* fileData_p,
                             uint64_t* size_p,
                             void* out_p) noexcept
      {
         // File data may or may not include the null terminator.
         auto numElements = wcsnlen_s(static_cast<const wchar_t*>(fileData_p),
                                      static_cast<size_t>(bytes));

         // Number of bytes needed including null terminator.
         auto sz = sizeof(wchar_t) * (numElements + 1);

         if (!out_p)
         {
            *size_p = sz;
            return S_OK;
         }

         // Size includes the null terminator so only copy 
         // size - sizeof(wchar_t) bytes.
         memcpy(out_p, fileData_p, *size_p - sizeof(wchar_t));

         // Add the null terminator to the end of the buffer.
         auto wcharBuffer = static_cast<wchar_t*>(out_p);
         wcharBuffer[numElements] = L'\0';

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