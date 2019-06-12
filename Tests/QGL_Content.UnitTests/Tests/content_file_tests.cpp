#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace winrt::Windows::Storage;
using namespace qgl::content;

namespace QGL_Content_UnitTests
{
   TEST_CLASS(ContentFileTests)
   {
      public:
      TEST_METHOD(CreateNewContentFile)
      {
         auto root = ApplicationData::Current().LocalFolder();
         winrt::hstring newFilePath = root.Path() +
            L"\\ContentFileCreateNew.txt";
         DeleteFile(newFilePath.c_str());

         icontent_file* cf = nullptr;
         winrt::check_hresult(qgl_open_content_file(newFilePath.c_str(),
                                                    qgl::QGL_VERSION_0_1_WIN,
                                                    &cf));
         cf->release();
         Assert::IsTrue(true, L"The test should pass.");
      }

      TEST_METHOD(CreateNewStorageContentFile)
      {
         test_storage_file_constructor();
      }

      TEST_METHOD(GetSetHeader)
      {
         auto root = ApplicationData::Current().LocalFolder();
         winrt::hstring newFilePath = root.Path() +
            L"\\ContentFileGetSetHeader.txt";
         DeleteFile(newFilePath.c_str());

         icontent_file* cf = nullptr;
         winrt::check_hresult(qgl_open_content_file(newFilePath.c_str(),
                                                    qgl::QGL_VERSION_0_1_WIN,
                                                    &cf));

         CONTENT_METADATA_BUFFER meta(RESOURCE_TYPE_BRUSH,
                                      CONTENT_LOADER_ID_BRUSH,
                                      L"Brush");
         CONTENT_FILE_HEADER_BUFFER newHeader(&meta);

         *cf->header() = newHeader;
         Assert::IsTrue(*cf->header() == newHeader,
                        L"The headers should be equal.");
         cf->release();
      }

      TEST_METHOD(GetSize)
      {
         auto root = ApplicationData::Current().LocalFolder();
         winrt::hstring newFilePath = root.Path() +
            L"\\ContentFileGetSize.txt";
         DeleteFile(newFilePath.c_str());

         icontent_file* cf = nullptr;
         winrt::check_hresult(qgl_open_content_file(newFilePath.c_str(),
                                                    qgl::QGL_VERSION_0_1_WIN,
                                                    &cf));

         Assert::AreEqual(static_cast<size_t>(0), cf->size(),
                          L"The size should be 0.");

         CONTENT_METADATA_BUFFER meta(RESOURCE_TYPE_BRUSH,
                                      CONTENT_LOADER_ID_BRUSH,
                                      L"Brush");
         SHARED_CONTENT_ENTRY sharedPathEntry(L"Q:Shared Path");
         cf->push_back(&meta, &sharedPathEntry);
         Assert::AreEqual(static_cast<size_t>(1), cf->size(),
                          L"The size should be 1.");

         uint8_t buffData[8] = { 0 };
         DATA_CONTENT_ENTRY dataBuffer(buffData, 8);
         cf->push_back(&meta, &dataBuffer);
         Assert::AreEqual(static_cast<size_t>(2), cf->size(),
                          L"The size should be 2.");
         cf->release();
      }

      TEST_METHOD(SinglePushBackAndIterator)
      {
         auto root = ApplicationData::Current().LocalFolder();
         winrt::hstring newFilePath = root.Path() +
            L"\\ContentFileSinglePushBack.txt";
         DeleteFile(newFilePath.c_str());

         icontent_file* cf = nullptr;
         winrt::check_hresult(qgl_open_content_file(newFilePath.c_str(),
                                                    qgl::QGL_VERSION_0_1_WIN,
                                                    &cf));

         Assert::AreEqual(static_cast<size_t>(0), cf->size(),
                          L"The beginning and end iterators should be equal.");

         uint8_t buffData[8] = { 0 };
         DATA_CONTENT_ENTRY dataBuffer1(buffData, 8);
         CONTENT_METADATA_BUFFER meta(RESOURCE_TYPE_BRUSH,
                                      CONTENT_LOADER_ID_BRUSH,
                                      L"Brush");

         cf->push_back(&meta, &dataBuffer1);

         Assert::IsTrue(*(cf->at(0)->metadata()) == meta,
                        L"The metadata should be equal.");
         cf->release();
      }

      TEST_METHOD(MultiPushBackAndIterator)
      {
         auto root = ApplicationData::Current().LocalFolder();
         winrt::hstring newFilePath = root.Path() +
            L"\\ContentFileMultiPushBack.txt";
         DeleteFile(newFilePath.c_str());

         icontent_file* cf = nullptr;
         winrt::check_hresult(qgl_open_content_file(newFilePath.c_str(),
                                                    qgl::QGL_VERSION_0_1_WIN,
                                                    &cf));
         uint8_t rawBuffData[8] = { 0 };
         DATA_CONTENT_ENTRY dataBuffer1(rawBuffData, 8);
         DATA_CONTENT_ENTRY dataBuffer2(rawBuffData, 8);

         CONTENT_METADATA_BUFFER meta1(RESOURCE_TYPE_BRUSH,
                                       CONTENT_LOADER_ID_BRUSH,
                                       L"Brush");

         CONTENT_METADATA_BUFFER meta2(RESOURCE_TYPE_CAMERA,
                                       CONTENT_LOADER_ID_CAMERA,
                                       L"Camera");

         cf->push_back(&meta1, &dataBuffer1);
         cf->push_back(&meta2, &dataBuffer2);
         SHARED_CONTENT_ENTRY sharedPath1(L"Q:Shared Path 1");
         SHARED_CONTENT_ENTRY sharedPath2(L"V:Shared Path 2");
         cf->push_back(&meta1, &sharedPath1);
         cf->push_back(&meta2, &sharedPath2);

         for (size_t i = 0; i < cf->size(); i++)
         {
            auto entry = cf->at(i);
            if (i & 1)
            {
               Assert::IsTrue(meta2 == *entry->metadata(),
                              L"Entry should be meta2.");
            }
            else
            {
               Assert::IsTrue(meta1 == *entry->metadata(),
                              L"Entry should be meta1.");
            }
            if (i > 1)
            {
               Assert::IsTrue(entry->shared(),
                              L"Entry should be shared.");
            }

            i++;
         }
         cf->release();
      }

      TEST_METHOD(Flush)
      {
         auto root = ApplicationData::Current().LocalFolder();
         winrt::hstring newFilePath = root.Path() +
            L"\\ContentFileFlush.txt";
         DeleteFile(newFilePath.c_str());

         icontent_file* cf = nullptr;
         winrt::check_hresult(qgl_open_content_file(newFilePath.c_str(),
                                                    qgl::QGL_VERSION_0_1_WIN,
                                                    &cf));
         std::vector<uint8_t> rawBuffData = { 0 };
         DATA_CONTENT_ENTRY buffData(rawBuffData);

         CONTENT_METADATA_BUFFER meta1(RESOURCE_TYPE_BRUSH,
                                       CONTENT_LOADER_ID_BRUSH,
                                       L"Brush");

         CONTENT_METADATA_BUFFER meta2(RESOURCE_TYPE_CAMERA,
                                       CONTENT_LOADER_ID_CAMERA,
                                       L"Camera");

         cf->push_back(&meta1, &buffData);
         cf->push_back(&meta2, &buffData);
         SHARED_CONTENT_ENTRY sharedPath1(L"Q:Shared Path 1");
         SHARED_CONTENT_ENTRY sharedPath2(L"V:Shared Path 2");
         cf->push_back(&meta1, &sharedPath1);
         cf->push_back(&meta2, &sharedPath2);

         Assert::IsTrue(SUCCEEDED(cf->flush()), L"The HRESULT failed.");

         cf->release();
      }

      TEST_METHOD(OpenExistingFile)
      {
         auto root = ApplicationData::Current().LocalFolder();
         winrt::hstring newFilePath = root.Path() +
            L"\\ContentFileOpenExisting.txt";
         DeleteFile(newFilePath.c_str());

         CONTENT_METADATA_BUFFER headerMeta(RESOURCE_TYPE_BRUSH,
                                            CONTENT_LOADER_ID_BRUSH,
                                            L"Brush");

         icontent_file* cf = nullptr;
         winrt::check_hresult(qgl_open_content_file(newFilePath.c_str(),
                                                    qgl::QGL_VERSION_0_1_WIN,
                                                    &cf));
         *cf->header() = CONTENT_FILE_HEADER_BUFFER(&headerMeta);

         CONTENT_METADATA_BUFFER meta1(RESOURCE_TYPE_FLOAT,
                                       CONTENT_LOADER_ID_STRING,
                                       L"Red Value");
         CONTENT_METADATA_BUFFER meta2(RESOURCE_TYPE_FLOAT,
                                       CONTENT_LOADER_ID_STRING,
                                       L"Green Value");
         SHARED_CONTENT_ENTRY sharedPath1(L"Q:Shared Path 1");
         SHARED_CONTENT_ENTRY sharedPath2(L"V:Shared Path 2");
         cf->push_back(&meta1, &sharedPath1);
         cf->push_back(&meta2, &sharedPath2);

         cf->flush();

         icontent_file* cfOpen = nullptr;
         winrt::check_hresult(qgl_open_content_file(newFilePath.c_str(),
                                                    qgl::QGL_VERSION_0_1_WIN,
                                                    &cfOpen));

         Assert::IsTrue(*cf->header() == *cfOpen->header(),
                        L"The headers are not equal.");

         Assert::AreEqual(cf->size(), cfOpen->size(),
                          L"The content files do not have the same number of"
                          " entries.");
         cf->release();
         cfOpen->release();
      }

      TEST_METHOD(ModifyExistingFile)
      {
         auto root = ApplicationData::Current().LocalFolder();
         winrt::hstring newFilePath = root.Path() +
            L"\\ContentFileModifyExisting.txt";
         DeleteFile(newFilePath.c_str());

         CONTENT_METADATA_BUFFER headerMeta(RESOURCE_TYPE_BRUSH,
                                            CONTENT_LOADER_ID_BRUSH,
                                            L"Brush");

         icontent_file* cf = nullptr;
         winrt::check_hresult(qgl_open_content_file(newFilePath.c_str(),
                                                    qgl::QGL_VERSION_0_1_WIN,
                                                    &cf));
         *cf->header() = CONTENT_FILE_HEADER_BUFFER(&headerMeta);

         CONTENT_METADATA_BUFFER meta1(RESOURCE_TYPE_FLOAT,
                                       CONTENT_LOADER_ID_STRING,
                                       L"Red Value");
         CONTENT_METADATA_BUFFER meta2(RESOURCE_TYPE_FLOAT,
                                       CONTENT_LOADER_ID_STRING,
                                       L"Green Value");
         SHARED_CONTENT_ENTRY sharedPath1(L"Q:Shared Path 1");
         SHARED_CONTENT_ENTRY sharedPath2(L"V:Shared Path 2");

         cf->push_back(&meta1, &sharedPath1);
         cf->push_back(&meta2, &sharedPath2);
         cf->flush();
         cf->release();

         CONTENT_METADATA_BUFFER meta3(RESOURCE_TYPE_FLOAT,
                                       CONTENT_LOADER_ID_STRING,
                                       L"Blue Value");
         std::vector<uint8_t> rawbluedata = { 0x00, 0x10, 0x20 };
         DATA_CONTENT_ENTRY bluedata(rawbluedata);

         {
            icontent_file* cfOpen = nullptr;
            winrt::check_hresult(qgl_open_content_file(newFilePath.c_str(),
                                                       qgl::QGL_VERSION_0_1_WIN,
                                                       &cfOpen));
            cfOpen->push_back(&meta3, &bluedata);
            cfOpen->flush();
            cfOpen->release();
         }

         {
            icontent_file* cfOpen = nullptr;
            winrt::check_hresult(qgl_open_content_file(newFilePath.c_str(),
                                                       qgl::QGL_VERSION_0_1_WIN,
                                                       &cfOpen));
            Assert::IsTrue(*(cfOpen->at(2)->metadata()) == meta3,
                           L"The last dictionary entry is not correct.");
            cfOpen->release();
         }
      }

      private:
      winrt::fire_and_forget test_storage_file_constructor()
      {
         auto root = ApplicationData::Current().LocalFolder();
         auto f = co_await root.CreateFileAsync(
            L"CreateNewStorageContentFile.txt",
            CreationCollisionOption::ReplaceExisting);

         icontent_file* cf = nullptr;
         winrt::check_hresult(qgl_open_content_file_sf(f,
                                                       qgl::QGL_VERSION_0_1_WIN,
                                                       &cf));
         cf->release();
         Assert::IsTrue(true, L"The test should pass.");
      }
   };
}