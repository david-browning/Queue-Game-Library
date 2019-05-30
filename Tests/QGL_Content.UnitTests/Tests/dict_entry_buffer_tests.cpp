#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace qgl::content;

namespace QGL_Content_UnitTests
{
   TEST_CLASS(DictionaryEntryTests)
   {
      public:
      TEST_METHOD(DefaultConstructor)
      {
         CONTENT_DICTIONARY_ENTRY_BUFFER e;
         Assert::AreEqual(static_cast<size_t>(0),
                          e.size(),
                          L"The data size is not correct.");

         Assert::AreEqual(static_cast<size_t>(-1),
                          e.offset(),
                          L"The offset is not correct.");

         Assert::AreEqual(static_cast<uint64_t>(0),
                          e.flags(),
                          L"The flags are not correct.");
         
         Assert::IsFalse(e.shared(),
                         L"The entry should not be shared.");

         const auto m = e.metadata();

         Assert::IsTrue(CONTENT_LOADER_IDS::CONTENT_LOADER_ID_UNKNOWN ==
                        static_cast<CONTENT_LOADER_IDS>(m->loader_id()),
                        L"The loader should be unknown.");

         Assert::IsTrue(winrt::hstring() == m->name(),
                        L"The name should be empty.");

         Assert::AreEqual(qgl::QGL_VERSION_LATEST,
                          m->version(),
                          L"The version is not correct.");

         Assert::IsTrue(m->content_visible(),
                        L"The content should be visible.");

         Assert::IsFalse(m->obey_physics(),
                         L"The content should not obey physics.");

      }

      TEST_METHOD(Constructor)
      {
         CONTENT_METADATA_BUFFER m(RESOURCE_TYPES::RESOURCE_TYPE_BRUSH,
                                   CONTENT_LOADER_IDS::CONTENT_LOADER_ID_BRUSH,
                                   L"Name");

         CONTENT_DICTIONARY_ENTRY_BUFFER e(8,
                                           &m);

         Assert::AreEqual(static_cast<size_t>(8),
                          e.size(),
                          L"The data size is not correct.");

         Assert::AreEqual(static_cast<size_t>(-1),
                          e.offset(),
                          L"The offset is not correct.");

         Assert::AreEqual(static_cast<uint64_t>(0),
                          e.flags(),
                          L"The flags are not correct.");
      }

      TEST_METHOD(ConstructorWithEntryOffset)
      {
         CONTENT_METADATA_BUFFER m(RESOURCE_TYPES::RESOURCE_TYPE_BRUSH,
                                   CONTENT_LOADER_IDS::CONTENT_LOADER_ID_BRUSH,
                                   L"Name");

         CONTENT_DICTIONARY_ENTRY_BUFFER e(8,
                                           &m,
                                           16);

         Assert::AreEqual(static_cast<size_t>(8),
                          e.size(),
                          L"The data size is not correct.");

         Assert::AreEqual(static_cast<size_t>(16),
                          e.offset(),
                          L"The offset is not correct.");

         Assert::AreEqual(static_cast<uint64_t>(0),
                          e.flags(),
                          L"The flags are not correct.");
      }

      TEST_METHOD(CopyConstructor)
      {
         CONTENT_METADATA_BUFFER m(RESOURCE_TYPES::RESOURCE_TYPE_BRUSH,
                                   CONTENT_LOADER_IDS::CONTENT_LOADER_ID_BRUSH,
                                   L"Name");

         CONTENT_DICTIONARY_ENTRY_BUFFER e(8,
                                           &m,
                                           16);

         CONTENT_DICTIONARY_ENTRY_BUFFER eCopy(e);

         Assert::AreEqual(e.flags(), eCopy.flags(),
                          L"The flags are not correct.");

         Assert::AreEqual(e.offset(), eCopy.offset(),
                          L"The offset is not correct.");

         Assert::AreEqual(e.size(), eCopy.size(),
                          L"The data size is not correct.");

         Assert::IsTrue(*e.metadata() == *eCopy.metadata(),
                        L"The metadata is not correct.");
      }

      TEST_METHOD(MoveConstructor)
      {
         CONTENT_METADATA_BUFFER m(RESOURCE_TYPES::RESOURCE_TYPE_BRUSH,
                                   CONTENT_LOADER_IDS::CONTENT_LOADER_ID_BRUSH,
                                   L"Name");

         CONTENT_DICTIONARY_ENTRY_BUFFER e(8,
                                           &m,
                                           16);

         CONTENT_DICTIONARY_ENTRY_BUFFER eCopy(e);
         CONTENT_DICTIONARY_ENTRY_BUFFER eMove(eCopy);

         Assert::AreEqual(e.flags(), eMove.flags(),
                          L"The flags are not correct.");

         Assert::AreEqual(e.offset(), eMove.offset(),
                          L"The offset is not correct.");

         Assert::AreEqual(e.size(), eMove.size(),
                          L"The data size is not correct.");

         Assert::IsTrue(*e.metadata() == *eMove.metadata(),
                        L"The metadata is not correct.");
      }

      TEST_METHOD(OffsetGet)
      {
         //Tested by constructor tests.
      }

      TEST_METHOD(DataSizeGet)
      {
         //Tested by constructor tests.
      }

      TEST_METHOD(FlagsGet)
      {
         //Tested by constructor tests.
      }

      TEST_METHOD(SharedSet)
      {
         CONTENT_DICTIONARY_ENTRY_BUFFER e;
         e.shared(true);
         Assert::IsTrue(e.shared(),
                        L"Entry should be shared.");

         e.shared(false);
         Assert::IsFalse(e.shared(),
                         L"Entry should not be shared.");
      }

      TEST_METHOD(MetadataGet)
      {
         CONTENT_METADATA_BUFFER m(RESOURCE_TYPES::RESOURCE_TYPE_BRUSH,
                                   CONTENT_LOADER_IDS::CONTENT_LOADER_ID_BRUSH,
                                   L"Name");

         CONTENT_DICTIONARY_ENTRY_BUFFER e(8,
                                           &m,
                                           16);

         Assert::IsTrue(m == *e.metadata(),
                        L"The metadata is not correct.");
      }

      TEST_METHOD(AssignOperator)
      {
         CONTENT_METADATA_BUFFER m(RESOURCE_TYPES::RESOURCE_TYPE_BRUSH,
                                   CONTENT_LOADER_IDS::CONTENT_LOADER_ID_BRUSH,
                                   L"Name");

         CONTENT_DICTIONARY_ENTRY_BUFFER e(8,
                                           &m,
                                           16);

         CONTENT_DICTIONARY_ENTRY_BUFFER eCopy = e;

         Assert::AreEqual(e.flags(), eCopy.flags(),
                          L"The flags are not correct.");

         Assert::AreEqual(e.offset(), eCopy.offset(),
                          L"The offset is not correct.");

         Assert::AreEqual(e.size(), eCopy.size(),
                          L"The data size is not correct.");

         Assert::IsTrue(*e.metadata() == *eCopy.metadata(),
                        L"The metadata is not correct.");
      }

      TEST_METHOD(EqualityOperator)
      {
         CONTENT_METADATA_BUFFER m(RESOURCE_TYPES::RESOURCE_TYPE_BRUSH,
                                   CONTENT_LOADER_IDS::CONTENT_LOADER_ID_BRUSH,
                                   L"Name");

         CONTENT_DICTIONARY_ENTRY_BUFFER e(8,
                                           &m,
                                           16);

         CONTENT_DICTIONARY_ENTRY_BUFFER eCopy = e;

         Assert::IsTrue(e == eCopy,
                        L"The dictionary entries should be equal.");

         CONTENT_DICTIONARY_ENTRY_BUFFER eDefault;

         Assert::IsFalse(e == eDefault,
                         L"The dictionary entries should not be equal.");
      }
   };
}