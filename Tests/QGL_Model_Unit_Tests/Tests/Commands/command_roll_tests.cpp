#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace qgl;

namespace QGL_Model_Unit_Tests
{
   TEST_CLASS(CommandRollTests)
   {
      public:

      TEST_METHOD(RecordAndSize)
      {
         command_roll<int> roll;
         Assert::AreEqual(0ull, roll.size());

         roll.record(guid("00112233445566778899AABBCCDDEEFF"), 10);
         roll.record(guid("0102030405060708090A0B0C0D0E0F00"), 20);

         Assert::AreEqual(2ull, roll.size());
      }

      TEST_METHOD(Clear)
      {
         command_roll<int> roll;
         roll.record(guid("AAAABBBBCCCCDDDD1111222233334444"), 42);
         roll.clear();

         Assert::AreEqual(0ull, roll.size());
      }

      TEST_METHOD(SortOrdersChronologically)
      {
         command_roll<int> roll;
         roll.record(guid("00000000000000000000000000000010"), 50);
         roll.record(guid("00000000000000000000000000000011"), 10);
         roll.record(guid("00000000000000000000000000000012"), 30);
         roll.sort();

         auto it = roll.begin();
         Assert::AreEqual(10, it->timestamp); ++it;
         Assert::AreEqual(30, it->timestamp); ++it;
         Assert::AreEqual(50, it->timestamp);
      }

      TEST_METHOD(PopBack)
      {
         command_roll<int> roll;
         roll.record(guid("00000000000000000000000000000010"), 1);
         roll.record(guid("00000000000000000000000000000011"), 2);
         roll.pop_back();

         Assert::AreEqual(1ull, roll.size());

         auto it = roll.begin();
         Assert::AreEqual(1, it->timestamp);
      }

      TEST_METHOD(ForwardIteration)
      {
         command_roll<int> roll;
         roll.record(guid("00000000000000000000000000000010"), 10);
         roll.record(guid("00000000000000000000000000000011"), 20);
         roll.record(guid("00000000000000000000000000000012"), 30);

         std::vector<int> times;
         for (auto& f : roll)
         {
            times.push_back(f.timestamp);
         }

         Assert::AreEqual(3ull, times.size());
         Assert::AreEqual(10, times[0]);
         Assert::AreEqual(20, times[1]);
         Assert::AreEqual(30, times[2]);
      }

      TEST_METHOD(ReverseIteration)
      {
         command_roll<int> roll;
         roll.record(guid("00000000000000000000000000000010"), 100);
         roll.record(guid("00000000000000000000000000000011"), 200);

         std::vector<int> times;
         for (auto it = roll.rbegin(); it != roll.rend(); ++it)
         {
            times.push_back(it->timestamp);
         }

         Assert::AreEqual(2ull, times.size());
         Assert::AreEqual(200, times[0]);
         Assert::AreEqual(100, times[1]);
      }

      TEST_METHOD(ConstIteration)
      {
         command_roll<int> roll;
         roll.record(guid("00000000000000000000000000000010"), 1);
         roll.record(guid("00000000000000000000000000000011"), 2);

         const auto& cref = roll;
         int total = 0;
         for (auto& f : cref)
         {
            total += f.timestamp;
         }

         Assert::AreEqual(3, total);
      }

      TEST_METHOD(ConstReverseIteration)
      {
         command_roll<int> roll;
         roll.record(guid("00000000000000000000000000000010"), 5);
         roll.record(guid("00000000000000000000000000000011"), 15);

         const auto& cref = roll;
         auto it = cref.crbegin();
         Assert::AreEqual(15, it->timestamp);
         ++it;
         Assert::AreEqual(5, it->timestamp);
      }
   };
}