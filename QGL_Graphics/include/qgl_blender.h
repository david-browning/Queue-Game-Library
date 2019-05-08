#pragma once
#include "qgl_graphics_include.h"
#include "qgl_blender_buffer.h"

namespace qgl::graphics
{
   /*
    The blender provides the parameters describing how to blend pixels.
    */
   class LIB_EXPORT blender
   {
      public:
      /*
       Constructs a blender with default parameters.
       */
      blender();

      blender(const BLENDER_BUFFER& buff);

      blender(const blender& r);

      blender(blender&& r);

      virtual ~blender();

      inline const D3D12_BLEND_DESC& description() const
      {
         return m_desc;
      }

      inline UINT mask() const
      {
         return m_mask;
      }

      inline UINT& mask()
      {
         return m_mask;
      }

      private:
      D3D12_BLEND_DESC m_desc;
      UINT m_mask;
   };
}