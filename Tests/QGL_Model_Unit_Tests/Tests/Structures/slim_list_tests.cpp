#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace qgl;

namespace QGL_Model_Unit_Tests
{
   TEST_CLASS(SlimListTests)
   {
      public:
      TEST_METHOD(DefaultConstructor_IsEmpty)
      {
         qgl::slim_list<int, qgl::win32_srw_traits> list;
         Assert::IsTrue(list.empty());
         Assert::AreEqual<size_t>(0, list.size());
      }

      TEST_METHOD(InitializerList_ConstructsCorrectly)
      {
         qgl::slim_list<int, qgl::win32_srw_traits> list{ 1, 2, 3 };
         Assert::AreEqual<size_t>(3, list.size());
         Assert::AreEqual(1, list.front().first);
         Assert::AreEqual(3, list.back().first);
      }

      TEST_METHOD(PushBackAndPopBack)
      {
         qgl::slim_list<int, qgl::win32_srw_traits> list;
         list.push_back(10);
         list.push_back(20);
         Assert::AreEqual<size_t>(2, list.size());
         Assert::AreEqual(20, list.back().first);

         list.pop_back();
         Assert::AreEqual<size_t>(1, list.size());
         Assert::AreEqual(10, list.back().first);
      }

      TEST_METHOD(PushFrontAndPopFront)
      {
         qgl::slim_list<int, qgl::win32_srw_traits> list;
         list.push_front(100);
         list.push_front(50);
         Assert::AreEqual(50, list.front().first);
         list.pop_front();
         Assert::AreEqual(100, list.front().first);
      }

      TEST_METHOD(CopyConstructor_ClonesElements)
      {
         qgl::slim_list<int, qgl::win32_srw_traits> original{ 7, 8, 9 };
         qgl::slim_list<int, qgl::win32_srw_traits> copy{ original };

         Assert::AreEqual(size_t(3), copy.size());
         Assert::AreEqual(7, copy.front().first);
         Assert::AreEqual(9, copy.back().first);
      }

      TEST_METHOD(MoveConstructor_TransfersOwnership)
      {
         qgl::slim_list<int, qgl::win32_srw_traits> original{ 5, 6, 7 };
         qgl::slim_list<int, qgl::win32_srw_traits> moved{ std::move(original) };

         Assert::AreEqual(size_t(3), moved.size());
         Assert::AreEqual(5, moved.front().first);
         Assert::AreEqual(7, moved.back().first);
      }

      TEST_METHOD(Iteration_TraversesAll)
      {
         qgl::slim_list<int, qgl::win32_srw_traits> list{ 1, 2, 3 };
         int sum = 0;
         for (auto it = list.begin(); it != list.end(); ++it)
         {
            sum += *it;
         }

         Assert::AreEqual(6, sum);
      }
   };
}