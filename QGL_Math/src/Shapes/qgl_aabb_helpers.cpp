#include "pch.h"
#include "include/Shapes/qgl_aabb_helpers.h"

namespace qgl::math
{
   void XM_CALLCONV aabb_from_minmax(DirectX::FXMVECTOR mins, 
                                     DirectX::FXMVECTOR maxs, 
                                     DirectX::BoundingBox* out_p)
   {
      using namespace DirectX;
      //Calculate the center:
      //[x1 + x2, y1 + y2, z1 + z2] / 2
      auto center = (mins + maxs) / 2.0f;

      //Calculate the extents:
      //[|x1| + |x2|, |y1| + |y2|, |z1| + |z2|] / 2
      auto extents = (XMVectorAbs(mins) + XMVectorAbs(maxs)) / 2.0f;

      //Store the center and extents in XMFLOAT3 so we can create the AABB.
      XMFLOAT3 aabbCenter;
      XMFLOAT3 aabbExtents;
      XMStoreFloat3(&aabbCenter, center);
      XMStoreFloat3(&aabbExtents, extents);

      *out_p = DirectX::BoundingBox(aabbCenter, aabbExtents);
   }

   void XM_CALLCONV aabb_verts(const DirectX::BoundingBox* aabb,
                               DirectX::XMVECTOR verts[VERTS_PER_BOX])
   {
      using namespace DirectX;
      auto center = XMLoadFloat3(&aabb->Center);
      auto extents = XMLoadFloat3(&aabb->Extents);

      verts[0] = center + extents;
      verts[1] = center + extents * XMVectorSet(1.0f, -1.0f, 1.0f, 0.0f);
      verts[2] = center + extents * XMVectorSet(1.0f, 1.0f, -1.0f, 0.0f);
      verts[3] = center + extents * XMVectorSet(1.0f, -1.0f, -1.0f, 0.0f);
      verts[4] = center - extents;
      verts[5] = center + extents * XMVectorSet(-1.0f, 1.0f, 1.0f, 0.0f);
      verts[6] = center + extents * XMVectorSet(-1.0f, -1.0f, -1.0f, 0.0f);
      verts[7] = center + extents * XMVectorSet(-1.0f, 1.0, -1.0f, 0.0f);
   }
}