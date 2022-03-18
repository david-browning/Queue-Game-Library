#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace qgl;

namespace QGL_Model_Unit_Tests
{
   TEST_CLASS(GUIDTests)
   {
      /*
       No assumptions.
       */
      TEST_METHOD(Constructor)
      {
         // Empty guid.
         {
            uint32_t expectedTimeLow = 0;
            uint16_t expectedTimeMid = 0;
            uint16_t expectedTimeHigh = 0;
            uint16_t expectedClock = 0;
            uint64_t expectedNode = 0;

            guid g;

            Assert::AreEqual(expectedTimeLow, g.time_low(),
               L"time_low is not expected.");
            Assert::AreEqual(expectedTimeMid, g.time_mid(),
               L"time_mid is not expected.");
            Assert::AreEqual(expectedTimeHigh, g.time_high(),
               L"time_high is not expected.");
            Assert::AreEqual(expectedClock, g.clock(),
               L"clock is not expected.");
            Assert::AreEqual(expectedNode, g.node(),
               L"node is not expected.");
         }

         // construct with these values as an std::array
         // 00 11 22 33 44 55 66 77 88 99 AA BB CC DD EE FF
         {
            uint32_t expectedTimeLow = 0x00112233;
            uint16_t expectedTimeMid = 0x4455;
            uint16_t expectedTimeHigh = 0x6677;
            uint16_t expectedClock = 0x8899;
            uint64_t expectedNode = 0x0000AABBCCDDEEFF;

            guid g{ input };

            Assert::AreEqual(expectedTimeLow, g.time_low(),
               L"time_low is not expected.");
            Assert::AreEqual(expectedTimeMid, g.time_mid(),
               L"time_mid is not expected.");
            Assert::AreEqual(expectedTimeHigh, g.time_high(),
               L"time_high is not expected.");
            Assert::AreEqual(expectedClock, g.clock(),
               L"clock is not expected.");
            Assert::AreEqual(expectedNode, g.node(),
               L"node is not expected.");
         }
      }

      /*
       Assume constructor and property access works.
       */
      TEST_METHOD(CopyConstructor)
      {
         guid g{ input };
         guid gCopy{ g };

         Assert::AreEqual(gCopy.time_low(), g.time_low(),
            L"time_low is not expected.");
         Assert::AreEqual(gCopy.time_mid(), g.time_mid(),
            L"time_mid is not expected.");
         Assert::AreEqual(gCopy.time_high(), g.time_high(),
            L"time_high is not expected.");
         Assert::AreEqual(gCopy.clock(), g.clock(),
            L"clock is not expected.");
         Assert::AreEqual(gCopy.node(), g.node(),
            L"node is not expected.");
      }

      /*
       Assume CopyConstructor works.
       */
      TEST_METHOD(MoveConstructor)
      {
         guid g{ input };
         guid gCopy{ g };
         guid gMove{ std::move(gCopy) };

         Assert::AreEqual(gMove.time_low(), g.time_low(),
            L"time_low is not expected.");
         Assert::AreEqual(gMove.time_mid(), g.time_mid(),
            L"time_mid is not expected.");
         Assert::AreEqual(gMove.time_high(), g.time_high(),
            L"time_high is not expected.");
         Assert::AreEqual(gMove.clock(), g.clock(),
            L"clock is not expected.");
         Assert::AreEqual(gMove.node(), g.node(),
            L"node is not expected.");
      }

      /*
       Assume CopyConstructor and MoveConstructor work.
       */
      TEST_METHOD(AssignOperator)
      {
         guid g{ input };
         guid gCopy = g;

         Assert::AreEqual(gCopy.time_low(), g.time_low(),
            L"time_low is not expected.");
         Assert::AreEqual(gCopy.time_mid(), g.time_mid(),
            L"time_mid is not expected.");
         Assert::AreEqual(gCopy.time_high(), g.time_high(),
            L"time_high is not expected.");
         Assert::AreEqual(gCopy.clock(), g.clock(),
            L"clock is not expected.");
         Assert::AreEqual(gCopy.node(), g.node(),
            L"node is not expected.");

         guid gMove = std::move(gCopy);
         Assert::AreEqual(gMove.time_low(), g.time_low(),
            L"time_low is not expected.");
         Assert::AreEqual(gMove.time_mid(), g.time_mid(),
            L"time_mid is not expected.");
         Assert::AreEqual(gMove.time_high(), g.time_high(),
            L"time_high is not expected.");
         Assert::AreEqual(gMove.clock(), g.clock(),
            L"clock is not expected.");
         Assert::AreEqual(gMove.node(), g.node(),
            L"node is not expected.");
      }

      /*
       Assume AssignOperator works.
       */
      TEST_METHOD(EqualityOperator)
      {
         guid g{ input };
         guid gCopy = g;

         Assert::IsTrue(g == gCopy, L"guids should be equal.");
         Assert::IsFalse(g != gCopy, L"guids should not be equal.");

         guid empty;
         Assert::IsTrue(g != empty, L"guids should not be equal.");
         Assert::IsFalse(g == empty, L"guids should not be equal.");
      }

      /*
       Assume constructor works.
       */
      TEST_METHOD(LessThanOperator)
      {
         static constexpr std::array<uint8_t, guid::UUID_BYTES> input2 =
         {
            0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x66,
            0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF,
         };

         static constexpr std::array<uint8_t, guid::UUID_BYTES> input3 =
         {
            0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x66,
            0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xEE,
         };

         guid g1{ input };
         guid g2{ input2 };
         guid g3{ input3 };

         Assert::IsTrue(g2 < g1, L"g2 should be less than g1.");
         Assert::IsTrue(g3 < g1, L"g3 should be less than g1.");
         Assert::IsTrue(g3 < g2, L"g3 should be less than g2.");
      }

      /*
       Assume constructor works.
       */
      TEST_METHOD(GreaterThanOperator)
      {
         static constexpr std::array<uint8_t, guid::UUID_BYTES> input2 =
         {
            0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x88,
            0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF,
         };

         static constexpr std::array<uint8_t, guid::UUID_BYTES> input3 =
         {
            0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x66,
            0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xFF, 0xFF,
         };

         guid g1{ input };
         guid g2{ input2 };
         guid g3{ input3 };

         Assert::IsTrue(g2 > g1, L"g2 should be greater than g1.");
         Assert::IsTrue(g3 > g1, L"g3 should be greater than g1.");
         Assert::IsTrue(g3 > g2, L"g3 should be greater than g2.");
      }

      TEST_METHOD(String)
      {
         guid g{ input };
         std::string expected = "00112233-4455-6677-8899-AABBCCDDEEFF";
         Assert::AreEqual(expected, g.str<char>(),
            L"String is not equal.");
      }

      /*
       Assume String works.
       */
      TEST_METHOD(OStreamOperator)
      {
         guid g{ input };
         std::string expected = "00112233-4455-6677-8899-AABBCCDDEEFF";

         std::stringstream s;
         s << g;

         Assert::AreEqual(expected, s.str(),
            L"String is not equal.");
      }

      /*
       Assume EqualityOperator works.
       */
      TEST_METHOD(CreateRandom)
      {
         auto g1 = qgl::random_guid();
         auto g2 = qgl::random_guid();
         Assert::IsTrue(g1 != g2, L"Random GUIDs should not be equal.");
      }

      private:
      static constexpr std::array<uint8_t, guid::UUID_BYTES> input =
      {
         0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
         0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF,
      };
   };
}