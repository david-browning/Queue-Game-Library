#pragma once
#include "include/qgl_math_includes.h"

namespace qgl::math
{
   /*
    Returns val rounded to the nearest.
    Returns val if it is already rounded to the nearest.
    Zero cannot be rounded up.
    */
   template<typename T>
   constexpr T round_up(T val, T nearest)
   {
      T remainder = val % nearest;
      return remainder == 0 ? val : val + nearest - remainder;
   }

   /*
    Specialization of round_up for floats.
    */
   template<>
   inline float round_up<float>(float val, float nearest)
   {
      auto remainder = fmod(val, nearest);
      return approx_equal(remainder, 0.0f) ? val : val + nearest - remainder;
   }

   /*
    Specialization of round_up for doubles.
    */
   template<>
   inline double round_up<double>(double val, double nearest)
   {
      auto remainder = fmod(val, nearest);
      return approx_equal(remainder, 0.0) ? val : val + nearest - remainder;
   }
}