#pragma once
#include "include/qgl_graphics_include.h"
#include "include/Helpers/qgl_dx_constructors.h"

namespace qgl::graphics
{
   enum class brush_styles : uint8_t
   {
      solid = 0,
      gradient_linear = 1,
      gradient_radial = 2,
   };
}

namespace qgl::graphics::descriptors
{
#pragma pack(push, 1)
   struct brush_stop final
   {
      /*
       Everything defaults to zero.
       */
      constexpr brush_stop()
      {
      }

      friend void swap(brush_stop& l, brush_stop& r) noexcept
      {
         using std::swap;
         swap(l.position, r.position);
         swap(l.color, r.color);
      }

      brush_stop& operator=(brush_stop r) noexcept
      {
         swap(*this, r);
         return *this;
      }

      /*
       A value that indicates the relative position of the gradient stop in
       the brush. This value must be in the [0.0f, 1.0f] range if the gradient
       stop is to be seen explicitly.
       */
      qgl::math::rational<uint32_t> position;

      /*
       The color of the gradient stop. Each element corisponds to the
       R, G, B, A values respectivly.
       */
      fixed_buffer<qgl::math::rational<uint32_t>, 4> color;
   };
#pragma pack(pop)

#pragma pack(push, 1)
   struct brush_descriptor final
   {
      private:
      static constexpr size_t BRUSH_DESCRIPTOR_MAX_PROPRTY_SIZE =
         std::max<size_t>({ sizeof(D2D1_LINEAR_GRADIENT_BRUSH_PROPERTIES),
                            sizeof(D2D1_RADIAL_GRADIENT_BRUSH_PROPERTIES) });
      static constexpr size_t BRUSH_DESCRIPTOR_MAX_GRADIANT_STOPS = 16;

      static constexpr std::array<uint8_t, BRUSH_DESCRIPTOR_MAX_PROPRTY_SIZE>
         EMPTY_PROPS{ 0 };

      public:
      constexpr brush_descriptor()
      {

      }

      friend void swap(brush_descriptor& l, brush_descriptor& r) noexcept
      {
         using std::swap;
         swap(l.style, r.style);
         swap(l.stop_count, r.stop_count);
         swap(l.reserved, r.reserved);
         swap(l.properties, r.properties);
         swap(l.stops, r.stops);
      }

      brush_descriptor& operator=(brush_descriptor r) noexcept
      {
         swap(*this, r);
         return *this;
      }
      
      /*
       This is a block of bytes that represent either a
       D2D1_LINEAR_GRADIENT_BRUSH_PROPERTIES or
       D2D1_RADIAL_GRADIENT_BRUSH_PROPERTIES.
       */
      fixed_buffer<uint8_t, BRUSH_DESCRIPTOR_MAX_PROPRTY_SIZE> properties;

      fixed_buffer<brush_stop, BRUSH_DESCRIPTOR_MAX_GRADIANT_STOPS> stops;

      /*
       Brush style. It can be solid, linear gradient, or radial gradient.
       */
      brush_styles style = brush_styles::solid;

      /*
       Number of gradient stops and properties.
       */
      uint8_t stop_count = 1;

      /*
       Reserved
       */
      uint16_t reserved = 0;
   };
#pragma pack(pop)
}

namespace qgl::graphics
{
   inline D2D1_GRADIENT_STOP to_d2d_stop(
      const descriptors::brush_stop& stop) noexcept
   {
      D2D1_GRADIENT_STOP ret;
      ret.color = to_d2d_color(stop.color);
      ret.position = stop.position.operator float();
      return ret;
   }

   template<class StopIt>
   inline std::vector<D2D1_GRADIENT_STOP> to_d2d_stop(
      StopIt first, StopIt last)
   {
      std::vector<D2D1_GRADIENT_STOP> ret;
      while (first != last)
      {
         D2D1_GRADIENT_STOP grad;
         grad.color = to_d2d_color(first->color);
         grad.position = first->position.operator float();
         ret.push_back(std::move(grad));
         first++;
      }

      return ret;
   }

   template<size_t n>
   inline std::vector<D2D1_GRADIENT_STOP> to_d2d_stop(
      const fixed_buffer<descriptors::brush_stop, n>& stops)
   {
      std::vector<D2D1_GRADIENT_STOP> ret;
      for (size_t i = 0; i < n; i++)
      {
         D2D1_GRADIENT_STOP grad;
         grad.color = to_d2d_color(first->color);
         grad.position = first->position.operator float();
         ret.push_back(std::move(grad));
      }

      return ret;
   }
}