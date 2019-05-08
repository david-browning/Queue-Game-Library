#pragma once
#include "qgl_content_include.h"
#include "qgl_content_project.h"

namespace qgl::content
{
   /*
    absPath: Path where to save the compiled project.
    */
   void LIB_EXPORT compile_project(content_project& proj,
                                   const winrt::hstring& absPath);
}