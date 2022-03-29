#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace qgl::math;

namespace QGL_Math_Unit_Tests
{
   TEST_CLASS(BoundaryTests)
   {
      TEST_METHOD(EmptyConstructor)
      {
         boundary<int, true> b;
         Assert::AreEqual(0, b.left, L"Left is not correct");
         Assert::AreEqual(0, b.top, L"Top is not correct");
         Assert::AreEqual(0, b.bottom, L"Bottom is not correct");
         Assert::AreEqual(0, b.right, L"Right is not correct");
         Assert::AreEqual(0, b.height(), L"Height is not correct");
         Assert::AreEqual(0, b.width(), L"Width is not correct");
      }

      TEST_METHOD(RECTConstructor)
      {
         RECT r{ 1, 2, 3, 4 };
         auto b = boundary<LONG, true>{ r };
         Assert::AreEqual(LONG(1), b.left, L"Left is not correct");
         Assert::AreEqual(LONG(2), b.top, L"Top is not correct");
         Assert::AreEqual(LONG(3), b.right, L"Right is not correct");
         Assert::AreEqual(LONG(4), b.bottom, L"Bottom is not correct");
         Assert::AreEqual(LONG(2), b.width(), L"Width is not correct");
         Assert::AreEqual(LONG(2), b.height(), L"Height is not correct");
      }

      TEST_METHOD(RECTIntersectConstructor)
      {
         RECT b1{ 1, 1, 4, 4 };
         RECT b2{ 3, 0, 5, 5 };
         boundary<int, true> intersect{ b1, b2 };

         Assert::AreEqual(3, intersect.left, L"Left is not correct.");
         Assert::AreEqual(1, intersect.top, L"Top is not correct.");
         Assert::AreEqual(4, intersect.right, L"Right is not correct.");
         Assert::AreEqual(4, intersect.bottom, L"Bottom is not correct.");
         Assert::AreEqual(1, intersect.width(), L"Width id not correct.");
         Assert::AreEqual(3, intersect.height(), L"Height is not correct.");
      }

      TEST_METHOD(BoundaryIntersectConstructorDown1)
      {
         boundary<int, true> b1{ 1, 1, 4, 4 };
         boundary<int, true> b2{ 1, 1, 2, 2 };
         boundary<int, true> intersect{ b1, b2 };

         Assert::AreEqual(1, intersect.left, L"Left is not correct.");
         Assert::AreEqual(1, intersect.top, L"Top is not correct.");
         Assert::AreEqual(2, intersect.right, L"Right is not correct.");
         Assert::AreEqual(2, intersect.bottom, L"Bottom is not correct.");
         Assert::AreEqual(1, intersect.width(), L"Width id not correct.");
         Assert::AreEqual(1, intersect.height(), L"Height is not correct.");
      }

      TEST_METHOD(BoundaryIntersectConstructorDown2)
      {
         boundary<int, true> b1{ 1, 1, 4, 4 };
         boundary<int, true> b2{ 3, 0, 5, 5 };
         boundary<int, true> intersect{ b1, b2 };

         Assert::AreEqual(3, intersect.left, L"Left is not correct.");
         Assert::AreEqual(1, intersect.top, L"Top is not correct.");
         Assert::AreEqual(4, intersect.right, L"Right is not correct.");
         Assert::AreEqual(4, intersect.bottom, L"Bottom is not correct.");
         Assert::AreEqual(1, intersect.width(), L"Width id not correct.");
         Assert::AreEqual(3, intersect.height(), L"Height is not correct.");
      }
      
      TEST_METHOD(BoundaryIntersectConstructorUp1)
      {
         boundary<int, false> b1{ 1, 5, 3, 1 };
         boundary<int, false> b2{ 2, 3, 4, 1 };
         boundary<int, false> intersect{ b1, b2 };

         Assert::AreEqual(2, intersect.left, L"Left is not correct.");
         Assert::AreEqual(3, intersect.top, L"Top is not correct.");
         Assert::AreEqual(3, intersect.right, L"Right is not correct.");
         Assert::AreEqual(1, intersect.bottom, L"Bottom is not correct.");
         Assert::AreEqual(1, intersect.width(), L"Width id not correct.");
         Assert::AreEqual(2, intersect.height(), L"Height is not correct.");
      }

      TEST_METHOD(CopyConstructor)
      {
         RECT r{ 1, 2, 3, 4 };
         auto b1 = boundary<LONG, true>{ r };
         boundary<LONG, true> b{ b1 };

         Assert::AreEqual(LONG(1), b.left, L"Left is not correct");
         Assert::AreEqual(LONG(2), b.top, L"Top is not correct");
         Assert::AreEqual(LONG(3), b.right, L"Right is not correct");
         Assert::AreEqual(LONG(4), b.bottom, L"Bottom is not correct");
         Assert::AreEqual(LONG(2), b.width(), L"Width is not correct");
         Assert::AreEqual(LONG(2), b.height(), L"Height is not correct");
      }

      TEST_METHOD(MoveConstructor)
      {
         RECT r{ 1, 2, 3, 4 };
         auto b1 = boundary<LONG, true>{ r };
         boundary<LONG, true> b{ std::move(b1) };

         Assert::AreEqual(LONG(1), b.left, L"Left is not correct");
         Assert::AreEqual(LONG(2), b.top, L"Top is not correct");
         Assert::AreEqual(LONG(3), b.right, L"Right is not correct");
         Assert::AreEqual(LONG(4), b.bottom, L"Bottom is not correct");
         Assert::AreEqual(LONG(2), b.width(), L"Width is not correct");
         Assert::AreEqual(LONG(2), b.height(), L"Height is not correct");
      }

      TEST_METHOD(CopyAssign)
      {
         RECT r{ 1, 2, 3, 4 };
         auto b1 = boundary<LONG, true>{ r };
         auto b = b1;

         Assert::AreEqual(LONG(1), b.left, L"Left is not correct");
         Assert::AreEqual(LONG(2), b.top, L"Top is not correct");
         Assert::AreEqual(LONG(3), b.right, L"Right is not correct");
         Assert::AreEqual(LONG(4), b.bottom, L"Bottom is not correct");
         Assert::AreEqual(LONG(2), b.width(), L"Width is not correct");
         Assert::AreEqual(LONG(2), b.height(), L"Height is not correct");
      }

      TEST_METHOD(Intersection)
      {
         boundary<int, true> b1{ 1, 1, 4, 4 };
         Assert::IsTrue(b1.intersects(b1), L"B1 does not intersect itself.");

         boundary<int, true> b2{ 1, 1, 2, 2 };
         Assert::IsTrue(b1.intersects(b2), L"B1 and B2 do not intersect.");

         boundary<int, true> b3{ 6, 6, 8, 8 };
         Assert::IsFalse(b1.intersects(b3), L"B1 and B3 intersect.");
      }
   };
}