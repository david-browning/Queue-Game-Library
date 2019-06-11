#include "pch.h"
#include "include/Content-Project/qgl_content_project_compiler.h"
#include "include/Interfaces/qgl_icontent_file.h"
#include "include/qgl_file_helpers.h"
#include "include/qgl_file_buffer.h"


namespace qgl::content
{
   HRESULT compile_content_project(const icontent_project* proj,
                                   icontent_file* cf) noexcept
   {
      HRESULT hr = S_OK;
      CONTENT_FILE_HEADER_BUFFER hdr(proj->metadata());
      (*cf->header()) = hdr;

      for (const auto& entry : *proj)
      {
         file_handle bufferHandle;
         hr = open_file_read(entry.second.c_str(), &bufferHandle);
         if (FAILED(hr))
         {
            return hr;
         }

         file_buffer rawBuffer(&bufferHandle);
         DATA_CONTENT_ENTRY buffer(rawBuffer.data(), rawBuffer.size());
         cf->push_back(&entry.first, &buffer);
      }

      return cf->flush();
   }
}

HRESULT qgl::content::compile_content_project(const icontent_project* proj,
                                              const wchar_t* absPath) noexcept
{
   //Create a content file in write mode.
   icontent_file* cf = nullptr;
   auto hr = qgl_open_content_file(absPath,
                                   proj->metadata()->version(),
                                   &cf);
   if (FAILED(hr))
   {
      return hr;
   }

   auto cfSafe = qgl::make_unique<icontent_file>(cf);
   hr = compile_content_project(proj, cfSafe->as<icontent_file>());
   if (FAILED(hr))
   {
      return hr;
   }

   return S_OK;
}


HRESULT qgl::content::compile_content_project_sf(
   const icontent_project* proj,
   const winrt::Windows::Storage::StorageFile& f) noexcept
{
   //Create a content file in write mode.
   icontent_file* cf = nullptr;
   auto hr = qgl_open_content_file_sf(f,
                                      proj->metadata()->version(),
                                      &cf);
   if (FAILED(hr))
   {
      return hr;
   }

   auto cfSafe = qgl::make_unique<icontent_file>(cf);
   hr = compile_content_project(proj, cfSafe->as<icontent_file>());
   if (FAILED(hr))
   {
      return hr;
   }

   return S_OK;
}
