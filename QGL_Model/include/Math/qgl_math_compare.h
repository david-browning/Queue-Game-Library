#pragma once
#include <cmath>
#include <limits>

namespace qgl::math
{
   /*
    Base case for finding the smallest element in a parameter packed list.
    */
   template<typename T>
   constexpr T smallest(T value)
   {
      return value;
   }

   /*
    Returns the smallest value in a parameter packed list.
    */
   template<typename T, typename... Tlist>
   constexpr T smallest(T value, Tlist... list)
   {
      T min = smallest<T>(list...);
      return value < min ? value : min;
   }

   /*
    Returns true if the difference between expected and actual is within the
    tolerance.
    */
   template<typename T>
   inline bool approx_equal(T expected,
                            T actual,
                            T tolerance)
   {
      return abs(expected - actual) < tolerance;
   }

   /*
    Specialization of approx_equal for floats.
    */
   inline bool approx_equal(float expected,
                            float actual,
                            float tolerance = FLT_EPSILON)
   {
      return abs(expected - actual) < tolerance;
   }

   /*
    Specialization of approx_equal for doubles.
    */
   inline bool approx_equal(double expected,
                            double actual,
                            double tolerance = DBL_EPSILON)
   {
      return abs(expected - actual) < tolerance;
   }
}