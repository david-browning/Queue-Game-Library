#pragma once
#include "include/qgl_graphics_include.h"
#include "include/GPU/Root-Signature/qgl_root_signature.h"

namespace qgl::graphics::gpu
{
   class ipso
   {
      public:

      /*
       Returns a pointer to the D3D pipeline state.
       Returns nullptr if the pipeline cannot be
       created, possibly due to an incorrect parameter.
       */
      virtual const ID3D12PipelineState* get() const noexcept = 0;

      /*
       Returns a pointer to the D3D pipeline state.
       Returns nullptr if the pipeline cannot be
       created, possibly due to an incorrect parameter.
       */
      virtual ID3D12PipelineState* get() noexcept = 0;

      private:
   };
}