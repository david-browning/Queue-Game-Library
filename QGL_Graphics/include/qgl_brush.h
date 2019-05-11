#pragma once
#include "qgl_graphics_include.h"
#include "qgl_brush_buffer.h"

namespace qgl::graphics
{
   class LIB_EXPORT brush : public qgl::content::wcontent_item
   {
      public:
      brush(const BRUSH_BUFFER& fmtBuffer,
            const winrt::com_ptr<d2d_context>& devContext_p,
            const content::wcontent_item::str_t& name,
            const content::wcontent_item::id_t& id);

     /*
      Copy constructor.
      This does a deep copy of the brush, but not the render target.
      */
      brush(const brush& r);

      /*
       Move constructor
       */
      brush(brush&& r);

      /*
       Destructor
       */
      virtual ~brush() noexcept = default;

      const ID2D1Brush* get() const noexcept
      {
         return m_brush.get();
      }

      ID2D1Brush* get() noexcept
      {
         return m_brush.get();
      }

      const auto& com_get() const noexcept
      {
         return m_brush;
      }

      auto& com_get() noexcept
      {
         return m_brush;
      }

      private:
      void p_make_brush();

      BRUSH_BUFFER m_brushData;
      #pragma warning(push)
      #pragma warning(disable: 4251)
      winrt::com_ptr<d2d_context> m_devContext;
      winrt::com_ptr<ID2D1Brush> m_brush;
      #pragma warning(pop)
   };
}