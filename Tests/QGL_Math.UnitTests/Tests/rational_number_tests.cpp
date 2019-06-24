#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace qgl::math;

namespace QGL_Math_Unit_Tests
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
         rational<int> r(1, 1);
         Assert::AreEqual(1, r.numerator(),
                          L"The numerator is not 1.");

         Assert::AreEqual(1, r.denominator(),
                          L"The denominator is not 1.");
      }

      /*
       Verify the default constructor creates a rational number that evaluates
       to 0.
       */
      TEST_METHOD(DefaultConstruct)
      {
         rational<int> r;
         Assert::AreEqual(0.0f, static_cast<float>(r),
                          L"The number is not 0.");
         Assert::AreEqual(0, r.numerator(),
                          L"The numerator should be 0.");
      }

      /*
       Verify that constructing a rational with 0 as the denominator throws 
       std::invalid_argument.
       */
      TEST_METHOD(ConstructWith0Denom)
      {
         try
         {
            rational<int> r(1, 0);
         }
         catch (std::invalid_argument&)
         {
            return;
         }

         Assert::Fail(L"The exception was not caught.");
      }

      /*
       Create a rational and copy it using the copy constructor.
       Verify the copies share the same members.
       */
      TEST_METHOD(CopyConstructor)
      {
         rational<int> r(1, 2);
         rational<int> r2(r);

         Assert::AreEqual(r.numerator(), r2.numerator(),
                          L"The numerators are not equal.");

         Assert::AreEqual(r.denominator(), r2.denominator(),
                          L"The denominators are not equal.");
      }

      /*
       Assume copy constructor is correct.
       Test == and != operators.
       */
      TEST_METHOD(EqualityOperator)
      {
         rational<int> r(1, 2);
         rational<int> r2(r);

         Assert::IsTrue(r == r2,
                        L"The numbers should be equal.");

         rational<int> r3(5, 8);
         Assert::IsTrue(r != r3,
                        L"The numbers should not be equal.");
      }

      /*
       Assume equality operator is correct.
       */
      TEST_METHOD(AssignOperator)
      {
         rational<int> r(-3, 8);
         auto r2 = r;
         Assert::IsTrue(r == r2,
                        L"The numbers should be equal.");
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