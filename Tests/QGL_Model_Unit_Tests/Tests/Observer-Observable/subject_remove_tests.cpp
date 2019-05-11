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
      /*
       Test removing an observer from a subject that does not contain that
       observer.
       There should be no state changes.
       */
      TEST_METHOD(remove_non_existent_observer)
      {
         basic_observer<int> observer1;
         auto state = observer1.state();

         subject<int> sbj;
         sbj.remove(&observer1);
         sbj.notify(1);

         Assert::AreEqual(state, observer1.state(),
                          L"The state should not have changed.");
      }

      /*
       Add observers to a subject, notify them, and remove them. After
       notifying again, the observers should not have their state changed.
       */
      TEST_METHOD(remove_all_observers)
      {
         basic_observer<int> observer1;
         basic_observer<int> observer2;
         basic_observer<int> observer3;

         subject<int> sbj{ {&observer1, &observer2, &observer3 } };
         sbj.notify(1);

         Assert::AreEqual(1, observer1.state(),
                          L"The state should be 1.");

         Assert::AreEqual(1, observer2.state(),
                          L"The state should be 1.");

         Assert::AreEqual(1, observer3.state(),
                          L"The state should be 1.");

         sbj.remove(&observer1);
         sbj.remove(&observer2);
         sbj.remove(&observer3);

         sbj.notify(2);

         Assert::AreEqual(1, observer1.state(),
                          L"The state should still be 1.");

         Assert::AreEqual(1, observer2.state(),
                          L"The state should still be 1.");

         Assert::AreEqual(1, observer3.state(),
                          L"The state should still be 1.");
      }

      /*
       Remove observers from a subject, add them back, and test that their
       states get updated after notifying.
       */
      TEST_METHOD(remove_readd)
      {
         basic_observer<int> observer1;
         basic_observer<int> observer2;

         subject<int> sbj{ {&observer1, &observer2 } };
         sbj.notify(1);

         Assert::AreEqual(1, observer1.state(),
                          L"The state should be 1.");

         Assert::AreEqual(1, observer2.state(),
                          L"The state should be 1.");

         sbj.remove(&observer1);
         sbj.remove(&observer2);

         sbj.notify(2);

         Assert::AreEqual(1, observer1.state(),
                          L"The state should still be 1.");

         Assert::AreEqual(1, observer2.state(),
                          L"The state should still be 1.");

         sbj.add(&observer1);
         sbj.add(&observer2);

         sbj.notify(3);


         Assert::AreEqual(3, observer1.state(),
                          L"The state should be 3.");

         Assert::AreEqual(3, observer2.state(),
                          L"The state should be 3.");
      }
   };
}