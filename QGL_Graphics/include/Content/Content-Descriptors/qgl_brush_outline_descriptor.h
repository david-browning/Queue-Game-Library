#pragma once
#include "include/qgl_graphics_include.h"
#include "include/Helpers/qgl_dx_constructors.h"
#include "include/Content/Content-Descriptors/qgl_vector_descriptor.h"

namespace qgl::graphics
{

   /*
    Based off https://learn.microsoft.com/en-us/windows/win32/api/d2d1/ne-d2d1-d2d1_cap_style
    */
   enum class brush_outline_cap_styles : uint8_t
   {
      /*
       A cap that does not extend past the last point of the line. Comparable 
       to cap used for objects other than lines.
       */
      none = D2D1_CAP_STYLE_FLAT,

      /*
       Half of a square that has a length equal to the line thickness.
       */
      square = D2D1_CAP_STYLE_SQUARE,

      /*
       A semicircle that has a diameter equal to the line thickness.
       */
      round = D2D1_CAP_STYLE_ROUND,

      /*
       An isosceles right triangle whose hypotenuse is equal in length to the 
       thickness of the line.
       */
      triangle = D2D1_CAP_STYLE_TRIANGLE,
   };

   /*
    Based off https://learn.microsoft.com/en-us/windows/win32/api/d2d1/ne-d2d1-d2d1_dash_style
    */
   enum class brush_outline_dash_styles : uint8_t
   {
      /*
       A solid line with no breaks.
       */
      solid = D2D1_DASH_STYLE_SOLID,

      /*
       A dash followed by a gap of equal length. The dash and the gap are each
       twice as long as the stroke thickness.
       */
      dash = D2D1_DASH_STYLE_DASH,

      /*
       A dot followed by a longer gap.
       */
      dot = D2D1_DASH_STYLE_DOT,

      /*
       A dash, followed by a gap, followed by a dot, followed by another gap.
       */
      dashdot = D2D1_DASH_STYLE_DASH_DOT,

      /*
       A dash, followed by a gap, followed by a dot, followed by another gap, 
       followed by another dot, followed by another gap.
       */
      dashdotdot = D2D1_DASH_STYLE_DASH_DOT_DOT,
   };

   /*
    Based off https://learn.microsoft.com/en-us/windows/win32/api/d2d1/ne-d2d1-d2d1_line_join
    */
   enum class brush_outline_join_styles : uint8_t
   {
      /*
       Regular angular vertices.
       */
      miter = D2D1_LINE_JOIN_MITER,

      /*
       Beveled vertices.
       */
      bevel = D2D1_LINE_JOIN_BEVEL,

      /*
       Rounded vertices.
       */
      round = D2D1_LINE_JOIN_ROUND,

      /*
       Regular angular vertices unless the join would extend beyond the miter 
       limit; otherwise, beveled vertices.
       */
      miterbevel = D2D1_LINE_JOIN_MITER_OR_BEVEL,
   };
}

namespace qgl::graphics::descriptors
{
   /*
    Describes how to draw an outline around geometry.
    */
#pragma pack(push, 1)
   struct brush_outline_descriptor final
   {
      constexpr brush_outline_descriptor()
      {

      }

      friend void swap(
         brush_outline_descriptor& l, 
         brush_outline_descriptor& r) noexcept
      {
         using std::swap;
         swap(l.outline, r.outline);
         swap(l.width, r.width);
         swap(l.dash_offset, r.dash_offset);
         swap(l.cap, r.cap);
         swap(l.style, r.style);
         swap(l.join, r.join);
         swap(l.reserved1, r.reserved1);
      }

      brush_outline_descriptor& operator=(brush_outline_descriptor r) noexcept
      {
         swap(*this, r);
         return *this;
      }

      /*
       Color of the outline.
       */
      vector_descriptor outline;


      /*
       A value that specifies an offset in the dash sequence. A positive dash 
       offset value shifts the dash pattern, in units of stroke width, toward
       the start of the stroked geometry. A negative dash offset value shifts
       the dash pattern, in units of stroke width, toward the end of the 
       stroked geometry.
       */
      qgl::math::rational<int32_t> dash_offset {0, 1};

      /*
       Width of the outline stroke.
       */
      qgl::math::rational<int32_t> width{ 1, 1 };

      /*
       How to cap the ends of the stroke.
       */
      brush_outline_cap_styles cap = brush_outline_cap_styles::none;

      /*
       How to draw the stroke.
       */
      brush_outline_dash_styles style = brush_outline_dash_styles::solid;

      /*
       How to join strokes.
       */
      brush_outline_join_styles join = brush_outline_join_styles::miter;

      uint8_t reserved1 = 0;
   };

#pragma pack(pop)
}