#pragma once
#include "include/qgl_graphics_include.h"

namespace qgl::graphics::gpu
{
   /*
    ibindable classes are meant to be attached to a root signature.
    */
   class QGL_GRAPHICS_API ibindable
   {
      public:
      /*
       Constructor.
       rootIndex: Index into the root signature where to bind this.
       */
      ibindable(UINT rootIndex) :
         m_rootIndex(rootIndex)
      {

      }

      /*
       Bindable objects cannot share a root signature index so do not allow
       copying.
       */
      ibindable(const ibindable&) = delete;

      /*
       Move constructor.
       */
      ibindable(ibindable&&) = default;

      /*
       Destructor.
       */
      virtual ~ibindable() noexcept = default;

      /*
       Returns the root signature index where this is bound.
       */
      UINT root_index() const noexcept
      {
         return m_rootIndex;
      }

      /*
       Returns a const pointer to a D3D root parameter. The root signature uses
       this to bind this to the root signature.
       */
      virtual const CD3DX12_ROOT_PARAMETER1* root_param() const = 0;

      private:
      UINT m_rootIndex;
   };
}