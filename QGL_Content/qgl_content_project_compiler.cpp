#include "pch.h"
#include "include\Content-Project\qgl_content_project_compiler.h"
#include "include/Content-Files/qgl_content_file.h"

namespace qgl::content
{
   void compile_content_project(const content_project& proj,
                                content_file<true>& cf)
   {
      CONTENT_FILE_HEADER_BUFFER hdr(proj.metadata());
      cf.header() = hdr;

      //Fill the dictionary and 
      for (const auto& entry : proj)
      {
         auto bufferHandle = open_file_read(entry.second);
         auto buffer = file_data(bufferHandle);
         cf.emplace_back(entry.first, buffer);
      }

      cf.flush();
   }
}

void qgl::content::compile_content_project(const content_project& proj,
                                           const winrt::hstring& absPath)
{
   //Create a content file in write mode.
   content_file<true> cf(absPath);
   compile_content_project(proj, cf);
}


void qgl::content::compile_content_project(
   const content_project& proj,
   const winrt::Windows::Storage::StorageFile& f)
{
   //Create a content file in write mode.
   content_file<true> cf(f);
   compile_content_project(proj, cf);
}
