#pragma once
#include "include/qgl_math_includes.h"
#include "include/qgl_math_funcs.h"
#include <DirectXMath.h>
#include <DirectXCollision.h>

namespace qgl::math
{
   /////////Overload equality operators for DirectX CPU vectors.////////////////
   inline bool operator==(const DirectX::XMFLOAT2& a,
      const DirectX::XMFLOAT2& b) noexcept
   {
      return approx_equal(a.x, b.x, FLT_EPSILON) &&
         approx_equal(a.y, b.y, FLT_EPSILON);
   }

   inline bool operator!=(const DirectX::XMFLOAT2& a,
      const DirectX::XMFLOAT2& b) noexcept
   {
      return !(a == b);
   }

   inline bool operator==(const DirectX::XMFLOAT3& a,
      const DirectX::XMFLOAT3& b) noexcept
   {
      return approx_equal(a.x, b.x, FLT_EPSILON) &&
         approx_equal(a.y, b.y, FLT_EPSILON) &&
         approx_equal(a.z, b.z, FLT_EPSILON);
   }

   inline bool operator!=(const DirectX::XMFLOAT3& a,
      const DirectX::XMFLOAT3& b) noexcept
   {
      return !(a == b);
   }

   inline bool operator==(const DirectX::XMFLOAT4& a,
      const DirectX::XMFLOAT4& b) noexcept
   {
      return approx_equal(a.x, b.x, FLT_EPSILON) &&
         approx_equal(a.y, b.y, FLT_EPSILON) &&
         approx_equal(a.z, b.z, FLT_EPSILON) &&
         approx_equal(a.w, b.w, FLT_EPSILON);
   }

   inline bool operator!=(const DirectX::XMFLOAT4& a,
      const DirectX::XMFLOAT4& b) noexcept
   {
      return !(a == b);
   }

   ////////////Overload equality operators for DirectX CPU matrices.////////////
   inline bool operator==(const DirectX::XMFLOAT4X4& a,
                          const DirectX::XMFLOAT4X4 b) noexcept
   {
      bool ret = true;
      for (size_t r = 0; r < 4; r++)
      {
         for (size_t c = 0; c < 4; c++)
         {
            ret &= (a(r, c) == b(r, c));
         }
      }

      return ret;
   }

   inline bool operator!=(const DirectX::XMFLOAT4X4& a,
                          const DirectX::XMFLOAT4X4 b) noexcept
   {
      return !(a == b);
   }

   //////////Overload equality operators for DirectX hardware vectors.//////////
   inline bool XM_CALLCONV operator==(
      DirectX::FXMVECTOR a,
      DirectX::FXMVECTOR b) noexcept
   {
      auto cmp = DirectX::XMVectorEqual(a, b);
      return DirectX::XMVectorGetX(cmp) != 0 &&
         DirectX::XMVectorGetY(cmp) != 0 &&
         DirectX::XMVectorGetZ(cmp) != 0 &&
         DirectX::XMVectorGetW(cmp) != 0;
   }

   inline bool XM_CALLCONV operator!=(
      DirectX::FXMVECTOR a,
      DirectX::FXMVECTOR b) noexcept
   {
      return !(a == b);
   }

   //////////Overload equality operators for DirectX hardware matrices./////////
   inline bool XM_CALLCONV operator==(
      DirectX::FXMMATRIX m1,
      DirectX::CXMMATRIX m2) noexcept
   {
      DirectX::XMFLOAT4X4 m1F;
      DirectX::XMFLOAT4X4 m2F;

      DirectX::XMStoreFloat4x4(&m1F, m1);
      DirectX::XMStoreFloat4x4(&m2F, m2);

      return m1F == m2F;
   }

   inline bool XM_CALLCONV operator!=(
      DirectX::FXMMATRIX m1,
      DirectX::CXMMATRIX m2) noexcept
   {
      return !(m1 == m2);
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