#pragma once
#include "include/qgl_graphics_include.h"
#include "include/Content/Content-Descriptors/qgl_brush_outline_descriptor.h"

namespace qgl::graphics
{
   /*
    Used to draw an outline around filled geometry.
    */
   class outline_brush
   {
      public:
      outline_brush(descriptors::brush_outline_descriptor&& desc,
                    i2d_context* devContext_p,
                    i2d_factory* fac_p) :
         m_desc(std::forward<descriptors::brush_outline_descriptor>(desc))
      {
         make(devContext_p, fac_p);
      }

      ID2D1Brush* get() noexcept
      {
         return m_brush.get();
      }

      const ID2D1Brush* get() const noexcept
      {
         return m_brush.get();
      }

      ID2D1StrokeStyle* style() noexcept
      {
         return m_style.get();
      }

      const ID2D1StrokeStyle* style() const noexcept
      {
         return m_style.get();
      }

      float width() const noexcept
      {
         return static_cast<float>(m_desc.width);
      }

      private:
      void make(i2d_context* devContext_p, i2d_factory* fac_p)
      {
         check_result(devContext_p->CreateSolidColorBrush(
            to_d2d_color(m_desc.outline),
            reinterpret_cast<ID2D1SolidColorBrush**>(m_brush.put())));

         auto cap = static_cast<D2D1_CAP_STYLE>(m_desc.cap);

         D2D1_STROKE_STYLE_PROPERTIES props;
         props.startCap = cap;
         props.endCap = cap;
         props.dashCap = cap;
         props.lineJoin = static_cast<D2D1_LINE_JOIN>(m_desc.join);
         props.dashStyle = static_cast<D2D1_DASH_STYLE>(m_desc.style);
         props.dashOffset = static_cast<float>(m_desc.dash_offset);
         props.miterLimit = 1.0f;

         check_result(fac_p->CreateStrokeStyle(
            props,
            nullptr,
            0,
            m_style.put()));
      }

      descriptors::brush_outline_descriptor m_desc;
      pptr<ID2D1Brush> m_brush;
      pptr<ID2D1StrokeStyle> m_style;
   };
}