#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace qgl;

namespace QGL_Model_Unit_Tests
{
   TEST_CLASS(TimerTests)
   {
      public:
      TEST_METHOD(CopyConstructor)
      {
         timer<int> t;
         t.tick();

         // Make a copy of the timer.
         timer<int> tCopy {t};

         // Check that the states are the same.
         auto tState{ t.state() };
         auto tCopyState{ tCopy.state() };

         Assert::IsTrue(tState == tCopyState,
                        L"States are not the same.");
      }

      TEST_METHOD(AssignmentOperator)
      {
         timer<int> t;
         t.tick();

         // Make a copy of the timer.
         auto tCopy = t;

         // Check that the states are the same.
         auto tState{ t.state() };
         auto tCopyState{ tCopy.state() };

         Assert::IsTrue(tState == tCopyState,
                        L"States are not the same.");

      }

      TEST_METHOD(MoveConstructor)
      {
         timer<int> t;
         t.tick();

         // Make a copy of the timer.
         timer<int> tCopy {t};
         timer<int> tMove {std::move(tCopy)};


         // Check that the states are the same.
         auto tState{ t.state() };
         auto tCopyState{ tMove.state() };

         Assert::IsTrue(tState == tCopyState,
                        L"States are not the same.");
      }

      TEST_METHOD(Tick)
      {
         using namespace std::chrono_literals;

         // How do you test a timer?
         // Make sure that the timer advanced between two advances of tick?

         timer<int> t;
         auto tState{ t.state() };
         t.tick();
         std::this_thread::sleep_for(30ms);
         t.tick();
         auto tickedState{ t.state() };
         Assert::IsTrue(tickedState > tState,
                        L"The future state is not less than the past state.");
      }
   };
}