#pragma once
#include "include/qgl_content_include.h"
#include "include/Interfaces/qgl_icontent_project.h"

namespace qgl::content
{
   /*
    Compiles a content project into a content file and saves it.
    absPath: Path where to save the compiled project.
    */
   extern QGL_CONTENT_API void compile_content_project(
      const icontent_project* proj,
      const wchar_t* absPath);

   /*
    Compiles a content project into a content file and saves it.
    Storage file must have write permissions.
    Storage file permissions should be brokered by the storage file provider.
    */
   extern QGL_CONTENT_API void compile_content_project(
      const icontent_project* proj,
      const winrt::Windows::Storage::StorageFile& f);
}