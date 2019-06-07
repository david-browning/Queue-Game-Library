#include "pch.h"
#include "include\Content-Project/qgl_content_project_compiler.h"
#include "include/Interfaces/qgl_icontent_file.h"
#include "include/qgl_file_helpers.h"


namespace qgl::content
{
   void compile_content_project(const content_project& proj,
                                icontent_file* cf)
   {
      CONTENT_FILE_HEADER_BUFFER hdr(proj.metadata());
      (*cf->header()) = hdr;

      //Fill the dictionary and 
      for (const auto& entry : proj)
      {
         file_handle bufferHandle;
         open_file_read(entry.second.c_str(), &bufferHandle);
         auto rawBuffer = file_data(&bufferHandle);
         DATA_CONTENT_ENTRY buffer(rawBuffer.data(), rawBuffer.size());
         cf->push_back(&entry.first, &buffer);
      }

      cf->flush();
   }
}

void qgl::content::compile_content_project(const content_project* proj,
                                           const wchar_t* absPath)
{
   //Create a content file in write mode.
   auto cf = qgl::make_shared(qgl_open_content_file(absPath));
   compile_content_project(*proj, cf->as<icontent_file>());
}


void qgl::content::compile_content_project(
   const content_project* proj,
   const winrt::Windows::Storage::StorageFile& f)
{
   //Create a content file in write mode.
   auto cf = qgl::make_shared(qgl_open_content_file(f));
   compile_content_project(*proj, cf->as<icontent_file>());
}
