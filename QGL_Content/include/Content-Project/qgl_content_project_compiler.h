#pragma once
#include "include/qgl_content_include.h"
#include "include/Content-Project/qgl_content_project.h"
#include "include/qgl_file_helpers.h"

namespace qgl::content
{
   /*
    Compiles a content project into a content file and saves it.
    hndl: Handle to a file where the content project will be compiled.
    The handle must have write permissions. This does not close the handle. 
    */
   void LIB_EXPORT compile_project(const content_project& proj,
                                   const winrt::file_handle& hndl);

   /*
    absPath: Path where to save the compiled project.
    */
   void compile_project(const content_project& proj,
                        const winrt::hstring& absPath)
   {
      auto handle = open_file_write(absPath);
      compile_project(proj, handle);
   }

   /*
    Storage file permissions should be brokered by the storage file provider.
    */
   void compile_project(const content_project& proj,
                        const winrt::Windows::Storage::StorageFile& f)
   {
      auto handle = open_file_write(f);
      compile_project(proj, handle);
   }
}