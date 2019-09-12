#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace qgl::math;

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

      }

      /*
       Assume constructor is correct.
       Create a rectangle.
       Copy the rectangle using the copy constructor.
       Check that the upper left, bottom right, and normal vectors are equal.
       */
      TEST_METHOD(CopyConstructor)
      {

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

      }

      /*
       Assume copy and equality operator are correct.
       Create a rectangle and copy it using the copy constructor.
       Move the copied rectangle to a new rectangle.
       Moved rectangle and original rectangle should be equal.
       */
      TEST_METHOD(MoveConstructor)
      {

      }
        
      /*
       Assume equality operator is correct.
       Create a rectangle and copy it using assignment operator.
       Rectangles should be equal.
       */
      TEST_METHOD(AssignmentOperator)
      {

      }

      /*
       Create a rectangle.
       Call norm_cache.
       Call norm.
       */
      TEST_METHOD(Normal)
      {

      }

      /*
       Create a rectangle.
       Call norm.
       Set upper left.
       Call norm.
       */
      TEST_METHOD(SetUpperLeft)
      {

      }

      /*
       Create a rectangle.
       Call norm.
       Set bottom right.
       Call norm.
       */
      TEST_METHOD(SetBottomRight)
      {

      }

      /*
       Create a rectangle.
       Verify the rect_2d.
       */
      TEST_METHOD(Get2DRectangle)
      {

      }
   };
}