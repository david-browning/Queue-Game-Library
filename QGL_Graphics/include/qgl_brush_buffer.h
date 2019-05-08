#pragma once
#include "qgl_graphics_include.h"

namespace qgl::graphics
{
   enum class TEXT_BRUSH_STYLE : uint8_t
   {
      TEXT_BRUSH_STYLE_SOLID,
      TEXT_BRUSH_STYLE_GRADIENT_LINEAR,
      TEXT_BRUSH_STYLE_GRADIENT_RADIAL,
   };

   static constexpr size_t BRUSH_FORMAT_BUFFER_MAX_GRADIANT_STOPS = 16;

   static constexpr size_t BRUSH_FORMAT_BUFFER_MAX_PROPRTY_SIZE =
      std::max<size_t>({ sizeof(D2D1_LINEAR_GRADIENT_BRUSH_PROPERTIES),
                         sizeof(D2D1_RADIAL_GRADIENT_BRUSH_PROPERTIES) });

   struct LIB_EXPORT BRUSH_BUFFER
   {
      public:
      BRUSH_BUFFER();

      BRUSH_BUFFER(const BRUSH_BUFFER& r);

      BRUSH_BUFFER(BRUSH_BUFFER&& r);

      ~BRUSH_BUFFER() noexcept = default;

      inline size_t count() const
      {
         return static_cast<size_t>(m_stopCount);
      }

      inline TEXT_BRUSH_STYLE style() const
      {
         return static_cast<TEXT_BRUSH_STYLE>(m_style);
      }

      inline const D2D1_GRADIENT_STOP& stop(size_t idx) const
      {
         if (idx >= BRUSH_FORMAT_BUFFER_MAX_GRADIANT_STOPS)
         {
            throw std::out_of_range("idx is out of range.");
         }

         return m_stops[idx];
      }

      inline const D2D1_GRADIENT_STOP* stops() const
      {
         return m_stops;
      }

      /*
       Throws a bad_variant_access exception if the style is not linear gradient.
       */
      const D2D1_LINEAR_GRADIENT_BRUSH_PROPERTIES& linear_properties();

      /*
       Throws a bad_variant_access exception if the style is not radial gradient.
       */
      const D2D1_RADIAL_GRADIENT_BRUSH_PROPERTIES& radial_properties();

      private:
      uint8_t m_style;
      uint8_t m_reserved1;
      uint16_t m_stopCount;

      uint8_t m_properties[BRUSH_FORMAT_BUFFER_MAX_PROPRTY_SIZE];

      D2D1_GRADIENT_STOP m_stops[BRUSH_FORMAT_BUFFER_MAX_GRADIANT_STOPS];
   };
}