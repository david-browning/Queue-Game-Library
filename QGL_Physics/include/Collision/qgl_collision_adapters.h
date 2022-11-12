#pragma once
#include "include/qgl_physics_include.h"

namespace qgl::physics
{
   /*
    Returns true if two bounding spheres intersect.
    */
   class sphere_intersects
   {
      public:
      bool operator()(const DirectX::BoundingSphere& s1,
                      const DirectX::BoundingSphere& s2) const noexcept
      {
         return s1.Intersects(s2);
      }
   };

   /*
    Returns the volume of the given bounding sphere.
    */
   class sphere_volume
   {
      public:
      float operator()(const DirectX::BoundingSphere& s) const noexcept
      {
         auto r3 = std::pow(s.Radius, 3.0f);
         return r3 * (4.0f / 3.0f) * math::const_pi<float>;
      }
   };

   /*
    Creates a bounding sphere that contains the two bounding spheres.
    */
   class sphere_union
   {
      public:
      DirectX::BoundingSphere operator()(
         const DirectX::BoundingSphere& s1,
         const DirectX::BoundingSphere& s2) const noexcept
      {
         using namespace DirectX;
         XMVECTOR v1Center = XMLoadFloat3(&s1.Center);
         XMVECTOR v2Center = XMLoadFloat3(&s2.Center);
         auto center = (v1Center + v2Center) / 2.0f;

         auto distanceBetween = v1Center - v2Center;
         auto diameter = s1.Radius + s2.Radius +
            XMVectorGetByIndex(XMVector3Length(distanceBetween), 0);
         auto r = diameter / 2.0f;

         XMFLOAT3 f3Center;
         XMStoreFloat3(&f3Center, center);
         return BoundingSphere(f3Center, r);
      }
   };

   class aabb_intersects
   {
      public:
      bool operator()(const DirectX::BoundingBox& b1,
                      const DirectX::BoundingBox& b2) const noexcept
      {
         return b1.Intersects(b2);
      }
   };

   class aabb_volume
   {
      public:
      float operator()(const DirectX::BoundingBox& b) const noexcept
      {
         return b.Extents.x * b.Extents.y * b.Extents.z * 2.0f;
      }
   };

   class aabb_union
   {
      public:
      DirectX::BoundingBox operator()(
         const DirectX::BoundingBox& b1,
         const DirectX::BoundingBox& b2) const noexcept
      {
         using namespace DirectX;
         XMVECTOR v1Center = XMLoadFloat3(&b1.Center);
         XMVECTOR v2Center = XMLoadFloat3(&b2.Center);
         auto center = (v1Center + v2Center) / 2.0f;
         XMFLOAT3 f3Center;
         XMStoreFloat3(&f3Center, center);

         XMFLOAT3 extents;
         extents.x = std::max<float>(b1.Extents.x, b2.Extents.x);
         extents.y = std::max<float>(b1.Extents.y, b2.Extents.y);
         extents.z = std::max<float>(b1.Extents.z, b2.Extents.z);

         return BoundingBox{ f3Center, extents };
      }
   };
}