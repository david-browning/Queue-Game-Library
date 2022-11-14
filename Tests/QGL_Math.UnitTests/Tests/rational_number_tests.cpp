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
         rational<int> twoFourths(2, 4);

         Assert::IsTrue(r == twoFourths,
                        L"The numbers should be equal.");

         rational<int> r3(5, 8);
         Assert::IsTrue(r != r3,
                        L"The numbers should not be equal.");

         rational<int> zeroThrids(0, 3);
         rational<int> zeroFourths(0, 4);
         Assert::IsTrue(zeroThrids == zeroFourths,
                        L"0 == 0");
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
       Create a rational number. Copy it to another rational.
       Move the copied rational number into the a new rational number.
       Verify the moved numbers are equal.
       */
      TEST_METHOD(MoveConstructor)
      {
         rational<int> oneThird(1, 3);
         rational<int> oneThirdCopy(oneThird);

         rational<int> oneThirdMoved(std::move(oneThirdCopy));
         Assert::IsTrue(oneThird == oneThirdMoved,
                        L"Moved not equal.");
      }

      /*
       Assume equality operator is correct.
       Assume move constructor is correct.
       */
      TEST_METHOD(MoveAssignOperator)
      {
         rational<int> oneThird(1, 3);
         rational<int> oneThirdCopy(oneThird);
         rational<int> oneThirdMoved = std::move(oneThirdCopy);
         Assert::IsTrue(oneThird == oneThirdMoved,
                        L"Moved not equal.");
      }

      /*
       Create a rational, convert it to a float and verify that float is
       approximately equal to an expected value.
       */
      TEST_METHOD(FloatConversion)
      {
         rational<int> zero;
         rational<int> one(1, 1);
         rational<int> netagiveOne(-1, 1);
         Assert::IsTrue(
            approx_equal(0.0f, static_cast<float>(zero), FLT_EPSILON),
            L"Should be 0");

         Assert::IsTrue(
            approx_equal(1.0f, static_cast<float>(one), FLT_EPSILON),
            L"Should be 1");

         Assert::IsTrue(
            approx_equal(-1.0f, static_cast<float>(netagiveOne), FLT_EPSILON),
            L"Should be -1");

         rational<int> oneTenth(1, 10);
         Assert::IsTrue(
            approx_equal(0.1f, static_cast<float>(oneTenth), FLT_EPSILON),
            L"Should be 1/10");

         Assert::IsTrue(
            !approx_equal(0.099999f, static_cast<float>(oneTenth), FLT_EPSILON),
            L"Should be 1/10");
      }

      /*
       Create a rational, convert it to a double and verify that double is
       approximately equal to an expected value.
       */
      TEST_METHOD(DoubleConversion)
      {
         rational<int> zero;
         rational<int> one(1, 1);
         rational<int> netagiveOne(-1, 1);
         Assert::IsTrue(
            approx_equal(0.0, static_cast<double>(zero), DBL_EPSILON),
            L"Should be 0");

         Assert::IsTrue(
            approx_equal(1.0, static_cast<double>(one), DBL_EPSILON),
            L"Should be 1");

         Assert::IsTrue(
            approx_equal(-1.0, static_cast<double>(netagiveOne), DBL_EPSILON),
            L"Should be -1");

         rational<int> oneTenth(1, 10);
         Assert::IsTrue(
            approx_equal(0.1, static_cast<double>(oneTenth), DBL_EPSILON),
            L"Should be 1/10");

         Assert::IsTrue(
            !approx_equal(0.099999, static_cast<double>(oneTenth), DBL_EPSILON),
            L"Should be 1/10");
      }

      /*
       Test that adding two rationals yields the correct result.
       */
      TEST_METHOD(Addition)
      {
         rational<int> oneHalf(1, 2);
         Assert::AreEqual(rational<int>(1, 1), oneHalf + oneHalf,
                          L"Should be 1/2");

         rational<int> two(2, 1);
         Assert::AreEqual(rational<int>(10, 4), two + oneHalf,
                          L"Should be 5/2");
      }

      /*
       Test that subtracting two rationals yields the correct result.
       */
      TEST_METHOD(Subtraction)
      {
         rational<int> oneHalf(1, 2);
         Assert::AreEqual(rational<int>(), oneHalf - oneHalf,
                          L"Should be 0");

         rational<int> zero;
         Assert::AreEqual(rational<int>(-1, 2), zero - oneHalf,
                          L"Should be -1/2");
      }

      /*
       Test that multiplying two rationals yields the correct result.
       */
      TEST_METHOD(Multiplication)
      {
         rational<int> oneThird(1, 3);
         rational<int> oneHalf(1, 2);
         Assert::AreEqual(rational<int>(1, 6), oneThird * oneHalf,
                        L"Should be 1/6");

         rational<int> negativeOneFourth(1, -4);
         Assert::AreEqual(rational<int>(-1, 8), oneHalf * negativeOneFourth,
                        L"Should be -1/8");
      }

      /*
       Test that dividing two rationals yields the correct result.
       */
      TEST_METHOD(Division)
      {
         rational<int> oneThird(1, 3);
         rational<int> oneHalf(1, 2);
         rational<int> negativeOneHalf(-1, 2);

         Assert::AreEqual(rational<int>(2, 3), oneThird / oneHalf,
                          L"Should be 2/3");

         Assert::AreEqual(rational<int>(3, 2), oneHalf / oneThird,
                          L"Should be 3/2");

         Assert::IsTrue(approx_equal(1.5f,
                                     static_cast<float>(oneHalf / oneThird),
                                     FLT_EPSILON),
                        L"Should be 1.5");
      }

      /*
       Test that <, >, <=, and >= are correct.
       */
      TEST_METHOD(ComparisonOperators)
      {
         rational<int> oneThird(1, 3);
         rational<int> oneHalf(1, 2);
         Assert::IsTrue(oneThird < oneHalf,
                        L"1/3 < 1/2");

         Assert::IsTrue(oneHalf > oneThird,
                        L"1/2 > 1/3");

         rational<int> twoFourths(2, 4);
         Assert::IsTrue(oneHalf >= twoFourths,
                        L"1/2 >= 2/4");

         Assert::IsTrue(oneHalf <= twoFourths,
                        L"1/2 <= 2/4");
      }
   };
}