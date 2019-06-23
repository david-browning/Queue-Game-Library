#pragma once
#include "include/qgl_math_includes.h"

namespace qgl::math
{
   /*
    Returns log base b of n.
    */
   template<typename T>
   constexpr T logbn(const T b, const T n)
   {
      return (n > b - 1) ? 1 + logbn(n / b, b) : 0;
   }

   /*
    Returns val rounded to the nearest.
    */
   template<typename T>
   constexpr T round_up(const T val, const T nearest)
   {
      T remainder = val % nearest;
      return remainder == 0 ? val : val + nearest - remainder;
   }
}