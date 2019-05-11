#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace qgl;

namespace QGL_Model_Unit_Tests
{
    /*
     time_state is a storage class for timing variables.
     The only functions to test are public accessors.
     */
   TEST_CLASS(time_state_tests)
   {
      TEST_METHOD(accessors)
      {
         time_state<uint64_t> tstate(static_cast<uint64_t>(500),
                                     static_cast<uint64_t>(4000),
                                     60);

         Assert::AreEqual(static_cast<uint64_t>(500),
                          tstate.delta_t(),
                          L"The delta ticks is not correct.");

         Assert::AreEqual(static_cast<uint64_t>(4000),
                          tstate.ticks(),
                          L"The total ticks is not correct.");

         Assert::AreEqual(static_cast<UINT>(60),
                          tstate.fps(),
                          L"The fps is not correct.");
      }

      TEST_METHOD(assignment_operator)
      {
         time_state<uint64_t> oldtstate(static_cast<uint64_t>(500),
                                        static_cast<uint64_t>(4000),
                                        60);

         time_state<uint64_t> tstate = oldtstate;

         Assert::AreEqual(static_cast<uint64_t>(500),
                          tstate.delta_t(),
                          L"The delta ticks is not correct.");

         Assert::AreEqual(static_cast<uint64_t>(4000),
                          tstate.ticks(),
                          L"The total ticks is not correct.");

         Assert::AreEqual(static_cast<UINT>(60),
                          tstate.fps(),
                          L"The fps is not correct.");
      }

      TEST_METHOD(copy_constructor)
      {
         time_state<uint64_t> oldtstate(static_cast<uint64_t>(500),
                                        static_cast<uint64_t>(4000),
                                        60);

         time_state<uint64_t> tstate(oldtstate);

         Assert::AreEqual(static_cast<uint64_t>(500),
                          tstate.delta_t(),
                          L"The delta ticks is not correct.");

         Assert::AreEqual(static_cast<uint64_t>(4000),
                          tstate.ticks(),
                          L"The total ticks is not correct.");

         Assert::AreEqual(static_cast<UINT>(60),
                          tstate.fps(),
                          L"The fps is not correct.");
      }

      TEST_METHOD(move_constructor)
      {
         time_state<uint64_t> oldtstate(static_cast<uint64_t>(500),
                                        static_cast<uint64_t>(4000),
                                        60);

         time_state<uint64_t> tstate(std::move(oldtstate));

         Assert::AreEqual(static_cast<uint64_t>(500),
                          tstate.delta_t(),
                          L"The delta ticks is not correct.");

         Assert::AreEqual(static_cast<uint64_t>(4000),
                          tstate.ticks(),
                          L"The total ticks is not correct.");

         Assert::AreEqual(static_cast<UINT>(60),
                          tstate.fps(),
                          L"The fps is not correct.");
      }
   };
}