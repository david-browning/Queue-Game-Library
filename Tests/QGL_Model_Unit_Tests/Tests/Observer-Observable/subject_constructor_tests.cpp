#include "pch.h"
#include "CppUnitTest.h"
#include "basic_observer.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace qgl;

namespace QGL_Model_Unit_Tests
{
     /*
      Test the constructors by asserting that the correct number of
      observers are stored after construction.
      This implicitly tests subject::count and subject::add
      */
   TEST_CLASS(subject_constructor_tests)
   {
      public:
      TEST_METHOD(no_observers)
      {
         subject<int> sbj;
         Assert::AreEqual(static_cast<size_t>(0), sbj.count(),
                          L"There should be 0 observers.");
      }

      TEST_METHOD(no_observers_input_iterator)
      {
          //Create a collection of observers to pass in.
         std::vector<iobserver<int>*> observers;

         subject<int> sbj{ observers.begin(), observers.end() };
         Assert::AreEqual(static_cast<size_t>(0), sbj.count(),
                          L"There should be 0 observers.");
      }

      TEST_METHOD(one_observer_input_iterator)
      {
         basic_observer<int> observer1;
         std::vector<iobserver<int>*> observers =
         {
             &observer1
         };

         subject<int> sbj{ observers.begin(), observers.end() };
         Assert::AreEqual(static_cast<size_t>(1), sbj.count(),
                          L"There should be 1 observer.");
      }

      TEST_METHOD(multiple_observers_input_iterator)
      {
         basic_observer<int> observer1;
         basic_observer<int> observer2;
         basic_observer<int> observer3;
         std::vector<iobserver<int>*> observers =
         {
             &observer1,
             &observer2,
             &observer3
         };

         subject<int> sbj{ observers.begin(), observers.end() };
         Assert::AreEqual(static_cast<size_t>(3), sbj.count(),
                          L"There should be 3 observers.");
      }

      TEST_METHOD(no_observers_initializer_list)
      {
         std::initializer_list<iobserver<int>*> il;
         subject<int> sbj{ il };
         Assert::AreEqual(static_cast<size_t>(0), sbj.count(),
                          L"There should be 0 observers.");
      }

      TEST_METHOD(one_observer_initializer_list)
      {
         basic_observer<int> observer1;
         subject<int> sbj{ { &observer1 } };
         Assert::AreEqual(static_cast<size_t>(1), sbj.count(),
                          L"There should be 1 observer.");
      }

      TEST_METHOD(multiple_observers_initializer_list)
      {
         basic_observer<int> observer1;
         basic_observer<int> observer2;
         basic_observer<int> observer3;

         subject<int> sbj{ {&observer1, &observer2, &observer3 } };
         Assert::AreEqual(static_cast<size_t>(3), sbj.count(),
                          L"There should be 3 observers.");
      }
   };
}