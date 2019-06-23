#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace qgl::math;

namespace QGL_Model_Unit_Tests
{
   TEST_CLASS(RationalTests)
   {
      public:
      /*
       Construct a rational and verify its numerator and denominator are 
       correct.
       */
      TEST_METHOD(Construct)
      {

      }

      /*
       Verify the default constructor creates a rational number that evaluates
       to 0.
       */
      TEST_METHOD(DefaultConstruct)
      {

      }

      /*
       Verify that constructing a rational with 0 as the denominator throws 
       std::invalid_argument.
       */
      TEST_METHOD(ConstructWith0Denom)
      {

      }

      /*
       Create a rational and copy it using the copy constructor.
       Verify the copies share the same members.
       */
      TEST_METHOD(CopyConstructor)
      {

      }

      /*
       Assume copy constructor is correct.
       Test == and != operators.
       */
      TEST_METHOD(EqualityOperator)
      {

      }

      /*
       Assume equality operator is correct.
       */
      TEST_METHOD(AssignOperator)
      {

      }

      /*
       Assume equality operator is correct.
       Create a rational number.
       Create a different rational number. Copy it to another rational.
       Move the copied rational number into the first rational number.
       Verify the moved numbers are equal.
       */
      TEST_METHOD(MoveConstructor)
      {

      }

      /*
       Assume equality operator is correct.
       Assume move constructor is correct.
       */
      TEST_METHOD(MoveAssignOperator)
      {

      }

      /*
       Create a rational, convert it to a float and verify that float is 
       approximately equal to an expected value.
       */
      TEST_METHOD(FloatConversion)
      {

      }

      /*
       Create a rational, convert it to a float and verify that float is
       approximately equal to an expected value.
       */
      TEST_METHOD(DoubleConversion)
      {

      }

      /*
       Test that adding two rationals yields the correct result.
       */
      TEST_METHOD(Addition)
      {

      }

      /*
       Test that subtracting two rationals yields the correct result.
       */
      TEST_METHOD(Subtraction)
      {

      }

      /*
       Test that multiplying two rationals yields the correct result.
       */
      TEST_METHOD(Multiplication)
      {

      }

      /*
       Test that dividing two rationals yields the correct result.
       */
      TEST_METHOD(Division)
      {

      }

      /*
       Test that <, >, <=, and >= are correct.
       */
      TEST_METHOD(ComparisonOperators)
      {

      }
   };
}