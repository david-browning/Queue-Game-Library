#pragma once
#include "include/qgl_model_include.h"
#include "include/Interfaces/qgl_interface.h"
#include "include/qgl_version.h"
#include "QGLPlatform.h"

namespace qgl
{
   /*
    Represents a loadable modules. This is stable at ABI boundaries.
    Instantiate by using the "make_module" factory function.
    */
   QGL_INTERFACE imodule : public iqgl
   {
      public:
      virtual result_t address(
         const char* procName, uint64_t* out_p) const noexcept = 0;
   };

   extern "C" QGL_MODEL_API result_t QGL_CC make_module(
      const sys_char* const path, qgl_version v, imodule** h_p) noexcept;
}