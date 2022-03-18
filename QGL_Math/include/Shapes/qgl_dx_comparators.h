#pragma once
#include "include/qgl_math_includes.h"

namespace qgl::math
{
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