#pragma once
#include "include/qgl_graphics_include.h"
#include "include/Content/Content-Buffers/qgl_brush_buffer.h"

namespace qgl::graphics::content
{
   class QGL_GRAPHICS_API brush : public qgl::content::content_item
   {
      public:
      /*
       Constructs a new brush.
       */
      brush(const buffers::BRUSH_BUFFER* fmtBuffer,
            graphics::d2d_context* devContext_p,
            const wchar_t* name,
            const qgl::content::content_id id);

     /*
      Copy constructor.
      */
      brush(const brush& r) = delete;

      /*
       Move constructor
       */
      brush(brush&& r) = delete;

      /*
       Destructor
       */
      virtual ~brush() noexcept;

      /*
       Returns a const pointer to the D2D brush.
       */
      const ID2D1Brush* get() const noexcept;

      /*
       Returns a pointer to the D2D brush.
       */
      ID2D1Brush* get() noexcept;

      private:
      struct impl;
      impl* m_impl_p;
   };
}