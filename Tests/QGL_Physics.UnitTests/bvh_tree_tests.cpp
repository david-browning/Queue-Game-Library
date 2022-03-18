#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace qgl;
using namespace qgl::math;
using namespace qgl::physics;

namespace QGL_Physics_UnitTests
{
   TEST_CLASS(BVHTreeTests)
   {
      public:
      TEST_METHOD(CreateEmptyTree)
      {
         bvh_tree<DirectX::BoundingSphere,
            test_intersects,
            test_volume,
            test_bound> emptyTree;

         Assert::IsTrue(emptyTree.begin() == emptyTree.end(),
                        L"The begin and end iterators should be equal.");

         DirectX::BoundingSphere testSphere
         {
            DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f),
            1.0f
         };

         //Make sure that going through the empty tree doesn't cause any 
         //problems.
         emptyTree.intersects(testSphere, intersection_callback(0));
      }

      TEST_METHOD(CreateRootTree)
      {
         bvh_tree<DirectX::BoundingSphere,
            test_intersects,
            test_volume,
            test_bound> rootTree;

         DirectX::BoundingSphere testSphere
         {
            DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f),
            1.0f
         };

         auto it = rootTree.insert(testSphere);
         Assert::IsTrue(it != rootTree.end(),
                        L"Iterator should not point to end after insertion.");      

         //height of the root should be 0.
         Assert::AreEqual(static_cast<size_t>(0), it->height(),
                          L"Root height should be 0.");

         //The inserted sphere should collide with itself.
         rootTree.intersects(testSphere, intersection_callback(1));

         //Test a sphere that doesn't intersect
         DirectX::BoundingSphere notIntersectingSphere
         {
            DirectX::XMFLOAT3(2.0f, 0.0f, 0.0f),
            0.99f
         };

         rootTree.intersects(notIntersectingSphere, intersection_callback(0));

         //Test a sphere that does intersect
         DirectX::BoundingSphere intersectingSphere
         {
            DirectX::XMFLOAT3(0.5f, 0.5f, 0.5f),
            1.0f
         };

         rootTree.intersects(intersectingSphere, intersection_callback(1));
      }

      TEST_METHOD(InsertTwoDisjointSpheres)
      {
         ////Insert two spheres that are not colliding.
         //DirectX::BoundingSphere sphere1
         //{
         //   DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f),
         //   0.49f
         //};

         //DirectX::BoundingSphere sphere2
         //{
         //   DirectX::XMFLOAT3(-1.0f, 0.0f, 0.0f),
         //   0.49f
         //};

         //bvh_tree<DirectX::BoundingSphere,
         //   test_intersects,
         //   test_volume,
         //   test_bound> bvh;
         //bvh.insert(sphere1);
         //bvh.insert(sphere2);

         ////height of the root should be 1.
         //Assert::AreEqual(static_cast<size_t>(1), bvh.begin()->height(),
         //                 L"Tree height should be 1.");

         ////Test with something that does not intersect.
         //DirectX::BoundingSphere notIntersectingSphere
         //{
         //   DirectX::XMFLOAT3(-4.0f, 0.0f, 0.0f),
         //   0.5f
         //};

         //bvh.intersects(notIntersectingSphere, intersection_callback(0));

         ////Test with something that intersects both spheres.
         //DirectX::BoundingSphere intersectingBothSphere
         //{
         //   DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f),
         //   2.0f
         //};
         //bvh.intersects(intersectingBothSphere, intersection_callback(2));

         ////Test something that intersects 1 sphere.
         //DirectX::BoundingSphere intersectingSphere1
         //{
         //   DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f),
         //   0.25f
         //};
         //bvh.intersects(intersectingSphere1, intersection_callback(1));

         ////Test something that intersects the other sphere.
         //DirectX::BoundingSphere intersectingSphere2
         //{
         //   DirectX::XMFLOAT3(-1.0f, 0.0f, 0.0f),
         //   0.25f
         //};
         //bvh.intersects(intersectingSphere2, intersection_callback(1));
         //return;
      }

      /*
       Create a bvh that will be right-right heavy after inserting 4 spheres.
       */
      TEST_METHOD(InsertUnbalancedRightHeavy)
      {
         //DirectX::BoundingSphere sphereA
         //{
         //   DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f),
         //   1.0f
         //};

         //DirectX::BoundingSphere sphereB
         //{
         //   DirectX::XMFLOAT3(3.0f, 0.0f, 0.0f),
         //   1.0f
         //};

         //DirectX::BoundingSphere sphereC
         //{
         //   DirectX::XMFLOAT3(5.0f, 0.0f, 0.0f),
         //   1.0f
         //};

         //DirectX::BoundingSphere sphereD
         //{
         //   DirectX::XMFLOAT3(7.0f, 0.0f, 0.0f),
         //   1.0f
         //};

         //bvh_tree<DirectX::BoundingSphere,
         //   test_intersects,
         //   test_volume,
         //   test_bound> bvh;
         //bvh.insert(sphereA);
         //bvh.insert(sphereB);
         //bvh.insert(sphereC); 
         //bvh.insert(sphereD);

         ////Verify that the inserted spheres are in the correct spots in the bvh.
         //auto rootIt = bvh.begin();
         //Assert::AreEqual(static_cast<size_t>(2), rootIt->height(), 
         //                 L"The root height is not correct.");

         ////A: index 3
         ////B: index 4
         ////C: index 5
         ////D: index 6

         //Assert::IsTrue((rootIt + 3)->volume() == sphereA,
         //               L"Index 3 does not contain sphereA.");

         //Assert::IsTrue((rootIt + 4)->volume() == sphereB,
         //               L"Index 4 does not contain sphereB.");

         //Assert::IsTrue((rootIt + 5)->volume() == sphereC,
         //               L"Index 5 does not contain sphereC.");

         //Assert::IsTrue((rootIt + 6)->volume() == sphereD,
         //               L"Index 6 does not contain sphereD.");
      }

      TEST_METHOD(InsertUnbalancedLeftHeavy)
      {
         /*DirectX::BoundingSphere sphereA
         {
            DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f),
            1.0f
         };

         DirectX::BoundingSphere sphereB
         {
            DirectX::XMFLOAT3(-1.0f, 0.0f, 0.0f),
            1.0f
         };

         DirectX::BoundingSphere sphereC
         {
            DirectX::XMFLOAT3(7.0f, 0.0f, 0.0f),
            1.0f
         };

         DirectX::BoundingSphere sphereD
         {
            DirectX::XMFLOAT3(3.4f, 0.0f, 0.0f),
            1.0f
         };

         bvh_tree<DirectX::BoundingSphere,
            test_intersects,
            test_volume,
            test_bound> bvh;
         bvh.insert(sphereA);
         bvh.insert(sphereB);
         bvh.insert(sphereC);
         bvh.insert(sphereD);

         Verify that the inserted spheres are in the correct spots in the bvh.
         auto rootIt = bvh.begin();
         Assert::AreEqual(static_cast<size_t>(2), rootIt->height(),
                          L"The root height is not correct.");

         A: index 3
         B: index 6
         C: index 5
         D: index 4

         Assert::IsTrue((rootIt + 3)->volume() == sphereA,
                        L"Index 3 does not contain sphereA.");

         Assert::IsTrue((rootIt + 6)->volume() == sphereB,
                        L"Index 6 does not contain sphereB.");

         Assert::IsTrue((rootIt + 5)->volume() == sphereC,
                        L"Index 5 does not contain sphereC.");

         Assert::IsTrue((rootIt + 4)->volume() == sphereD,
                        L"Index 4 does not contain sphereD.");*/
      }

      TEST_METHOD(InsertUnbalancedLeftRightHeavy)
      {
         //DirectX::BoundingSphere sphereA
         //{
         //   DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f),
         //   1.0f
         //};

         //DirectX::BoundingSphere sphereB
         //{
         //   DirectX::XMFLOAT3(-1.0f, 0.0f, 0.0f),
         //   1.0f
         //};

         //DirectX::BoundingSphere sphereC
         //{
         //   DirectX::XMFLOAT3(7.0f, 0.0f, 0.0f),
         //   1.0f
         //};

         //DirectX::BoundingSphere sphereD
         //{
         //   DirectX::XMFLOAT3(4.0f, 0.0f, 0.0f),
         //   1.0f
         //};

         //bvh_tree<DirectX::BoundingSphere,
         //   test_intersects,
         //   test_volume,
         //   test_bound> bvh;
         //bvh.insert(sphereA);
         //bvh.insert(sphereB);
         //bvh.insert(sphereC);
         //bvh.insert(sphereD);

         ////Verify that the inserted spheres are in the correct spots in the bvh.
         //auto rootIt = bvh.begin();
         //Assert::AreEqual(static_cast<size_t>(2), rootIt->height(),
         //                 L"The root height is not correct.");






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
   };
}