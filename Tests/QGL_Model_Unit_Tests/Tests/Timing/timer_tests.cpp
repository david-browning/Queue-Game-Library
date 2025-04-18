#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace qgl;

namespace QGL_Model_Unit_Tests
{
   TEST_CLASS(timer_tests)
   {
      using TickT = int64_t;
      using timer_t = timer<TickT>;
      using time_state_t = time_state<TickT>;

      static constexpr TickT one_second = timer_t::TICKS_PER_SECOND;

      public:

      TEST_METHOD(single_tick_advances_and_calls_functor)
      {
         timer_t t(timer_t::TICK_60_HZ);
         int updatesCalled = 0;

         Sleep(20); // ~1 update frame

         t.tick([&]
         {
            updatesCalled++;
         });

         auto state = t.state();
         Assert::IsTrue(updatesCalled > 0, L"Update functor should be called at least once");
         Assert::IsTrue(state.delta_t() > 0, L"delta_t should be positive after tick");
         Assert::IsTrue(state.ticks() > 0, L"ticks should be positive after tick");
      }

      TEST_METHOD(multiple_update_calls)
      {
         timer_t t(timer_t::TICK_60_HZ);
         int updatesCalled = 0;

         Sleep(40); // enough for about 2-3 ticks

         t.tick([&]
         {
            updatesCalled++;
         });

         Assert::IsTrue(updatesCalled >= 2, L"Update functor should be called multiple times if enough time has passed");
      }

      TEST_METHOD(empty_lambda_safe)
      {
         timer_t t(timer_t::TICK_60_HZ);
         Sleep(16);
         // should not crash or throw
         t.tick([] {});
         auto state = t.state();
         Assert::IsTrue(state.ticks() > 0, L"tick with empty lambda should still advance state");
      }
   };
}