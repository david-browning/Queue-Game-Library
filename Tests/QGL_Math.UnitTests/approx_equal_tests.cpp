#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace qgl;
using namespace qgl::math;

namespace QGL_Math_Unit_Tests
{
   TEST_CLASS(ApproxEqualTests)
   {
      TEST_METHOD(IntApproxEqual)
      {
         Assert::IsTrue(approx_equal(1, 0, 1),
            L"0 and 1 should share a tolerance of 1.");

         Assert::IsFalse(approx_equal(1, -1, 1),
            L"1 and -1 should share not a tolerance of 1.");
      }

      TEST_METHOD(RoundUp)
      {
         //Round 2 to the nearest 10 => 10
         static_assert(round_up(2, 10) == 10, "2 round 10 should be 10");

         //Round 0 to the nearest 2 => 0
         Assert::IsTrue(round_up(0, 2) == 0, L"0 round 2 should be 0");

         //Round 1 to the nearest 1 => 1
         static_assert(round_up(1, 1) == 1, "1 round 1 should be 1");

         //Round 45681 to the nearest 1000 => 46000
         static_assert(round_up(45681, 1000) == 46000,
            "45681 round 1000 should be 46000");

         //Round 0.5 to the nearest 1 => 1
         Assert::IsTrue(approx_equal(round_up(0.5, 1.0), 1.0, DBL_EPSILON),
            L"0.5 round 1 should be close to 1");

         //Round 0.5f to the nearest 0.1f => 0.5f
         Assert::IsTrue(approx_equal(round_up(0.5f, 0.1f), 0.5f, FLT_EPSILON),
            L"0.5 round 0.1 should be 0.5");
      }

      TEST_METHOD(IntEqual)
      {
         Assert::IsTrue(approx_equal(1, 1, 0),
            L"1 and 1 should be equal.");
      }

      TEST_METHOD(FloatApproxEqual)
      {
         //Test with the default epsilon.
         Assert::IsTrue(approx_equal(1.0f, 1.0f, FLT_EPSILON),
            L"1.0 and 1.0 should be equal.");

         Assert::IsFalse(approx_equal(1.0f, 0.9999f, FLT_EPSILON),
            L"1.0 and 0.9999 should not be equal.");

         //Test with custom epsilon.
         Assert::IsTrue(approx_equal(1.0f, 1.05f, 0.05f),
            L"1.0 and 1.05f should be equal with a "
            "tolerance of 0.05");
      }

      TEST_METHOD(DoubleApproxEqual)
      {
         //Test with the default epsilon.
         Assert::IsTrue(approx_equal(1.0, 1.0, DBL_EPSILON),
            L"1.0 and 1.0 should be equal.");

         Assert::IsFalse(approx_equal(1.0, 0.9999, DBL_EPSILON),
            L"1.0 and 0.9999 should not be equal.");

         //Test with custom epsilon.
         Assert::IsTrue(approx_equal(1.0, 1.05, 0.051),
            L"1.0 and 1.05f should be equal with a "
            "tolerance of 0.051");
      }
   };
}