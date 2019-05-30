#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace qgl::content;

namespace QGL_Content_UnitTests
{
   TEST_CLASS(ContentFileHeaderTests)
   {
      public:
      TEST_METHOD(DefaultConstructor)
      {
         CONTENT_FILE_HEADER_BUFFER hdr;
         const auto m = hdr.metadata();

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


         Assert::AreEqual(sizeof(CONTENT_FILE_HEADER_BUFFER),
                          hdr.dictionary_offset(),
                          L"The dictionary offset is not correct.");
      }

      TEST_METHOD(MetadataConstructor)
      {
         CONTENT_METADATA_BUFFER m(RESOURCE_TYPES::RESOURCE_TYPE_BRUSH,
                                   CONTENT_LOADER_IDS::CONTENT_LOADER_ID_BRUSH,
                                   L"Name");
         CONTENT_FILE_HEADER_BUFFER hdr(&m);

         Assert::IsTrue(m == *hdr.metadata(),
                        L"The metadata is not default.");

         Assert::AreEqual(sizeof(CONTENT_FILE_HEADER_BUFFER),
                          hdr.dictionary_offset(),
                          L"The dictionary offset is not correct.");
      }

      TEST_METHOD(CopyConstructor)
      {
         CONTENT_METADATA_BUFFER m(RESOURCE_TYPES::RESOURCE_TYPE_BRUSH,
                                   CONTENT_LOADER_IDS::CONTENT_LOADER_ID_BRUSH,
                                   L"Name");
         CONTENT_FILE_HEADER_BUFFER hdr(&m);
         CONTENT_FILE_HEADER_BUFFER hdrCopy(hdr);

         Assert::IsTrue(*hdr.metadata() == *hdrCopy.metadata(),
                        L"The metadatas are not equal.");

         Assert::AreEqual(hdr.dictionary_offset(), hdrCopy.dictionary_offset(),
                          L"The dictionary offsets are not equal.");
      }

      /*
       Assume copy constructor is correct.
       */
      TEST_METHOD(MoveConstructor)
      {
         CONTENT_METADATA_BUFFER m(RESOURCE_TYPES::RESOURCE_TYPE_BRUSH,
                                   CONTENT_LOADER_IDS::CONTENT_LOADER_ID_BRUSH,
                                   L"Name");
         CONTENT_FILE_HEADER_BUFFER hdr(&m);
         CONTENT_FILE_HEADER_BUFFER hdrCopy(hdr);
         CONTENT_FILE_HEADER_BUFFER hdrMove(std::move(hdrCopy));

         Assert::IsTrue(*hdr.metadata() == *hdrMove.metadata(),
                        L"The metadatas are not equal.");

         Assert::AreEqual(hdr.dictionary_offset(), hdrMove.dictionary_offset(),
                          L"The dictionary offsets are not equal.");
      }

      TEST_METHOD(DictionaryOffsetCorrect)
      {
         //Already tested by the constructor tests.
      }

      TEST_METHOD(MetadataCorrect)
      {
         //Already tested by the constructor tests.
      }

      /*
       Assume copy constructor is correct.
       */
      TEST_METHOD(EqualityOperator)
      {
         CONTENT_METADATA_BUFFER m(RESOURCE_TYPES::RESOURCE_TYPE_BRUSH,
                                   CONTENT_LOADER_IDS::CONTENT_LOADER_ID_BRUSH,
                                   L"Name");
         CONTENT_FILE_HEADER_BUFFER hdr(&m);
         CONTENT_FILE_HEADER_BUFFER hdrCopy(hdr);

         Assert::IsTrue(hdr == hdrCopy,
                        L"The headers should be equal.");

         CONTENT_METADATA_BUFFER m2(RESOURCE_TYPES::RESOURCE_TYPE_BRUSH,
                                    CONTENT_LOADER_IDS::CONTENT_LOADER_ID_BRUSH,
                                    L"Name");
         CONTENT_FILE_HEADER_BUFFER hdrNotEqual(&m2);

         Assert::IsFalse(hdr == hdrNotEqual,
                         L"The headers should not be equal.");
      }

      /*
       Assume the copy constructor and equality operators are correct.
       */
      TEST_METHOD(AssignOperator)
      {
         CONTENT_METADATA_BUFFER m(RESOURCE_TYPES::RESOURCE_TYPE_BRUSH,
                                   CONTENT_LOADER_IDS::CONTENT_LOADER_ID_BRUSH,
                                   L"Name");
         CONTENT_FILE_HEADER_BUFFER hdr(&m);
         CONTENT_FILE_HEADER_BUFFER hdrCopy = hdr;

         Assert::IsTrue(hdr == hdrCopy,
                        L"The headers should be equal.");
      }
   };
}