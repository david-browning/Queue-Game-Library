#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace qgl::math;

namespace QGL_Math_Unit_Tests
{
   TEST_CLASS(DecimalTests)
   {
      public:
      /*
       Constructs a decimal and verifies its numerator and denominator.
       */
      TEST_METHOD(Construct)
      {
         decimal<10000> d1{};
         using d1NumType = decltype(d1.numerator());
         Assert::AreEqual(static_cast<d1NumType>(10000), d1.denominator(),
            L"The denominator is not 10000");
         Assert::AreEqual(static_cast<d1NumType>(0), d1.numerator(),
            L"The numerator is not 0");

         decimal<255> d2{ 100 };
         using d2NumType = decltype(d2.numerator());
         Assert::AreEqual(static_cast<d2NumType>(255), d2.denominator(),
            L"Denominator is not 255");
         Assert::AreEqual(static_cast<d2NumType>(100), d2.numerator(),
            L"The numerator is not 100");
      }

      /*
       Verify that constructing from a float is correct.
       */
      TEST_METHOD(FromFloat)
      {
         decimal<10000> d1{ 0.1234f };
         using d1NumType = decltype(d1.numerator());
         Assert::AreEqual(d1NumType(1234), d1.numerator(),
            L"Numerator is not 1234");

         decimal<1000> d2{ 0.1f + 0.2f };
         using d2NumType = decltype(d2.numerator());
         Assert::AreEqual(d2NumType(300), d2.numerator(),
            L"Numerator should be 300");
      }

      /*
       Verify that constructing from a double is correct.
       */
      TEST_METHOD(FromDouble)
      {
         decimal<10000> d1{ 0.1234 };
         using d1NumType = decltype(d1.numerator());
         Assert::AreEqual(d1NumType(1234), d1.numerator(),
            L"Numerator is not 1234");

         decimal<100000> d2{ 0.1 + 0.2 };
         using d2NumType = decltype(d2.numerator());
         Assert::AreEqual(d2NumType(30000), d2.numerator(),
            L"Numerator should be 30000");
      }

      /*
       Create a decimal and copy it using the copy constructor.
       Verify the copy shares the same members.
       */
      TEST_METHOD(CopyConstructor)
      {
         decimal<10000> d1{ 100 };
         decimal<10000> d2{ d1 };
         Assert::AreEqual(d1.numerator(), d2.numerator(),
            L"The numerators are not equal.");
      }

      /*
       Assume copy constructor is correct.
       Test == and != operators.
       */
      TEST_METHOD(EqualityOperator)
      {
         decimal<100> d1{ 10 };
         decimal<10> d2{ 1 };
         Assert::IsTrue(d1 == d2, L"d1 and d2 should be equal.");

         decimal<100> d3{ 8 };
         Assert::IsFalse(d1 == d3, L"d1 and d3 should not be equal.");
      }

      /*
       Assume equality operator is correct.
       */
      TEST_METHOD(AssignmentOperator)
      {
         decimal<10000> d1{ 100 };
         auto d2 = d1 ;
         Assert::AreEqual(d1.numerator(), d2.numerator(),
            L"The numerators are not equal.");
      }

      /*
       Assume equality operator is correct.
       Create a decimal number. Copy it to another decimal.
       Move the copied decimal number into the a new decimal number.
       Verify the moved numbers are equal.
       */
      TEST_METHOD(MoveConstructor)
      {
         decimal<100> d1{ 10 };
         decimal<100> d1Copy{ d1 };
         decimal<100> d1Moved{ std::move(d1Copy) };
         Assert::IsTrue(d1 == d1Moved, L"Moved not equal.");
      }

      /*
       Assume equality operator is correct.
       Assume move constructor is correct.
       */
      TEST_METHOD(MoveAssignOperator)
      {
         decimal<100> d1{ 10 };
         decimal<100> d1Copy{ d1 };
         decimal<100> d1Moved = std::move(d1Copy);
         Assert::IsTrue(d1 == d1Moved, L"Moved not equal.");
      }

      /*
       Create a decimal, convert it to a float and verify that float is
       approximately equal to an expected value.
       */
      TEST_METHOD(FloatConversion)
      {
         decimal<100> d1{ 10 };
         Assert::IsTrue(approx_equal(0.1f, static_cast<float>(d1)),
            L"Should be 1/10");

         decimal<100> d2{};
         Assert::IsTrue(approx_equal(0.0f, static_cast<float>(d2)),
            L"Should be 0.");

         decimal<10000> d3{ 3333 };
         Assert::IsTrue(approx_equal(0.3333f, static_cast<float>(d3)),
            L"Should be 1/3");
      }

      /*
       Create a decimal, convert it to a double and verify that double is
       approximately equal to an expected value.
       */
      TEST_METHOD(DoubleConversion)
      {
         decimal<100> d1{ 10 };
         Assert::IsTrue(approx_equal(0.1, static_cast<double>(d1)),
            L"Should be 1/10");

         decimal<100> d2{};
         Assert::IsTrue(approx_equal(0.0, static_cast<double>(d2)),
            L"Should be 0.");

         decimal<10000> d3{ 3333 };
         Assert::IsTrue(approx_equal(0.3333, static_cast<double>(d3)),
            L"Should be 1/3");
      }
   };
}