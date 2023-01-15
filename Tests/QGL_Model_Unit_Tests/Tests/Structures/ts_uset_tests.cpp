#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace qgl;

namespace std
{
   template<>
   struct hash<std::function<void(int)>>
   {
      size_t operator()(std::function<void(int)> f)
      {
         return (size_t)f.target<void(*)(int)>();
      }
   };
}

namespace QGL_Model_Unit_Tests
{
   inline void UmapFunctor(int x)
   {
      x *= 2;
   }

   inline void UmapFunctor2(int x)
   {
      x *= 2;
   }

   inline void UmapFunctor3(int x)
   {
      x *= 2;
   }

   TEST_CLASS(TSUMapTests)
   {
      TEST_METHOD(Construct)
      {
         ts_uset<int> s;
         Assert::AreEqual(size_t{ 0 }, s.size(),
                          L"Set should be empty.");
      }

      TEST_METHOD(ConstructFunctor)
      {
         ts_uset<void(*)(int)> f;
         Assert::AreEqual(size_t{ 0 }, f.size(),
                          L"Set should be empty.");

         f.insert(&UmapFunctor);
         f.insert(&UmapFunctor2);
         Assert::AreEqual(size_t{ 2 }, f.size(),
                          L"Set should have two elements.");
      }

      TEST_METHOD(CopyConstruct)
      {
         ts_uset<void(*)(int)> f;
         f.insert(&UmapFunctor);
         auto fCopy{ f };
         Assert::AreEqual(f.size(), fCopy.size(),
                          L"Copied set does not have the same size.");
      }

      TEST_METHOD(MoveConstruct)
      {
         ts_uset<void(*)(int)> f;
         f.insert(&UmapFunctor);
         auto fCopy{ f };
         auto fMove{ std::move(fCopy) };

         Assert::AreEqual(f.size(), fMove.size(),
                          L"Copied set does not have the same size.");

      }

      TEST_METHOD(CopyAssign)
      {
         ts_uset<void(*)(int)> f;
         f.insert(&UmapFunctor);
         auto fCopy = f;
         Assert::AreEqual(f.size(), fCopy.size(),
                          L"Copied set does not have the same size.");
      }

      TEST_METHOD(Empty)
      {
         ts_uset<int> s;
         Assert::IsTrue(s.empty(),
                        L"Set should be empty.");

         s.insert(1);
         Assert::IsFalse(s.empty(),
                         L"Set should not be empty.");
      }

      TEST_METHOD(Size)
      {
         ts_uset<int> s;
         s.insert(1);
         s.insert(2);
         Assert::AreEqual(size_t{ 2 }, s.size(),
                          L"Set should have 2 elements.");

         s.insert(1);
         Assert::AreEqual(size_t{ 2 }, s.size(),
                          L"Set should have 2 elements.");
      }

      TEST_METHOD(Clear)
      {
         ts_uset<int> s;
         s.insert(1);
         s.insert(2);
         Assert::AreEqual(size_t{ 2 }, s.size(),
                          L"Set should have 2 elements.");
         s.clear();
         Assert::IsTrue(s.empty(),
               L"Set should be empty.");
      }

      TEST_METHOD(InsertErase)
      {
         ts_uset<int> s;
         s.insert(1);
         s.insert(2);
         s.erase(2);
         Assert::AreEqual(size_t{ 1 }, s.size(),
                          L"Set should have 1 element.");
      }

      TEST_METHOD(Iterate)
      {
         std::vector<int> ints = { 1, 2, 3, 4 };
      }

      TEST_METHOD(Find)
      {
         ts_uset<void(*)(int)> f;
         f.insert(&UmapFunctor);
         f.insert(&UmapFunctor2);

         Assert::IsTrue(f.find(&UmapFunctor2) != f.cend(),
                        L"Find should not have returned end()");

         Assert::IsTrue(f.find(UmapFunctor3) == f.cend(),
                        L"Find should have returned end()");
      }
   };
}