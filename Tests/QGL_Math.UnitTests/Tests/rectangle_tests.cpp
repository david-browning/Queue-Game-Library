#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace qgl;
using namespace qgl::math;
using namespace DirectX;

namespace QGL_Math_Unit_Tests
{
   TEST_CLASS(RectangleTests)
   {
      public:

      /*
       Create a rectangle.
       Check that the upper left and bottom right vectors are correct.
       */
      TEST_METHOD(Constructor)
      {
         auto ul = XMVectorSet(1.0f, -1.0f, 2.0f, 0.0f);
         auto br = XMVectorSet(2.0f, 1.0f, 1.0f, 99.9f);
         auto normal = XMVectorSet(-2.0f, 1.0f, 0.0f, 0.0f);

         rectangle r{ ul, br };
         Assert::IsTrue(XMVector3Equal(ul, r.ul()), L"UL is not correct.");
         Assert::IsTrue(XMVector3Equal(br, r.br()), L"BR is not correct.");
         Assert::IsTrue(XMVector3Equal(normal, r.norm_cache()),
                        L"Normal is not correct.");
      }

      /*
       Assume constructor is correct.
       Create a rectangle.
       Copy the rectangle using the copy constructor.
       Check that the upper left, bottom right, and normal vectors are equal.
       */
      TEST_METHOD(CopyConstructor)
      {
         auto ul = XMVectorSet(1.0f, -1.0f, 2.0f, 0.0f);
         auto br = XMVectorSet(2.0f, 1.0f, 1.0f, 99.9f);
         auto normal = XMVectorSet(-2.0f, 1.0f, 0.0f, 0.0f);

         rectangle r{ ul, br };
         rectangle rCopy{ r };
         Assert::IsTrue(XMVector3Equal(r.ul(), rCopy.ul()), 
                        L"ULs are not equal.");
         Assert::IsTrue(XMVector3Equal(r.br(), rCopy.br()), 
                        L"BRs are not equal.");
         Assert::IsTrue(XMVector3Equal(r.norm_cache(), rCopy.norm_cache()),
                        L"Normals are not equal.");
      }
      
      /*
       Assume copy constructor is correct.
       Create a rectangle and copy it using the copy constructor.
       They should be equal.
       Create another, different rectangle.
       This rectangle should not be equal to the others.
       */
      TEST_METHOD(EqualityOperator)
      {
         auto ul = XMVectorSet(1.0f, -1.0f, 2.0f, 0.0f);
         auto br = XMVectorSet(2.0f, 1.0f, 1.0f, 99.9f);
         auto normal = XMVectorSet(-2.0f, 1.0f, 0.0f, 0.0f);

         rectangle r{ ul, br };
         rectangle rCopy{ r };
         Assert::IsTrue(r == rCopy, L"Rectangles should be equal.");

         r.br(ul);
         Assert::IsFalse(r == rCopy, L"Rectangles should not be equal.");
      }

      /*
       Assume copy and equality operator are correct.
       Create a rectangle and copy it using the copy constructor.
       Move the copied rectangle to a new rectangle.
       Moved rectangle and original rectangle should be equal.
       */
      TEST_METHOD(MoveConstructor)
      {
         auto ul = XMVectorSet(1.0f, -1.0f, 2.0f, 0.0f);
         auto br = XMVectorSet(2.0f, 1.0f, 1.0f, 99.9f);
         auto normal = XMVectorSet(-2.0f, 1.0f, 0.0f, 0.0f);

         rectangle r{ ul, br };
         rectangle rCopy{ r };
         rectangle rMove{ std::move(rCopy) };
         Assert::IsTrue(r == rMove, L"Moved rectangle is not equal.");
      }
        
      /*
       Assume equality operator is correct.
       Create a rectangle and copy it using assignment operator.
       Rectangles should be equal.
       */
      TEST_METHOD(AssignmentOperator)
      {
         auto ul = XMVectorSet(1.0f, -1.0f, 2.0f, 0.0f);
         auto br = XMVectorSet(2.0f, 1.0f, 1.0f, 99.9f);
         auto normal = XMVectorSet(-2.0f, 1.0f, 0.0f, 0.0f);

         rectangle r{ ul, br };
         auto rCopy =  r;
         Assert::IsTrue(r == rCopy, L"Rectangles should be equal.");
      }

      /*
       Create a rectangle.
       Call norm_cache.
       Call norm.
       */
      TEST_METHOD(Normal)
      {
         auto ul = XMVectorSet(1.0f, -1.0f, 2.0f, 0.0f);
         auto br = XMVectorSet(2.0f, 1.0f, 1.0f, 99.9f);
         auto normal = XMVectorSet(-2.0f, 1.0f, 0.0f, 0.0f);

         rectangle r{ ul, br };
         Assert::IsTrue(XMVector3Equal(normal, r.norm_cache()),
                        L"Cached normal is not correct.");

         Assert::IsTrue(XMVector3Equal(normal, r.norm()),
                        L"Normal is not correct.");
      }

      /*
       Create a rectangle.
       Set upper left.
       Call norm.
       */
      TEST_METHOD(SetUpperLeft)
      {
         auto ul = XMVectorSet(1.0f, -1.0f, 2.0f, 0.0f);
         auto br = XMVectorSet(2.0f, 1.0f, 1.0f, 99.9f);
         rectangle r{ ul, br };

         auto newul = XMVectorSet(3.0f, 3.0f, -3.0f, 0.0f);
         r.ul(newul);
         auto normal = XMVectorSet(8.0f, -4.0f, 0.0f, 0.0f);

         Assert::IsTrue(XMVector3Equal(normal, r.norm()),
                        L"Normal is not correct.");
      }

      /*
       Create a rectangle.
       Set bottom right.
       Call norm.
       */
      TEST_METHOD(SetBottomRight)
      {
         auto ul = XMVectorSet(1.0f, -1.0f, 2.0f, 0.0f);
         auto br = XMVectorSet(2.0f, 1.0f, 1.0f, 99.9f);
         rectangle r{ ul, br };

         auto newbr = XMVectorSet(2.0f, 2.0f, 1.0f, 0.0f);
         r.br(newbr);
         auto normal = XMVectorSet(-3.0f, 1.0f, 0.0f, 0.0f);
         auto x = r.norm();
         Assert::IsTrue(XMVector3Equal(normal, r.norm()),
                        L"Normal is not correct.");

      }

      /*
       Create a rectangle.
       Verify the rect_2d.
       */
      TEST_METHOD(Get2DRectangle)
      {
         auto ul = XMVectorSet(1.0f, -1.0f, 2.0f, 0.0f);
         auto br = XMVectorSet(2.0f, 1.0f, 1.0f, 99.9f);
         rectangle r{ ul, br };

         D2D_RECT_F expected{ 1.0f, -1.0f, 2.0f, 1.0f };
         auto actual = r.rect_2d();
         Assert::IsTrue(approx_equal(expected.top, actual.top), 
                        L"Tops are not equal.");
         Assert::IsTrue(approx_equal(expected.right, actual.right),
                        L"Rights are not equal.");
         Assert::IsTrue(approx_equal(expected.left, actual.left),
                        L"Lefts are not equal.");
         Assert::IsTrue(approx_equal(expected.bottom, actual.bottom),
                        L"Bottoms are not equal.");
      }
   };
}