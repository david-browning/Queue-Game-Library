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
   TEST_CLASS(subject_out_of_scope_tests)
   {
      TEST_METHOD(single_subject_out_of_scope)
      {
         auto observer1 = new basic_observer<int>();

         auto sbj1 = new subject<int>({ observer1 });
         auto sbj2 = new subject<int>({ observer1 });

         sbj1->notify(1);
         Assert::AreEqual(1, observer1->state(),
                          L"Observer1's state should be 1.");

         delete sbj1;

         sbj2->notify(2);
         Assert::AreEqual(2, observer1->state(),
                          L"Observer1's state should be 2.");

         delete observer1;
         delete sbj2;
      }

      TEST_METHOD(all_subjects_out_of_scope)
      {
         auto observer1 = new basic_observer<int>();

         auto sbj1 = new subject<int>({ observer1 });
         auto sbj2 = new subject<int>({ observer1 });

         sbj1->notify(1);
         Assert::AreEqual(1, observer1->state(),
                          L"Observer1's state should be 1.");

         delete sbj1;

         sbj2->notify(2);
         Assert::AreEqual(2, observer1->state(),
                          L"Observer1's state should be 2.");
         delete sbj2;
         delete observer1;
      }
   };
}