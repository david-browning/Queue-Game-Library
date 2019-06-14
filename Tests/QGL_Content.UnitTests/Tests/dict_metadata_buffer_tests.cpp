#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace qgl::content;

namespace QGL_Content_UnitTests
{
   TEST_CLASS(DictionaryMetadataTests)
   {
      public:
      TEST_METHOD(DefaultConstructor)
      {
         CONTENT_DICTIONARY_METADATA_BUFFER m;
         Assert::AreEqual(static_cast<size_t>(0), m.count(),
                          L"There should be no dictionary entries.");

         Assert::AreEqual(static_cast<size_t>(0), m.entry_size(),
                          L"The entry size should be zero.");
      }

      TEST_METHOD(Constructor)
      {
         CONTENT_DICTIONARY_METADATA_BUFFER
            m(8, sizeof(CONTENT_DICTIONARY_ENTRY_BUFFER));

         Assert::AreEqual(static_cast<size_t>(8), m.count(),
                          L"The number of entries is not correct.");

         Assert::AreEqual(sizeof(CONTENT_DICTIONARY_ENTRY_BUFFER),
                          m.entry_size(),
                          L"The entry size is not correct.");

         Assert::AreEqual(static_cast<uint64_t>(0),
                          m.flags(),
                          L"The flags are not correct.");
      }

      TEST_METHOD(CopyConstructor)
      {
         CONTENT_DICTIONARY_METADATA_BUFFER
            m(8, sizeof(CONTENT_DICTIONARY_ENTRY_BUFFER));

         CONTENT_DICTIONARY_METADATA_BUFFER mCopy(m);
         Assert::AreEqual(static_cast<size_t>(8), mCopy.count(),
                          L"The number of entries is not correct.");

         Assert::AreEqual(sizeof(CONTENT_DICTIONARY_ENTRY_BUFFER),
                          mCopy.entry_size(),
                          L"The entry size is not correct.");

         Assert::AreEqual(static_cast<uint64_t>(0),
                          mCopy.flags(),
                          L"The flags are not correct.");
      }

      TEST_METHOD(MoveConstructor)
      {
         CONTENT_DICTIONARY_METADATA_BUFFER
            m(8, sizeof(CONTENT_DICTIONARY_ENTRY_BUFFER));

         CONTENT_DICTIONARY_METADATA_BUFFER mMove(std::move(m));
         Assert::AreEqual(static_cast<size_t>(8), mMove.count(),
                          L"The number of entries is not correct.");

         Assert::AreEqual(sizeof(CONTENT_DICTIONARY_ENTRY_BUFFER),
                          mMove.entry_size(),
                          L"The entry size is not correct.");

         Assert::AreEqual(static_cast<uint64_t>(0),
                          mMove.flags(),
                          L"The flags are not correct.");
      }

      TEST_METHOD(CountCorrect)
      {
         CONTENT_DICTIONARY_METADATA_BUFFER
            m(8, sizeof(CONTENT_DICTIONARY_ENTRY_BUFFER));

         CONTENT_DICTIONARY_METADATA_BUFFER mCopy(m);
         Assert::AreEqual(static_cast<size_t>(8), mCopy.count(),
                          L"The number of entries is not correct.");
      }

      TEST_METHOD(EntrySizeCorrect)
      {
         CONTENT_DICTIONARY_METADATA_BUFFER
            m(8, sizeof(CONTENT_DICTIONARY_ENTRY_BUFFER));

         Assert::AreEqual(sizeof(CONTENT_DICTIONARY_ENTRY_BUFFER),
                          m.entry_size(),
                          L"The entry size is not correct.");

      }

      TEST_METHOD(FlagsCorrect)
      {
         CONTENT_DICTIONARY_METADATA_BUFFER
            m(8, sizeof(CONTENT_DICTIONARY_ENTRY_BUFFER));

         Assert::AreEqual(static_cast<uint64_t>(0),
                          m.flags(),
                          L"The flags are not correct.");
      }

      TEST_METHOD(AssignOperator)
      {
         CONTENT_DICTIONARY_METADATA_BUFFER
            m(8, sizeof(CONTENT_DICTIONARY_ENTRY_BUFFER));

         CONTENT_DICTIONARY_METADATA_BUFFER mCopy = m;
         Assert::AreEqual(static_cast<size_t>(8), mCopy.count(),
                          L"The number of entries is not correct.");

         Assert::AreEqual(sizeof(CONTENT_DICTIONARY_ENTRY_BUFFER),
                          mCopy.entry_size(),
                          L"The entry size is not correct.");

         Assert::AreEqual(static_cast<uint64_t>(0),
                          mCopy.flags(),
                          L"The flags are not correct.");
      }

      TEST_METHOD(EqualityOperator)
      {
         CONTENT_DICTIONARY_METADATA_BUFFER
            m(8, sizeof(CONTENT_DICTIONARY_ENTRY_BUFFER));

         CONTENT_DICTIONARY_METADATA_BUFFER mCopy(m);
         Assert::IsTrue(m == mCopy,
                        L"The buffers should be equal.");

         CONTENT_DICTIONARY_METADATA_BUFFER mNotEqual;
         Assert::IsFalse(m == mNotEqual,
                         L"The buffers should not be equal.");
      }
   };
}