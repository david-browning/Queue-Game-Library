#include "pch.h"
#include "CppUnitTest.h"
#include "basic_observer.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace qgl;

namespace QGL_Model_Unit_Tests
{
    /*
     Test notifying observers. 
     When updated, the basic_observer sets its state to the notify message.
     If a basic_observer's state changes after calling subject::notify,
     then basic_observer::update and subject::notify, work correctly.
     */
    TEST_CLASS(subject_notify_tests)
    {
        public:
        TEST_METHOD(no_observers)
        {
            subject<int> sbj;

            //With no observers, this should not change any state.
            sbj.notify(0);
        }

        TEST_METHOD(single_observer)
        {
            basic_observer<int> observer;

            subject<int> sbj;
            sbj.add(&observer);

            auto oldState = observer.state();

            sbj.notify(1);
            Assert::AreNotEqual(oldState, observer.state(),
                                L"The observer's state did not change.");

            Assert::AreEqual(1, observer.state(),
                             L"After notify, the observer's state should be 1");
        }

        TEST_METHOD(multiple_observers)
        {
            basic_observer<int> observer1;
            basic_observer<int> observer2;
            basic_observer<int> observer3;

            subject<int> sbj;
            sbj.add(&observer1);
            sbj.add(&observer2);
            sbj.add(&observer3);

            sbj.notify(3);
            Assert::AreEqual(3, observer1.state(),
                             L"Observer1's state should be 3");

            Assert::AreEqual(3, observer2.state(),
                             L"Observer2's state should be 3");

            Assert::AreEqual(3, observer3.state(),
                             L"Observer3's state should be 3");
        }
    };
}