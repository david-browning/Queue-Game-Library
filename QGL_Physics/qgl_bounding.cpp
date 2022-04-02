#include "pch.h"
#include "include/Collision/qgl_bounding.h"

namespace qgl::physics
{
   void XM_CALLCONV resize_aabb(const DirectX::BoundingBox* aabb,
                               DirectX::FXMMATRIX xform, 
                               DirectX::BoundingBox* out_p)
   {
      using namespace DirectX;
      //Set up the plane normals.
      static const auto xyN = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
      static const auto xzN = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
      static const auto yzN = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);

      //Keep track of the minimum and maximum distance for the 6 planes.
      auto xMin = std::numeric_limits<float>::infinity();
      auto xMax = -std::numeric_limits<float>::infinity();
      auto yMin = std::numeric_limits<float>::infinity();
      auto yMax = -std::numeric_limits<float>::infinity();
      auto zMin = std::numeric_limits<float>::infinity();
      auto zMax = -std::numeric_limits<float>::infinity();

      //Calculate the 8 vertices in the AABB and store them in an array.
      DirectX::XMVECTOR verts[math::VERTS_PER_BOX];
      math::aabb_verts(aabb, verts);

      //For each transformed vertex:
      for (size_t i = 0; i < math::VERTS_PER_BOX; i++)
      {
         //Apply the transform to the vertices.
         auto v = DirectX::XMVector3Transform(verts[i],
                                              xform);

         //Calculate the plane equation for the vertex.
         //Do this 6 times: For +x, -x, +y, -y, +z, -z
         auto xPPlane = XMPlaneFromPointNormal(v, yzN);
         xMax = std::max(xMax, DirectX::XMVectorGetW(xPPlane));

         auto xNPlane = XMPlaneFromPointNormal(v, -yzN);
         xMin = std::min(xMin, DirectX::XMVectorGetW(xNPlane));

         auto yPPlane = XMPlaneFromPointNormal(v, xzN);
         yMax = std::max(yMax, DirectX::XMVectorGetW(yPPlane));

         auto yNPlane = XMPlaneFromPointNormal(v, -xzN);
         yMin = std::min(yMin, DirectX::XMVectorGetW(yNPlane));

         auto zPPlane = XMPlaneFromPointNormal(v, xyN);
         zMax = std::max(zMax, DirectX::XMVectorGetW(zPPlane));

         auto zNPlane = XMPlaneFromPointNormal(v, -xyN);
         zMin = std::min(zMin, DirectX::XMVectorGetW(zNPlane));
      }

      auto mins = XMVectorSet(xMin, yMin, zMin, 0.0f);
      auto maxs = XMVectorSet(xMax, yMax, zMax, 0.0f);

      math::aabb_from_minmax(mins, maxs, out_p);
   }
}