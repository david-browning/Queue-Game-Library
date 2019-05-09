#include "pch.h"
#include "CppUnitTest.h"
#include "basic_observer.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace qgl;

namespace QGL_Model_Unit_Tests
{
    /*
     Test that subjects still work when one of their observers goes out of
     scope.
     The only time when the subject's correct behavior can be observed is when
     the observer goes out of scope and the subject's descturctor does not
     crash.
     */
    TEST_CLASS(observer_out_of_scope_tests)
    {
        public:
        TEST_METHOD(single_observer_out_of_scope)
        {
            auto observer1 = new basic_observer<int>();
            auto observer2 = new basic_observer<int>();

            auto sbj = new subject<int>({ observer1, observer2 });
            sbj->notify(1);

            Assert::AreEqual(1, observer1->state(),
                             L"Observer 1's state should be 1");

            Assert::AreEqual(1, observer1->state(),
                             L"Observer 2's state should be 1");

            delete observer1;

            sbj->notify(2);

            Assert::AreEqual(2, observer2->state(),
                             L"Observer 2's state should be 2");

            delete sbj;
            delete observer2;
        }

        TEST_METHOD(all_observers_out_of_scope)
        {
            auto observer1 = new basic_observer<int>();
            auto observer2 = new basic_observer<int>();

            auto sbj = new subject<int>({ observer1, observer2 });
            sbj->notify(1);

            Assert::AreEqual(1, observer1->state(),
                             L"Observer 1's state should be 1");

            Assert::AreEqual(1, observer1->state(),
                             L"Observer 2's state should be 1");
                       
            delete observer1;
            delete observer2;

            sbj->notify(2);
            delete sbj;
        }
    };
}