/*
 Contains specializations of comparison functions for types not defined by QGL.
 For example, this includes equality and inequality operators for DirectX 
 vectors and bounding volumes.
 */
#pragma once
#include "include/qgl_math_includes.h"

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
                            T tolerance) noexcept
   {
      return abs(expected - actual) <= tolerance;
   }

   /*
    Specialization of approx_equal for floats.
    */
   inline bool approx_equal(float expected,
                            float actual,
                            float tolerance = FLT_EPSILON) noexcept
   {
      return abs(expected - actual) <= tolerance;
   }

   /*
    Specialization of approx_equal for doubles.
    */
   inline bool approx_equal(double expected,
                            double actual,
                            double tolerance = DBL_EPSILON) noexcept
   {
      return abs(expected - actual) <= tolerance;
   }


   /////////Overload equality operators for DirectX CPU vectors.////////////////
   inline bool operator==(const DirectX::XMFLOAT2& a,
                          const DirectX::XMFLOAT2& b) noexcept
   {
      return approx_equal(a.x, b.x) &&
         approx_equal(a.y, b.y);
   }

   inline bool operator!=(const DirectX::XMFLOAT2& a,
                          const DirectX::XMFLOAT2& b) noexcept
   {
      return !(a == b);
   }

   inline bool operator==(const DirectX::XMFLOAT3& a,
                          const DirectX::XMFLOAT3& b) noexcept
   {
      return approx_equal(a.x, b.x) &&
         approx_equal(a.y, b.y) &&
         approx_equal(a.z, b.z);
   }

   inline bool operator!=(const DirectX::XMFLOAT3& a,
                          const DirectX::XMFLOAT3& b) noexcept
   {
      return !(a == b);
   }

   inline bool operator==(const DirectX::XMFLOAT4& a,
                          const DirectX::XMFLOAT4& b) noexcept
   {
      return approx_equal(a.x, b.x) &&
         approx_equal(a.y, b.y) &&
         approx_equal(a.z, b.z) &&
         approx_equal(a.w, b.w);
   }

   inline bool operator!=(const DirectX::XMFLOAT4& a,
                          const DirectX::XMFLOAT4& b) noexcept
   {
      return !(a == b);
   }

   /////////Overload equality operators for DirectX bounding volumes.///////////
   inline bool operator==(const DirectX::BoundingSphere& a,
                          const DirectX::BoundingSphere& b) noexcept
   {
      return a.Radius == b.Radius &&
         a.Center == b.Center;
   }

   inline bool operator!=(const DirectX::BoundingSphere& a,
                          const DirectX::BoundingSphere& b) noexcept
   {
      return !(a == b);
   }
}