#pragma once
#include "include/qgl_math_includes.h"

namespace qgl::math
{
   /*
    PI
    */
   template<typename T>
   constexpr T const_pi = T(M_PI);

   /*
    PI / 2
    */
   template<typename T>
   constexpr T const_pi_2 = T(M_PI_2);

   /*
    PI / 4
    */
   template<typename T>
   constexpr T const_pi_4 = T(M_PI_4);

   /*
    e
    */
   template<typename T>
   constexpr T const_e = T(M_E);

   /*
    log2(e)
    */
   template<typename T>
   constexpr T const_log2e = T(M_LOG2E);

   /*
    log10(e)
    */
   template<typename T>
   constexpr T const_log10e = T(M_LOG10E);

   /*
    sqrt(2)
    */
   template<typename T>
   constexpr T const_sqrt2 = T(M_SQRT2);
}