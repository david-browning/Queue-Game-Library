#include "pch.h"
#include <CppUnitTest.h>
#include <CppUnitTestLogger.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace qgl;

namespace QGL_Model_Unit_Tests
{
   TEST_CLASS(MiscHelperTests)
   {
      TEST_METHOD(AlternateEmpty)
      {
         std::vector<int> empty;
         qgl::alternate(empty.begin(), empty.end());
         Assert::IsTrue(empty.empty(), L"Empty vector should be empty.");
      }

      TEST_METHOD(Alternate1)
      {
         std::array<int, 1> a1 = { 1 };
         qgl::alternate(a1.begin(), a1.end());
         Assert::AreEqual(a1[0], 1, L"A1's 0th element should be 1.");
      }

      TEST_METHOD(Alternate2)
      {
         std::array<int, 2> a2 = { 0, 1 };
         std::array<int, 2> a2Expected = { 0, 1 };
         qgl::alternate(a2.begin(), a2.end());
         array_equal(a2Expected, a2);
      }

      TEST_METHOD(Alternate3)
      {
         std::array<int, 3> a3 = { 0, 1, 2 };
         std::array<int, 3> a3Expected = { 0, 2, 1 };
         qgl::alternate(a3.begin(), a3.end());
         array_equal(a3Expected, a3);
      }
      
      TEST_METHOD(AlternateEven)
      {
         std::array<int, 6> a4 = { 0, 1, 2, 3, 4, 5 };
         std::array<int, 6> a4Expected = { 0, 5, 1, 4, 2, 3 };
         qgl::alternate(a4.begin(), a4.end());
         array_equal(a4Expected, a4);
      }

      TEST_METHOD(AlternateOdd)
      {
         std::array<int, 12> a5 = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
         std::array<int, 12> a5Expected = { 0, 11, 1, 10, 2, 9, 3, 8, 4, 7, 5, 6 };
         qgl::alternate(a5.begin(), a5.end());
         array_equal(a5Expected, a5);
      }

      /*
       This test is only valid on PC.
       */
      TEST_METHOD(HWType)
      {
         auto expected = hw_types::hw_type_pc;
         Assert::AreEqual(expected, hw_type(),
            L"The hardware type should be PC.");
      }

      private:
      template<typename T, size_t Count>
      void array_equal(
         const std::array<T, Count>& expected,
         const std::array<T, Count>& actual)
      {
         for (size_t i = 0; i < Count; i++)
         {
            std::wstringstream stream;
            stream << "Index " << i;
            Assert::AreEqual(expected[i], actual[i], stream.str().c_str());
         }
      }
   };
}