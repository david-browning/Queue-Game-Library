#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace qgl;

namespace QGL_Model_Unit_Tests
{
   TEST_CLASS(TimeConversionPrecisionTests)
   {
      using TickT = int64_t;

      static constexpr double MAX_ERROR = 1e-8;

      void AssertAlmostEqual(double actual, double expected)
      {
         Assert::IsTrue(std::abs(actual - expected) < MAX_ERROR,
                        L"Value not accurate to 8 decimal places");
      }

      void RunTest(double seconds)
      {
         TickT ticks = seconds_to_ticks<double, TickT>(seconds);
         time_state<TickT> ts(0, ticks, 0);
         double result = ticks_to_seconds<double>(ts.ticks());
         AssertAlmostEqual(result, seconds);
      }

      TEST_METHOD(Precision_0s) { RunTest(0.0); }
      TEST_METHOD(Precision_3us) { RunTest(0.000003); }
      TEST_METHOD(Precision_10us) { RunTest(0.000010); }
      TEST_METHOD(Precision_25us) { RunTest(0.000025); }
      TEST_METHOD(Precision_100us) { RunTest(0.000100); }
      TEST_METHOD(Precision_500us) { RunTest(0.000500); }
      TEST_METHOD(Precision_1ms) { RunTest(0.001); }
      TEST_METHOD(Precision_3ms) { RunTest(0.003); }
      TEST_METHOD(Precision_25ms) { RunTest(0.025); }
      TEST_METHOD(Precision_100ms) { RunTest(0.1); }
      TEST_METHOD(Precision_500ms) { RunTest(0.5); }
      TEST_METHOD(Precision_1s) { RunTest(1.0); }
      TEST_METHOD(Precision_3s) { RunTest(3.0); }
      TEST_METHOD(Precision_1Day) { RunTest(86400.0); }
      TEST_METHOD(Precision_1Month) { RunTest(30.0 * 86400.0); }
      TEST_METHOD(Precision_140Years) { RunTest(140.0 * 365.25 * 86400.0); }
      TEST_METHOD(Precision_200Years) { RunTest(200.0 * 365.25 * 86400.0); }
   };

   TEST_CLASS(TimeRoundTripTests)
   {
      public:
      using TickT = int64_t;
      static constexpr double MAX_ERROR = 1e-8;

      void AssertRoundTripAccurate(double originalSeconds)
      {
         // Convert seconds ? ticks
         TickT ticks = seconds_to_ticks<double, TickT>(originalSeconds);

         // Convert ticks ? seconds
         double result = ticks_to_seconds<double>(ticks);

         double error = std::abs(result - originalSeconds);
         Assert::IsTrue(error < MAX_ERROR,
            (L"Round-trip error exceeds 8 decimal places. Error: " + std::to_wstring(error)).c_str());
      }

      TEST_METHOD(RT_0s) { AssertRoundTripAccurate(0.0); }
      TEST_METHOD(RT_3us) { AssertRoundTripAccurate(0.000003); }
      TEST_METHOD(RT_10us) { AssertRoundTripAccurate(0.000010); }
      TEST_METHOD(RT_25us) { AssertRoundTripAccurate(0.000025); }
      TEST_METHOD(RT_100us) { AssertRoundTripAccurate(0.000100); }
      TEST_METHOD(RT_500us) { AssertRoundTripAccurate(0.000500); }
      TEST_METHOD(RT_1ms) { AssertRoundTripAccurate(0.001); }
      TEST_METHOD(RT_3ms) { AssertRoundTripAccurate(0.003); }
      TEST_METHOD(RT_25ms) { AssertRoundTripAccurate(0.025); }
      TEST_METHOD(RT_100ms) { AssertRoundTripAccurate(0.1); }
      TEST_METHOD(RT_500ms) { AssertRoundTripAccurate(0.5); }
      TEST_METHOD(RT_1s) { AssertRoundTripAccurate(1.0); }
      TEST_METHOD(RT_3s) { AssertRoundTripAccurate(3.0); }
      TEST_METHOD(RT_1Day) { AssertRoundTripAccurate(86400.0); }
      TEST_METHOD(RT_1Month) { AssertRoundTripAccurate(30.0 * 86400.0); }
      TEST_METHOD(RT_140Years) { AssertRoundTripAccurate(140.0 * 365.25 * 86400.0); }
      TEST_METHOD(RT_200Years) { AssertRoundTripAccurate(200.0 * 365.25 * 86400.0); }
   };
}