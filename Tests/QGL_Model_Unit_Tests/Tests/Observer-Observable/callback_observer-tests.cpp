#include "pch.h"
#include "CppUnitTest.h"
#include "..\basic_state.h"
#include "basic_observer.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace qgl;

namespace QGL_Model_Unit_Tests
{
    static basic_state<int> global_state;

    template<class MessageT>
    void free_callback_function(MessageT msg)
    {
        global_state.state() = msg;
    }

    /*
     Test using an observer that raises a callback.
     The only new functionality to test it to verify that the callback is
     raised. We can do this by having the callback change some object's state.
     If the object is in a new state after calling subject::notify, then the
     test is passed.
     */
    TEST_CLASS(callback_observer_tests)
    {
        public:
        TEST_METHOD(callback_raised)
        {
            m_state.state() = 0;
            global_state.state() = 0;
            Assert::AreEqual(0, global_state.state(),
                             L"The global state's state should be 0");
            Assert::AreEqual(0, m_state.state(),
                             L"The member state's state should be 0.");

            //Create two callback_observers
            callback_observer<int> observer1(free_callback_function<int>);
            member_callback_observer<int, callback_observer_tests>
                observer2(*this,
                          &callback_observer_tests::member_callback_function);

            //Create a subject
            subject<int> sbj{ {&observer1, &observer2 } };

            sbj.notify(1);

            Assert::AreEqual(1, global_state.state(),
                             L"The global state's state should be 1");
            Assert::AreEqual(1, m_state.state(),
                             L"The member state's state should be 1.");
        }

        private:
        void member_callback_function(int msg)
        {
            m_state.state() = msg;
        }

        basic_state<int> m_state;
    };
}