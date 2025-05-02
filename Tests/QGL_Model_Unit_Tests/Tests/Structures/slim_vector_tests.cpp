#include "pch.h"
#include "CppUnitTest.h"
#include <thread>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace qgl;

namespace QGL_Model_Unit_Tests
{
   TEST_CLASS(SlimVectorTests)
   {
      public:
      TEST_METHOD(DefaultConstructor_IsEmpty)
      {
         qgl::slim_vector<int, qgl::win32_srw_traits> vec;
         Assert::IsTrue(vec.empty());
         Assert::AreEqual<size_t>(0, vec.size());
      }

      TEST_METHOD(InitializerList_ConstructsCorrectly)
      {
         qgl::slim_vector<int, qgl::win32_srw_traits> vec{ 1, 2, 3 };
         Assert::AreEqual<size_t>(3, vec.size());
         Assert::AreEqual(1, vec[0].first);
         Assert::AreEqual(3, vec.back().first);
      }

      TEST_METHOD(PushBack_AppendsElements)
      {
         qgl::slim_vector<int, qgl::win32_srw_traits> vec;
         vec.push_back(10);
         vec.push_back(20);
         Assert::AreEqual<size_t>(2, vec.size());
         Assert::AreEqual(10, vec[0].first);
         Assert::AreEqual(20, vec[1].first);
      }

      TEST_METHOD(At_ThrowsOutOfRange)
      {
         qgl::slim_vector<int, qgl::win32_srw_traits> vec{ 42 };
         Assert::ExpectException<std::out_of_range>([&]()
         {
            vec.at(5);
         });
      }

      TEST_METHOD(FrontBackWork)
      {
         qgl::slim_vector<int, qgl::win32_srw_traits> vec{ 100, 200, 300 };
         Assert::AreEqual(100, vec.front().first);
         Assert::AreEqual(300, vec.back().first);
      }

      TEST_METHOD(Erase_SingleElement)
      {
         qgl::slim_vector<int, qgl::win32_srw_traits> vec{ 1, 2, 3 };
         vec.erase(1);
         Assert::AreEqual<size_t>(2, vec.size());
         Assert::AreEqual(3, vec[1].first);
      }

      TEST_METHOD(Resize_LargerAddsDefaults)
      {
         qgl::slim_vector<int, qgl::win32_srw_traits> vec{ 5 };
         vec.resize(5);
         Assert::AreEqual<size_t>(5, vec.size());
         Assert::AreEqual(5, vec[0].first);  // Original
      }

      TEST_METHOD(Resize_WithValue)
      {
         qgl::slim_vector<int, qgl::win32_srw_traits> vec;
         vec.resize(3, 7);
         Assert::AreEqual(7, vec[1].first);
      }

      TEST_METHOD(Swap_WorksCorrectly)
      {
         qgl::slim_vector<int, qgl::win32_srw_traits> a{ 1, 2 };
         qgl::slim_vector<int, qgl::win32_srw_traits> b{ 3, 4 };
         swap(a, b);
         Assert::AreEqual(3, a[0].first);
         Assert::AreEqual(1, b[0].first);
      }
   };

   TEST_CLASS(SlimVectorConcurrencyTestsBasic)
   {
      public:
      TEST_METHOD(ConcurrentReadsWork)
      {
         qgl::slim_vector<int, qgl::win32_srw_traits> vec{ 1, 2, 3, 4, 5 };
         std::atomic<bool> ready = false;
         std::mutex start_mutex;
         std::condition_variable start_cv;

         auto reader_task = [&](int& out_sum)
         {
            std::unique_lock<std::mutex> lock(start_mutex);
            start_cv.wait(lock, [&]() { return ready.load(); });

            int sum = 0;
            for (auto it = vec.begin(); it != vec.end(); ++it)
            {
               sum += *it;
               std::this_thread::sleep_for(std::chrono::milliseconds(1));
            }
            out_sum = sum;
         };

         int sum1 = 0, sum2 = 0;

         std::thread t1(reader_task, std::ref(sum1));
         std::thread t2(reader_task, std::ref(sum2));

         // Start both readers at the same time
         {
            std::lock_guard<std::mutex> lock(start_mutex);
            ready = true;
         }
         start_cv.notify_all();

         t1.join();
         t2.join();

         Assert::AreEqual(15, sum1);
         Assert::AreEqual(15, sum2);
      }

      TEST_METHOD(WriterBlocksReader)
      {
         qgl::slim_vector<int, qgl::win32_srw_traits> vec{ 10 };
         std::atomic<bool> writer_locked = false;
         std::atomic<bool> reader_done = false;

         std::mutex start_mutex;
         std::condition_variable start_cv;
         bool go = false;

         std::thread writer([&]()
         {
            {
               std::unique_lock<std::mutex> lock(start_mutex);
               start_cv.wait(lock, [&]() { return go; });
            }

            writer_locked = true;
            vec.push_back(20);  // Exclusive lock
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            writer_locked = false;
         });

         std::thread reader([&]()
         {
            {
               std::unique_lock<std::mutex> lock(start_mutex);
               start_cv.wait(lock, [&]() { return go; });
            }

            // This will be blocked if writer has exclusive lock
            auto pair = vec.at(0);  // Shared lock
            reader_done = true;
            (void)pair;
         });

         {
            std::lock_guard<std::mutex> lock(start_mutex);
            go = true;
         }
         start_cv.notify_all();

         writer.join();
         reader.join();

         Assert::IsTrue(reader_done);
      }
   };
}