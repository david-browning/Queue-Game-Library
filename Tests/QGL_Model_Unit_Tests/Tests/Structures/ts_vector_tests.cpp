#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace qgl;

namespace QGL_Model_Unit_Tests
{
   TEST_CLASS(TSVectorTests)
   {
      TEST_METHOD(Construct)
      {
         ts_vector<int> l;
         Assert::AreEqual(size_t{ 0 }, l.size(),
                          L"List is not empty.");

         std::list<int> iteratorInput = { 0, 1, 2 };
         ts_vector<int> iteratorL{ iteratorInput.begin(), iteratorInput.end() };
         Assert::AreEqual(size_t{ 3 }, iteratorL.size(),
                          L"Iterator constructor does not have 3 elements.");

         ts_vector<int> inittList = { 0, 1, 2 };
         Assert::AreEqual(size_t{ 3 }, inittList.size(),
                          L"Iterator constructor does not have 3 elements.");
      }

      TEST_METHOD(CopyConstruct)
      {
         ts_vector<int> l;
         l.push_back(0);

         ts_vector<int> lCopy{ l };
         Assert::AreEqual(l.size(), lCopy.size(),
                          L"Lists do not have the same size.");

         Assert::AreEqual(l.back(), lCopy.back(),
                          L"Last elements are not equal.");
      }

      TEST_METHOD(MoveConstruct)
      {
         ts_vector<int> l;
         l.push_back(0);

         ts_vector<int> lCopy{ l };
         ts_vector<int> lMove{ std::move(lCopy) };

         Assert::AreEqual(l.size(), lMove.size(),
                          L"Lists do not have the same size.");

         Assert::AreEqual(l.back(), lMove.back(),
                          L"Last elements are not equal.");
      }

      TEST_METHOD(CopyAssign)
      {
         ts_vector<int> l;
         l.push_back(0);

         auto lCopy = l;
         Assert::AreEqual(l.size(), lCopy.size(),
                          L"Lists do not have the same size.");

         Assert::AreEqual(l.back(), lCopy.back(),
                          L"Last elements are not equal.");
      }

      TEST_METHOD(Empty)
      {
         ts_vector<int> l;
         Assert::IsTrue(l.empty(),
                        L"List should be empty.");
         l.push_back(0);
         Assert::IsFalse(l.empty(),
                         L"List should have an element.");
      }

      TEST_METHOD(PushBack)
      {
         ts_vector<int> l;
         l.push_back(0);
         Assert::AreEqual(size_t{ 1 }, l.size(),
                          L"List does not have 1 element.");
         l.push_back(1);
         Assert::AreEqual(size_t{ 2 }, l.size(),
                          L"List does not have 2 elements.");
      }

      TEST_METHOD(PopBack)
      {
         ts_vector<int> l;
         l.push_back(0);
         Assert::AreEqual(size_t{ 1 }, l.size(),
                          L"List does not have 1 element.");
         l.push_back(1);
         Assert::AreEqual(size_t{ 2 }, l.size(),
                          L"List does not have 2 elements.");
         Assert::AreEqual(1, l.back(),
                          L"The last back element should be 1");

         l.pop_back();
         Assert::AreEqual(size_t{ 1 }, l.size(),
                          L"List does not have 1 element after pop_back.");

         Assert::AreEqual(0, l.back(),
                          L"The last back element should be 0");
      }

      TEST_METHOD(Clear)
      {
         ts_vector<int> l;
         l.push_back(0);
         Assert::AreEqual(size_t{ 1 }, l.size(),
                 L"List does not have 1 element.");

         l.clear();
         Assert::IsTrue(l.empty(),
               L"List should be empty.");
      }

      TEST_METHOD(Resize)
      {
         // Calling resize should trigger a realloc.
         ts_vector<int> l;
         auto lEmptyPtr = l.data();

         l.resize(4);
         Assert::AreNotEqual(lEmptyPtr, l.data(),
                             L"The data pointers should be different after resize.");

         l.resize(8, 1);
         Assert::AreEqual(1, l.back(),
                          L"Resized did not emplace 1's");
      }

      TEST_METHOD(Erase)
      {
         ts_vector<int> l = { 0, 1, 2 };
         l.erase(++l.begin());
         Assert::AreEqual(size_t{ 2 }, l.size(),
                          L"List should have 2 elements");
         Assert::AreEqual(0, l.front(),
                          L"Front is not 0");
         Assert::AreEqual(2, l.back(),
                          L"Back is not 2");

         l.push_back(3);
         l.erase(++l.begin(), l.end());
         Assert::AreEqual(size_t{ 1 }, l.size(),
                          L"List should have 1 element");
      }

      TEST_METHOD(Iterate)
      {
         int expected = 0;
         ts_vector<int> l = { 0, 1, 2, 3, 4 };
         for (auto i : l)
         {
            Assert::AreEqual(expected, i);
            expected++;
         }

         expected = 0;
         for (auto cit = l.cbegin(); cit != l.cend(); cit++)
         {
            Assert::AreEqual(expected, *cit);
            expected++;
         }
      }
   };
}