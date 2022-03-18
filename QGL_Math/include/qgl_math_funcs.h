#pragma once
#include "include/qgl_math_funcs.h"

namespace qgl::math
{
   template<typename T>
   constexpr T absolute_value(T x) noexcept
   {
      if (x < 0)
      {
         return -x;
      }

      return x;
   }

   /*
    Returns true if the difference between expected and actual is within the
    tolerance.
    */
   template<typename T>
   constexpr bool approx_equal(T expected,
      T actual,
      T tolerance) noexcept
   {
      return absolute_value(expected - actual) <= tolerance;
   }

   /*
    Specialization of approx_equal for floats.
    */
   constexpr bool approx_equal(float expected,
      float actual,
      float tolerance = FLT_EPSILON) noexcept
   {
      return absolute_value(expected - actual) <= tolerance;
   }

   /*
    Specialization of approx_equal for doubles.
    */
   constexpr bool approx_equal(double expected,
      double actual,
      double tolerance = DBL_EPSILON) noexcept
   {
      return absolute_value(expected - actual) <= tolerance;
   }

   /*
    Returns val rounded to the nearest.
    Returns val if it is already rounded to the nearest.
    Zero cannot be rounded up.
    */
   template<typename T>
   constexpr T round_up(T val, T nearest) noexcept
   {
      T remainder = val % nearest;
      return remainder == 0 ? val : val + nearest - remainder;
   }

   /*
    Specialization of round_up for floats.
    */
   template<>
   inline float round_up<float>(float val, float nearest) noexcept
   {
      auto remainder = fmod(val, nearest);
      return approx_equal(remainder, 0.0f) ? val : val + nearest - remainder;
   }

   /*
    Specialization of round_up for doubles.
    */
   template<>
   inline double round_up<double>(double val, double nearest) noexcept
   {
      auto remainder = fmod(val, nearest);
      return approx_equal(remainder, 0.0) ? val : val + nearest - remainder;
   }

   /*
    Calculates an integer exponent.
    */
   template<typename BaseT, typename ExponentT = typename BaseT>
   constexpr BaseT power(BaseT base, ExponentT exponent)
   {
      static_assert(std::is_unsigned<ExponentT>::value,
         "T must be unsigned. The exponent cannot be less than 0.");

      static_assert(std::is_integral<BaseT>::value,
         "The base must be an integer-like type.");

      static_assert(std::is_integral<ExponentT>::value,
         "The exponent must be an integer-like type.");

      return impl::power(base, exponent, static_cast<BaseT>(1));
   }

   /*
   Base case for finding the smallest element in a parameter packed list.
   */
   template<typename T>
   constexpr T smallest(T value) noexcept
   {
      return value;
   }

   /*
    Returns the smallest value in a parameter packed list.
    */
   template<typename T, typename... Tlist>
   constexpr T smallest(T value, Tlist... list) noexcept
   {
      T min = smallest<T>(list...);
      return value < min ? value : min;
   }
}