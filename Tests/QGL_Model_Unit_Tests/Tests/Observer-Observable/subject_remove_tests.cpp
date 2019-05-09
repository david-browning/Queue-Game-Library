#include "pch.h"
#include "CppUnitTest.h"
#include "basic_observer.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace qgl;

namespace QGL_Model_Unit_Tests
{
    /*
     Test removing observers from a subject.
     An observer is removed if its state does not change after calling 
     subject::notify.
     */
    TEST_CLASS(subject_remove_observer_tests)
    {
        public:
        TEST_METHOD(remove_non_existent_observer)
        {

        }

        TEST_METHOD(remove_all_observers)
        {

        }

        TEST_METHOD(remove_readd)
        {

        }
    };
}