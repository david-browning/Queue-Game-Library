#include "pch.h"
#include "include/Content-Project/qgl_content_project_compiler.h"
#include "include/Interfaces/qgl_icontent_file.h"
#include "include/qgl_file_helpers.h"
#include "include/Interfaces/qgl_ifile_buffer.h"

namespace qgl::content
{
   HRESULT compile_content_project(const icontent_project* proj,
                                   icontent_file* cf) noexcept
   {
      CONTENT_FILE_HEADER_BUFFER hdr(proj->metadata());
      (*cf->header()) = hdr;

      for (const auto& entry : *proj)
      {
         if (entry.first.shared())
         {
            //Create a shared entry using the file path.
         }
         else
         {
         //Load the entry's file into memory.
            ifile_buffer* buff = nullptr;
            auto hr = qgl_open_file_buffer(entry.second.c_str(),
                                           hdr.metadata()->version(),
                                           &buff);
            if (FAILED(hr))
            {
               return hr;
            }

            auto safeBuffer = qgl::make_unique<ifile_buffer>(buff);

            cf->push_data_entry(&entry.first,
                                safeBuffer->const_data(),
                                safeBuffer->size());
         }
      }

      //Transfered all entries from the project to the content file.
      //Flush the content file to save.
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
