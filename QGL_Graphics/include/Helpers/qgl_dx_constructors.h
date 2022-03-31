#pragma once
#include "include/qgl_graphics_include.h"

namespace qgl::graphics
{
   inline D2D1_COLOR_F to_d2d_color(const std::array<float, 4>& a) noexcept
   {
      D2D1_COLOR_F ret;
      ret.r = a[0];
      ret.g = a[1];
      ret.b = a[2];
      ret.a = a[3];
      return ret;
   }

   template<typename T>
   inline D2D1_COLOR_F to_d2d_color(
      const std::array<qgl::math::rational<T>, 4>& a) noexcept
   {
      D2D1_COLOR_F ret;
      ret.r = a[0].operator float();
      ret.g = a[1].operator float();
      ret.b = a[2].operator float();
      ret.a = a[3].operator float();
      return ret;
   }

   template<typename T>
   inline D2D1_COLOR_F to_d2d_color(
      const fixed_buffer<qgl::math::rational<T>, 4>& a) noexcept
   {
      D2D1_COLOR_F ret;
      ret.r = a[0].operator float();
      ret.g = a[1].operator float();
      ret.b = a[2].operator float();
      ret.a = a[3].operator float();
      return ret;
   }

   template<typename T>
   inline DirectX::XMVECTOR XM_CALLCONV to_xmvector(
      const std::array<math::rational<T>, 4>& a) noexcept
   {
      return DirectX::XMVectorSet(
         a[0].operator float(),
         a[1].operator float(),
         a[2].operator float(),
         a[3].operator float());
   }
}