#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace qgl;

namespace QGL_Model_Unit_Tests
{
   TEST_CLASS(time_state_tests)
   {
      using TickT = int64_t;
      using time_state_t = time_state<TickT>;

      public:

      TEST_METHOD(accessors)
      {
         TickT delta = 16;
         TickT total = 160;
         UINT fps = 60;

         time_state_t t(delta, total, fps);

         Assert::AreEqual(delta, t.delta_t(), L"delta_t() incorrect");
         Assert::AreEqual(total, t.ticks(), L"ticks() incorrect");
         Assert::AreEqual(fps, t.fps(), L"fps() incorrect");
      }

      TEST_METHOD(assignment_operator)
      {
         time_state_t t1(5, 100, 60);
         time_state_t t2;

         t2 = t1;

         Assert::AreEqual(t1.delta_t(), t2.delta_t());
         Assert::AreEqual(t1.ticks(), t2.ticks());
         Assert::AreEqual(t1.fps(), t2.fps());
      }

      TEST_METHOD(copy_constructor)
      {
         time_state_t original(10, 200, 120);
         time_state_t copy(original);

         Assert::AreEqual(original.delta_t(), copy.delta_t());
         Assert::AreEqual(original.ticks(), copy.ticks());
         Assert::AreEqual(original.fps(), copy.fps());
      }

      TEST_METHOD(move_constructor)
      {
         time_state_t original(20, 300, 144);
         time_state_t moved(std::move(original));

         Assert::AreEqual(static_cast<TickT>(20), moved.delta_t());
         Assert::AreEqual(static_cast<TickT>(300), moved.ticks());
         Assert::AreEqual(144u, moved.fps());
      }
   };
}