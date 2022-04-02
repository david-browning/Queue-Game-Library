#pragma once
#include "include/qgl_physics_include.h"

namespace qgl::physics
{
   /*
    Calculates an AABB for the set of vertices. The AABB  will most-likely not
    be centered at the origin.
    This operation is slow and should only be done once. Calculate the AABB at
    content compile time or content load time.
    Dereferencing the ForwardIterator should yield an XMFLOAT3.
    Complexity: O(Number of vertices)
    */
   template<class ForwardIterator>
   DirectX::BoundingBox bound_aabb(ForwardIterator first, ForwardIterator last)
   {
      using namespace DirectX;
      static_assert(std::numeric_limits<float>::is_iec559,
                    L"The function requires IEEE 754.");
      static_assert(
         std::is_same<std::remove_reference<decltype(*first)>::type,
            XMFLOAT3>::value,
         "ForwardIterator does not point to an XMFLOAT3.");

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

      //For each vertex in the model:
      while (first != last)
      {
         auto v = XMLoadFloat3(&*first);

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

         first++;
      }

      auto mins = XMVectorSet(xMin, yMin, zMin, 0.0f);
      auto maxs = XMVectorSet(xMax, yMax, zMax, 0.0f);

      BoundingBox ret;
      math::aabb_from_minmax(mins, maxs, &ret);
      return ret;
   }

   /*
    Calculates a bounding sphere for the set of vertices. The sphere
    will most-likely not be centered at the origin.
    This operation is slow and should only be done once. Calculate the bounding
    sphere at content compile time or content load time.
    Dereferencing the ForwardIterator should yield an XMFLOAT3.
    Complexity: O(Number of vertices)
    */
   template<class ForwardIterator>
   DirectX::BoundingSphere bound_sphere(ForwardIterator first,
                                        ForwardIterator last)
   {
      using namespace DirectX;
      static_assert(
         std::is_same<std::remove_reference<decltype(*first)>::type,
            XMFLOAT3>::value,
         "ForwardIterator does not point to an XMFLOAT3.");

      //Accumulate all vertex positions. Used to calculate the sphere center.
      XMVECTOR accum = XMVectorZero();
      //Count the number of vertices as we go.
      size_t numVerts = 0;
      //Keep track of the vertex farthest away from the origin.
      XMVECTOR largestVert = DirectX::operator*(-1.0f, XMVectorSplatInfinity());
      auto largestVertMag = -1.0f * std::numeric_limits<float>::infinity();

      //For each vertex in the model:
      while (first != last)
      {
         auto v = XMLoadFloat3(&*first);

         //Accumulate the center and counter.
         accum += v;
         numVerts++;

         //Check if this vertex has a greater magnitude. 
         //Use the length squared because it is cheaper to compute.
         //Once we find the vector with the greatest magnitude, we'll use a 
         //more accurate length measurement.
         auto lSquared = XMVectorGetX(XMVector3LengthSq(v));
         if (lSquared > largestVertMag)
         {
            largestVertMag = lSquared;
            largestVert = v;
         }

         first++;
      }

      auto center = accum / numVerts;
      auto radius = XMVector3Length(
         XMVectorAbs(largestVert) - XMVectorAbs(center));

      XMFLOAT3 sphereCenter;
      XMStoreFloat3(&sphereCenter, center);
      return BoundingSphere(sphereCenter, XMVectorGetX(radius));
   }

   /*
    Transforms a bounding box and returns an AABB large enough to hold the
    transformed bounding box.
    */
   extern "C" QGL_PHYSICS_API void XM_CALLCONV resize_aabb(
      const DirectX::BoundingBox* aabb_p, 
      DirectX::FXMMATRIX xform, 
      DirectX::BoundingBox* out_p);

}