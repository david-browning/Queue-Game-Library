#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace qgl;

namespace QGL_Model_Unit_Tests
{
   TEST_CLASS(QGLVersionTests)
   {
      public:

      TEST_METHOD(Constructor_SetsFieldsCorrectly)
      {
         qgl_version v(3, 7, char_encodings::multibyte, os_version_flags::linux);

         Assert::AreEqual(uint8_t(3), v.major());
         Assert::AreEqual(uint8_t(7), v.minor());
         Assert::AreEqual((int)char_encodings::multibyte, (int)v.path_encoding());
         Assert::AreEqual((int)os_version_flags::linux, (int)v.os());
      }

      TEST_METHOD(Swap_WorksCorrectly)
      {
         qgl_version a(1, 1, char_encodings::one_byte, os_version_flags::windows);
         qgl_version b(2, 3, char_encodings::two_byte, os_version_flags::mac);

         swap(a, b);

         Assert::AreEqual(uint8_t(2), a.major());
         Assert::AreEqual(uint8_t(3), a.minor());
         Assert::AreEqual((int)char_encodings::two_byte, (int)a.path_encoding());
         Assert::AreEqual((int)os_version_flags::mac, (int)a.os());

         Assert::AreEqual(uint8_t(1), b.major());
         Assert::AreEqual(uint8_t(1), b.minor());
         Assert::AreEqual((int)char_encodings::one_byte, (int)b.path_encoding());
         Assert::AreEqual((int)os_version_flags::windows, (int)b.os());
      }

      TEST_METHOD(Hash_MatchesMajorAndMinorBytes)
      {
         qgl_version v(0x12, 0x34, char_encodings::one_byte, os_version_flags::windows);
         size_t hash = std::hash<qgl_version>{}(v);

         size_t expected = (size_t(0x12) << 8) | 0x34;
         Assert::AreEqual(expected, hash);
      }

      TEST_METHOD(DefaultConstructor_ZeroFields)
      {
         constexpr qgl_version v;
         Assert::AreEqual(uint8_t(0), v.major());
         Assert::AreEqual(uint8_t(0), v.minor());
         Assert::AreEqual((int)char_encodings::one_byte, (int)v.path_encoding());  // defaults to 0
         Assert::AreEqual((int)os_version_flags::unknown, (int)v.os());
      }

      TEST_METHOD(StaticVersionConstants_ValidateFields)
      {
         Assert::AreEqual(QGL_VERSION_MAJOR_0, QGL_VERSION_0_1_WIN.major());
         Assert::AreEqual(QGL_VERSION_MINOR_1, QGL_VERSION_0_1_WIN.minor());
         Assert::AreEqual((int)char_encodings::two_byte, (int)QGL_VERSION_0_1_WIN.path_encoding());
         Assert::AreEqual((int)os_version_flags::windows, (int)QGL_VERSION_0_1_WIN.os());

         Assert::AreEqual(QGL_VERSION_MAJOR_0, QGL_VERSION_0_2_WIN.major());
         Assert::AreEqual(uint8_t(2), QGL_VERSION_0_2_WIN.minor());
         Assert::AreEqual(QGL_VERSION_0_2_WIN.major(), QGL_VERSION_LATEST.major());
         Assert::AreEqual(QGL_VERSION_0_2_WIN.minor(), QGL_VERSION_LATEST.minor());
      }

      TEST_METHOD(StaticHashes_AreCorrect)
      {
         constexpr size_t hash_0_1 = (QGL_VERSION_MAJOR_0 << 8) | QGL_VERSION_MINOR_1;
         constexpr size_t hash_0_2 = (QGL_VERSION_MAJOR_0 << 8) | 2;

         Assert::AreEqual(hash_0_1, qgl::hashes::VERSION_0_1_HASH);
         Assert::AreEqual(hash_0_2, qgl::hashes::VERSION_0_2_HASH);
      }
   };
}
