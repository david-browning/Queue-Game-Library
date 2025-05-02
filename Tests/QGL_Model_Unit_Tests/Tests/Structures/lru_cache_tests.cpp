#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace qgl;

namespace QGL_Model_Unit_Tests
{
   class test_sizer
   {
      public:
      size_t operator()(const std::string& s) const
      {
         return s.size(); // Each character = 1 byte
      }
   };

   TEST_CLASS(LRUCacheTests)
   {
      using cache_t = qgl::lru_cache<std::string, std::string, test_sizer>;

      public:
      TEST_METHOD(PutAndGet)
      {
         cache_t cache(1024);
         cache.put("key1", "value1");

         Assert::AreEqual<std::string>("value1", cache.get("key1"));
      }

      TEST_METHOD(PutReplacesExisting)
      {
         cache_t cache(1024);
         cache.put("k", "short");
         cache.put("k", "longer");

         Assert::AreEqual(std::string("longer"), cache.get("k"));
      }

      TEST_METHOD(CapacityAndSize)
      {
         cache_t cache(50); // bytes
         cache.put("x", "12345"); // 5
         cache.put("y", "abcde"); // 5

         Assert::AreEqual<size_t>(10, cache.size());
         Assert::AreEqual<size_t>(50, cache.capacity());
         Assert::IsFalse(cache.full());
      }

      TEST_METHOD(CachedReturnsTrueIfKeyPresent)
      {
         cache_t cache(64);
         cache.put("present", "yes");
         Assert::IsTrue(cache.cached("present"));
         Assert::IsFalse(cache.cached("absent"));
      }

      TEST_METHOD(ThrowsIfKeyNotCached)
      {
         cache_t cache(64);
         auto call = [&] {
           auto val = cache.get("not_found");
         };

         Assert::ExpectException<qgl::not_cached<std::string>>(call);
      }

      TEST_METHOD(FrontBackWorkCorrectly)
      {
         cache_t cache(64);
         cache.put("k1", "v1");
         cache.put("k2", "v2");
         cache.put("k3", "v3");

         auto& front = cache.front();
         auto& back = cache.back();

         Assert::AreEqual<std::string>("v3", front);
         Assert::AreEqual<std::string>("v1", back);
      }

      TEST_METHOD(EvictBackRemovesOldest)
      {
         cache_t cache(64);
         cache.put("old", "A");
         cache.put("new", "B");

         cache.evict_back();

         Assert::IsFalse(cache.cached("old"));
         Assert::IsTrue(cache.cached("new"));
      }

      TEST_METHOD(InsertBigObject_EvictsEnough)
      {
         cache_t cache(10);
         cache.put("1", "a");
         cache.put("2", "b");
         cache.put("3", "c");

         cache.put("big", "1234567890"); // size = 10

         Assert::IsTrue(cache.cached("big"));
         Assert::AreEqual(size_t(10), cache.size());
         Assert::IsFalse(cache.cached("1")); // Should be evicted
      }

      TEST_METHOD(ReplaceWithLargerObjectTriggersEviction)
      {
         cache_t cache(15);
         cache.put("k", "12345"); // 5 bytes
         cache.put("k", "abcdefghijklmno"); // 15 bytes

         Assert::AreEqual(size_t(15), cache.size());
         Assert::AreEqual(std::string("abcdefghijklmno"), cache.get("k"));
      }

      TEST_METHOD(CopyConstructorAndAssignment)
      {
         cache_t original(100);
         original.put("k1", "v1");
         original.put("k2", "v2");

         cache_t copy{ original };
         Assert::AreEqual(std::string("v1"), copy.get("k1"));

         cache_t assigned(50);
         assigned = original;
         Assert::AreEqual(std::string("v2"), assigned.get("k2"));
      }

      TEST_METHOD(MoveConstructorAndAssignment)
      {
         cache_t source(50);
         source.put("a", "b");

         cache_t moved{ std::move(source) };
         Assert::AreEqual(std::string("b"), moved.get("a"));

         cache_t temp(100);
         temp.put("x", "y");

         moved = std::move(temp);
         Assert::AreEqual(std::string("y"), moved.get("x"));
      }
      TEST_METHOD(EvictWhenFull)
      {
         struct FixedSize
         {
            size_t operator()(const int&) const { return 50; }
         };

         lru_cache<std::string, int, FixedSize> cache(100);
         cache.put("a", 1);
         cache.put("b", 2);
         cache.put("c", 3); // Should evict "a"

         try
         {
            cache.get("a");
            Assert::Fail(L"Expected exception for missing key.");
         }
         catch (const qgl::not_cached<std::string>&)
         {
            // Expected
         }

         Assert::AreEqual(2, cache.get("b"));
         Assert::AreEqual(3, cache.get("c"));
      }

      TEST_METHOD(AccessUpdatesOrder)
      {
         lru_cache<std::string, int> cache(300);
         cache.put("a", 1);
         cache.put("b", 2);
         cache.put("c", 3);

         cache.get("a"); // Make "a" most recently used

         Assert::AreEqual(1, cache.front()); // MRU
         Assert::AreEqual(2, cache.back());  // LRU
      }

      TEST_METHOD(Iterator)
      {
         lru_cache<std::string, int> cache(300);
         cache.put("a", 1);
         cache.put("b", 2);
         cache.put("c", 3);

         int sum = 0;
         for (auto& [k, v] : cache)
         {
            sum += v;
         }

         Assert::AreEqual(6, sum);
      }

      TEST_METHOD(ConstIterator)
      {
         const lru_cache<std::string, int> cache = []
         {
            lru_cache<std::string, int> c(300);
            c.put("a", 1);
            c.put("b", 2);
            return c;
         }();

         int sum = 0;
         for (auto it = cache.cbegin(); it != cache.cend(); ++it)
         {
            auto [k, v] = *it;
            sum += v;
         }

         Assert::AreEqual(3, sum);
      }
   };
}