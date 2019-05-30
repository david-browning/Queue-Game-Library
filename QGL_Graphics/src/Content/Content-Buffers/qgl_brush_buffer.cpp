#include "pch.h"
#include "include/Content/Content-Buffers/qgl_brush_buffer.h"

namespace qgl::graphics::content::buffers
{
   BRUSH_BUFFER::BRUSH_BUFFER() :
      m_style(static_cast<uint8_t>(TEXT_BRUSH_STYLE::TEXT_BRUSH_STYLE_SOLID)),
      m_reserved1(0),
      m_stopCount(1)
   {
      m_stops[0].color.r = 1.0f;
      m_stops[0].color.g = 1.0f;
      m_stops[0].color.b = 1.0f;
      m_stops[0].color.a = 1.0f;
   }

   size_t BRUSH_BUFFER::size() const
   {
      return static_cast<size_t>(m_stopCount);
   }

   TEXT_BRUSH_STYLE BRUSH_BUFFER::style() const
   {
      return static_cast<TEXT_BRUSH_STYLE>(m_style);
   }

   const D2D1_GRADIENT_STOP* BRUSH_BUFFER::stop(size_t idx) const
   {
      if (idx >= BRUSH_FORMAT_BUFFER_MAX_GRADIANT_STOPS)
      {
         throw std::out_of_range("idx is out of range.");
      }

      return m_stops + idx;
   }

   inline const D2D1_GRADIENT_STOP* BRUSH_BUFFER::stops() const noexcept
   {
      return nullptr;
   }

   const D2D1_LINEAR_GRADIENT_BRUSH_PROPERTIES* BRUSH_BUFFER::linear_properties() const
   {
      if (style() == TEXT_BRUSH_STYLE::TEXT_BRUSH_STYLE_GRADIENT_LINEAR)
      {
         return reinterpret_cast<const D2D1_LINEAR_GRADIENT_BRUSH_PROPERTIES*>(
            &m_properties);
      }
      else
      {
         throw std::bad_variant_access();
      }
   }

   const D2D1_RADIAL_GRADIENT_BRUSH_PROPERTIES* BRUSH_BUFFER::radial_properties() const
   {
      if (style() == TEXT_BRUSH_STYLE::TEXT_BRUSH_STYLE_GRADIENT_RADIAL)
      {
         return reinterpret_cast<const D2D1_RADIAL_GRADIENT_BRUSH_PROPERTIES*>(
            &m_properties);
      }
      else
      {
         throw std::bad_variant_access();
      }
   }
}