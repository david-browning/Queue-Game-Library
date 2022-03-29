#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace qgl;

namespace QGL_Model_Unit_Tests
{
   TEST_CLASS(FixedBufferTests)
   {
      TEST_METHOD(EmptyConstructor)
      {
         fixed_buffer<int, 4> b;
         Assert::AreEqual(0, b.at(0), L"Element 0 is not 0.");
         Assert::AreEqual(size_t(4), b.size(), L"Size is not 4.");
      }

      TEST_METHOD(CopyPointerConstructor)
      {
         std::string s{ "Hello World" };
         fixed_buffer<char, 32> b{ s.c_str(), s.size() };
         Assert::AreEqual('H', b.at(0), L"Element 0 is not 'H'.");
         Assert::AreEqual('\0', b.at(s.size()), L"Does not end with null.");
      }

      TEST_METHOD(CopyConstructor)
      {
         std::string s{ "Hello World" };
         fixed_buffer<char, 32> b{ s.c_str(), s.size() };
         fixed_buffer<char, 32> bCopy{ b };

         Assert::AreEqual('H', bCopy.at(0), L"Element 0 is not 'H'.");
         Assert::AreEqual('\0', bCopy.at(s.size()), L"Does not end with null.");
      }

      TEST_METHOD(MoveConstructor)
      {
         std::string s{ "Hello World" };
         fixed_buffer<char, 32> b{ s.c_str(), s.size() };
         fixed_buffer<char, 32> bCopy{ b };
         fixed_buffer<char, 32> bMoved{ std::move(bCopy) };

         Assert::AreEqual('H', bMoved.at(0), L"Element 0 is not 'H'.");
         Assert::AreEqual('\0', bMoved.at(s.size()), L"Does not end with null.");
      }

      TEST_METHOD(CopyAssign)
      {
         std::string s{ "Hello World" };
         fixed_buffer<char, 32> b{ s.c_str(), s.size() };
         auto bCopy = b;

         Assert::AreEqual('H', bCopy.at(0), L"Element 0 is not 'H'.");
         Assert::AreEqual('\0', bCopy.at(s.size()), L"Does not end with null.");
      }

      TEST_METHOD(AtThrows)
      {
         fixed_buffer<int, 4> b;
         auto call = [&] { b.at(4); };
         Assert::ExpectException<std::out_of_range>(call);
      }
   };
}