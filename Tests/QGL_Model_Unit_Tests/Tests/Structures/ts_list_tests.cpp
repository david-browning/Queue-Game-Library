#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace qgl;

namespace QGL_Model_Unit_Tests
{
   TEST_CLASS(TSListTests)
   {
      TEST_METHOD(Construct)
      {
         ts_list<int> l;
         Assert::AreEqual(size_t{ 0 }, l.size(),
                          L"List is not empty.");

         std::list<int> iteratorInput = {0, 1, 2};
         ts_list<int> iteratorL{ iteratorInput.begin(), iteratorInput.end() };
         Assert::AreEqual(size_t{ 3 }, iteratorL.size(),
                          L"Iterator constructor does not have 3 elements.");

         ts_list<int> inittList = { 0, 1, 2 };
         Assert::AreEqual(size_t{ 3 }, inittList.size(),
                          L"Iterator constructor does not have 3 elements.");
      }

      TEST_METHOD(CopyConstruct)
      {
         ts_list<int> l;
         l.push_back(0);

         ts_list<int> lCopy{ l };
         Assert::AreEqual(l.size(), lCopy.size(),
                          L"Lists do not have the same size.");

         Assert::AreEqual(l.back(), lCopy.back(),
                          L"Last elements are not equal.");
      }

      TEST_METHOD(MoveConstruct)
      {
         ts_list<int> l;
         l.push_back(0);

         ts_list<int> lCopy{ l };
         ts_list<int> lMove{ std::move(lCopy) };

         Assert::AreEqual(l.size(), lMove.size(),
                          L"Lists do not have the same size.");

         Assert::AreEqual(l.back(), lMove.back(),
                          L"Last elements are not equal.");
      }

      TEST_METHOD(CopyAssign)
      {
         ts_list<int> l;
         l.push_back(0);

         auto lCopy = l;
         Assert::AreEqual(l.size(), lCopy.size(),
                          L"Lists do not have the same size.");

         Assert::AreEqual(l.back(), lCopy.back(),
                          L"Last elements are not equal.");
      }

      TEST_METHOD(Empty)
      {
         ts_list<int> l;
         Assert::IsTrue(l.empty(),
                        L"List should be empty.");
         l.push_back(0);
         Assert::IsFalse(l.empty(),
                         L"List should have an element.");
      }

      TEST_METHOD(PushBack)
      {
         ts_list<int> l;
         l.push_back(0);
         Assert::AreEqual(size_t{ 1 }, l.size(),
                          L"List does not have 1 element.");
         l.push_front(1);
         Assert::AreEqual(size_t{ 2 }, l.size(),
                          L"List does not have 2 elements.");
      }

      TEST_METHOD(PopBack)
      {
         ts_list<int> l;
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

      TEST_METHOD(GPop)
      {
         ts_list<int> l;
         l.push_back(0);
         l.push_back(1);

         auto popped = l.gpop_back();
         Assert::AreEqual(1, popped,
                          L"The popped element should be 1");
         Assert::AreEqual(size_t{ 1 }, l.size(),
                 L"List does not have 1 element.");
      }

      TEST_METHOD(Clear)
      {
         ts_list<int> l;
         l.push_front(0);
         Assert::AreEqual(size_t{ 1 }, l.size(),
                 L"List does not have 1 element.");

         l.clear();
         Assert::IsTrue(l.empty(),
               L"List should be empty.");
      }

      TEST_METHOD(Access)
      {
         // Test "front"
         ts_list<int> l;
         l.push_back(0);
         Assert::AreEqual(0, l.front(),
                          L"Front is not 0");

         // Test "back"
         l.push_back(1);
         Assert::AreEqual(1, l.back(),
                          L"Back is not 0");
         Assert::AreEqual(0, l.front(),
                          L"Front is not 0");
      }

      TEST_METHOD(Erase)
      {
         ts_list<int> l = { 0, 1, 2 };
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

      TEST_METHOD(Remove)
      {
         ts_list<int> l = { 0, 1, 2 };
         l.remove(2);
         Assert::AreEqual(size_t{ 2 }, l.size(),
                 L"List should have 2 elements");
         Assert::AreEqual(0, l.front(),
                          L"Front is not 0");
         Assert::AreEqual(1, l.back(),
                          L"Back is not 1");

         l.push_back(3);
         struct pred
         {
            bool operator()(int x)
            {
               return x == 0;
            }
         };

         l.remove_if(pred{});
         Assert::AreEqual(1, l.front(),
                          L"Front should be 1 after predicate removal.");
      }

      TEST_METHOD(Emplace)
      {
         struct emplaceable
         {
            emplaceable(int x) :
               data(x)
            {

            }

            int data;
         };

         ts_list<emplaceable> l;
         l.emplace_back(0);
         Assert::AreEqual(size_t{ 1 }, l.size(),
                 L"List should have 1 element");
         Assert::AreEqual(0, l.back().data,
                          L"The last element's data should be 0");
      }

      TEST_METHOD(Iterate)
      {
         int expected = 0;
         ts_list<int> l = { 0, 1, 2, 3, 4 };
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