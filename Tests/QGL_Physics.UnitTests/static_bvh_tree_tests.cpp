#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace qgl;
using namespace qgl::math;
using namespace qgl::physics;

namespace QGL_Physics_UnitTests
{
   TEST_CLASS(StaticBVHTreeTests)
   {
      public:
      TEST_METHOD(CreateRootTree)
      {
         std::vector<DirectX::BoundingSphere> spheres{ ZERO_SPHERE };
         static_bvh_tree<DirectX::BoundingSphere,
            sphere_intersects,
            sphere_volume,
            sphere_union> tree{ spheres.begin(), spheres.end() };

         //The inserted sphere should collide with itself.
         tree.intersects(ZERO_SPHERE, intersection_callback(1));

         //Test a sphere that doesn't intersect
         tree.intersects(ZERO_SPHERE_NO_INTERSECT, intersection_callback(0));

         //Test a sphere that does intersect
         tree.intersects(ZERO_SPHERE_INTERSECTS, intersection_callback(1));
      }

      TEST_METHOD(InsertTwoDisjointSpheres)
      {
         //Insert two spheres that are not colliding.
         DirectX::BoundingSphere sphere1
         {
            DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f),
            0.49f
         };

         DirectX::BoundingSphere sphere2
         {
            DirectX::XMFLOAT3(-1.0f, 0.0f, 0.0f),
            0.49f
         };

         std::vector<DirectX::BoundingSphere> spheres{ sphere1, sphere2 };

         static_bvh_tree<DirectX::BoundingSphere,
            sphere_intersects,
            sphere_volume,
            sphere_union> bvh{ spheres.begin(), spheres.end() };

         //Test with something that does not intersect.
         DirectX::BoundingSphere notIntersectingSphere
         {
            DirectX::XMFLOAT3(-4.0f, 0.0f, 0.0f),
            0.5f
         };

         bvh.intersects(notIntersectingSphere, intersection_callback(0));

         //Test with something that intersects both spheres.
         DirectX::BoundingSphere intersectingBothSphere
         {
            DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f),
            2.0f
         };
         bvh.intersects(intersectingBothSphere, intersection_callback(2));

         //Test something that intersects 1 sphere.
         DirectX::BoundingSphere intersectingSphere1
         {
            DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f),
            0.25f
         };
         bvh.intersects(intersectingSphere1, intersection_callback(1));

         //Test something that intersects the other sphere.
         DirectX::BoundingSphere intersectingSphere2
         {
            DirectX::XMFLOAT3(-1.0f, 0.0f, 0.0f),
            0.25f
         };
         bvh.intersects(intersectingSphere2, intersection_callback(1));
      }

      private:
      class intersection_callback
      {
         public:
         /*
          Pass in the number of times we expect the callback to be raised.
          */
         intersection_callback(int expectingCalls) :
            m_expecting(expectingCalls)
         {

         }

         bool operator()(const DirectX::BoundingSphere& v)
         {
            if (m_expecting <= 0)
            {
               Assert::Fail(L"The intersection callback should not be called.");
            }

            m_expecting--;
            return true;
         }

         private:
         int m_expecting;
      };

      static constexpr DirectX::BoundingSphere ZERO_SPHERE
      {
         DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f),
         1.0f
      };

      static constexpr DirectX::BoundingSphere ZERO_SPHERE_NO_INTERSECT
      {
         DirectX::XMFLOAT3(2.0f, 0.0f, 0.0f),
         0.99f
      };

      static constexpr DirectX::BoundingSphere ZERO_SPHERE_INTERSECTS
      {
         DirectX::XMFLOAT3(0.5f, 0.5f, 0.5f),
         1.0f
      };
   };
}