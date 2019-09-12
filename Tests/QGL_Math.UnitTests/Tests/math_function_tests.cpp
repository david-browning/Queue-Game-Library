#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace qgl::math;

namespace QGL_Math_Unit_Tests
{
   TEST_CLASS(MathFuncTests)
   {
      public:
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
         Assert::IsTrue(approx_equal(round_up(0.5, 1.0), 1.0), 
                        L"0.5 round 1 should be close to 1");

         //Round 0.5f to the nearest 0.1f => 0.5f
         Assert::IsTrue(approx_equal(round_up(0.5f, 0.1f), 0.5f),
                        L"0.5 round 0.1 should be 0.5");
      }
   };
}