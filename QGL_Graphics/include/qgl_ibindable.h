#pragma once
#include "qgl_graphics_include.h"

namespace qgl::graphics::low
{
   /*
    ibindable classes are meant to be attached to a root signature.
    rootIndex: Index into the root signature where to bind this.
    */
   class LIB_EXPORT ibindable
   {
      public:
      ibindable(UINT rootIndex) :
         _rootIndex(rootIndex)
      {
      }

      virtual ~ibindable() noexcept
      {

      }

      /*
       Returns the root signature index where this is bound.
       */
      inline UINT root_index() const
      {
         return _rootIndex;
      }

      virtual const CD3DX12_ROOT_PARAMETER1 root_param() const = 0;

      protected:

      private:
      UINT _rootIndex;
   };
}