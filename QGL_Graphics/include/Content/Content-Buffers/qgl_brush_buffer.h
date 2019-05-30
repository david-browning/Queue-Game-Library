#pragma once
#include "include/qgl_graphics_include.h"

namespace qgl::graphics::content::buffers
{
   enum class TEXT_BRUSH_STYLE : uint8_t
   {
      TEXT_BRUSH_STYLE_SOLID,
      TEXT_BRUSH_STYLE_GRADIENT_LINEAR,
      TEXT_BRUSH_STYLE_GRADIENT_RADIAL,
   };

   static constexpr size_t BRUSH_FORMAT_BUFFER_MAX_GRADIANT_STOPS = 16;

   struct QGL_GRAPHICS_API BRUSH_BUFFER
   {
      public:
      /*
       Default constructor.
       Creates a white solid color brush with 1 stop.
       */
      BRUSH_BUFFER();

      /*
       Copy constructor.
       */
      BRUSH_BUFFER(const BRUSH_BUFFER& r) = default;

      /*
       Move constructor.
       */
      BRUSH_BUFFER(BRUSH_BUFFER&& r) = default;

      /*
       Destructor. Do not mark as virtual.
       */
      ~BRUSH_BUFFER() noexcept = default;

      /*
       Returns the number of stops for gradient brush,
       */
      size_t size() const;

      /*
       Returns the style of brush.
       */
      TEXT_BRUSH_STYLE style() const;

      /*
       Returns the idx'th stop for a gradient brush.
       Throws std::out_of_range if the index is greater than the number of
       gradient stops.
       */
      const D2D1_GRADIENT_STOP* stop(size_t idx) const;

      /*
       Returns a const pointer to the array of gradient stops.
       D2D1_GRADIENT_STOP contain the position and color of a gradient stop.
       */
      inline const D2D1_GRADIENT_STOP* stops() const noexcept;

      /*
       Contains the starting point and endpoint of the gradient axis for an
       ID2D1LinearGradientBrush.
       Throws a bad_variant_access exception if the style is not linear
       gradient.
       */
      const D2D1_LINEAR_GRADIENT_BRUSH_PROPERTIES* linear_properties() const;

      /*
       Contains the gradient origin offset and the size and position of the
       gradient ellipse for an ID2D1RadialGradientBrush.
       Throws a bad_variant_access exception if the style is not radial
       gradient.
       */
      const D2D1_RADIAL_GRADIENT_BRUSH_PROPERTIES* radial_properties() const;

      /*
       Swaps the contents of l with the contents of r.
       */
      friend void swap(BRUSH_BUFFER& l, BRUSH_BUFFER& r) noexcept
      {
         using std::swap;
         swap(l.m_properties, r.m_properties);
         swap(l.m_reserved1, r.m_properties);
         swap(l.m_stopCount, r.m_stopCount);
         swap(l.m_stops, r.m_stops);
         swap(l.m_style, r.m_style);
      }

      /*
       Assignment operator.
       */
      BRUSH_BUFFER& operator=(BRUSH_BUFFER r) noexcept
      {
         swap(*this, r);
         return *this;
      }

      /*
       Equality operator.
       */
      friend bool operator==(const BRUSH_BUFFER& l,
                             const BRUSH_BUFFER& r) noexcept
      {
         bool intTypesEqual = (l.m_reserved1 == r.m_reserved1 &&
                               l.m_stopCount == r.m_stopCount &&
                               l.m_style == l.m_style);

         return intTypesEqual &&
            (0 == MemoryCompare(l.m_stops, 
                                r.m_stops, 
                                l.size())) &&
            (0 == MemoryCompare(l.m_properties, 
                                r.m_properties, 
                                BRUSH_FORMAT_BUFFER_MAX_PROPRTY_SIZE));
      }

      private:

      static constexpr size_t BRUSH_FORMAT_BUFFER_MAX_PROPRTY_SIZE =
         std::max<size_t>({ sizeof(D2D1_LINEAR_GRADIENT_BRUSH_PROPERTIES),
                            sizeof(D2D1_RADIAL_GRADIENT_BRUSH_PROPERTIES) });

      /*
       Brush style. It can be solid, linear gradient, or radial gradient.
       */
      uint8_t m_style;

      /*
       Reserved.
       */
      uint8_t m_reserved1;

      /*
       Number of gradient stops and properties.
       */
      uint16_t m_stopCount;

      /*
       Byte buffer.
       */
      uint8_t m_properties[BRUSH_FORMAT_BUFFER_MAX_PROPRTY_SIZE];

      D2D1_GRADIENT_STOP m_stops[BRUSH_FORMAT_BUFFER_MAX_GRADIANT_STOPS];
   };
}