#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace qgl;

namespace QGL_Model_Unit_Tests
{
    TEST_CLASS(time_convert_tests)
    {
        public:
        TEST_METHOD(seconds_to_ticks)
        {
            //1 60th of a second
            double seconds = 1.0 / 60.0;
            uint64_t expectedTicks = 166'666;
            
            Assert::AreEqual(expectedTicks,
                             qgl::seconds_to_ticks<decltype(seconds),
                             decltype(expectedTicks)>(seconds),
                             L"The number of ticks in a second is not correct.");
        }

        TEST_METHOD(milliseconds_to_ticks)
        {
            double milliseconds = 1.0 / 60.0;
            uint64_t expectedTicks = 166;

            Assert::AreEqual(expectedTicks,
                             qgl::milliseconds_to_ticks<decltype(milliseconds),
                             decltype(expectedTicks)>(milliseconds),
                             L"The number of ticks in a millisecond is not correct.");
        }

        TEST_METHOD(ticks_to_seconds)
        {
            uint64_t wholeTicks = 20'000'000;
            float wholeSeconds = 2.0f;
            Assert::IsTrue(
                approx_equal(wholeSeconds,
                             qgl::ticks_to_seconds<decltype(wholeSeconds),
                             decltype(wholeTicks)>(wholeTicks)),
                L"Whole seconds does not fall in the allowed tolerance.");

            uint64_t fractionTicks = 12'345'678;
            float fractionSeconds = 1.2345678;
            Assert::IsTrue(
                approx_equal(fractionSeconds,
                             qgl::ticks_to_seconds<decltype(fractionSeconds),
                             decltype(fractionTicks)>(fractionTicks)),
                L"Fraction seconds does not fall in the allowed tolerance.");
        }

        TEST_METHOD(ticks_to_milliseconds)
        {
            uint64_t wholeTicks = 20'000;
            float wholeMilliseconds = 2.0f;
            Assert::IsTrue(
                approx_equal(wholeMilliseconds,
                             qgl::ticks_to_milliseconds<decltype(wholeMilliseconds),
                             decltype(wholeTicks)>(wholeTicks)),
                L"Whole milliseconds does not fall in the allowed tolerance.");

            uint64_t fractionTicks = 12'345;
            float fractionMilliseconds = 1.2345f;
            Assert::IsTrue(
                approx_equal(fractionMilliseconds,
                             qgl::ticks_to_milliseconds<decltype(fractionMilliseconds),
                             decltype(fractionTicks)>(fractionTicks)),
                L"Fraction milliseconds does not fall in the allowed tolerance.");
        }
    };
}