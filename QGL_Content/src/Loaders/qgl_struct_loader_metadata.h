#pragma once
#include "include/qgl_content_include.h"
#include "include/Loaders/qgl_iloader_metadata.h"
#include "include/Loaders/qgl_loader_guids.h"

namespace qgl::content
{
   /*
    The struct loader is just a one to one copy of the file data.
    */
   class struct_loader_1_0 : public iloader_metadata
   {
      public:
      struct_loader_1_0()
      {

      }

      virtual ~struct_loader_1_0() noexcept = default;

      virtual void release() noexcept
      {
         delete this;
      }

      virtual iqgl* duplicate() const noexcept
      {
         return new (std::nothrow)struct_loader_1_0();
      }

      virtual void id(qgl::guid* g) const noexcept
      {
         *g = STRUCT_LOADER_GUID;
      }

      virtual result_t input(uint64_t bytes,
                             const void* fileData_p,
                             uint64_t* size_p,
                             void* out_p) noexcept
      {
         if (!out_p)
         {
            *size_p = bytes;
            return S_OK;
         }

         // Buffers should be the same size.
         if (bytes != *size_p)
         {
            return E_INVALIDARG;
         }

         memcpy(out_p, fileData_p, bytes);
         return S_OK;
      }

      virtual result_t output(uint64_t bytes,
                             const void* object_p,
                             uint64_t* size_p,
                             void* fileData_p) noexcept
      {
         if (!fileData_p)
         {
            *size_p = bytes;
            return S_OK;
         }

         // Buffers should be the same size.
         if (bytes != *size_p)
         {
            return E_INVALIDARG;
         }

         memcpy(fileData_p, object_p, bytes);
         return S_OK;
      }
   };
}