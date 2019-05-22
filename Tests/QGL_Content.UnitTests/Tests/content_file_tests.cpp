#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace winrt::Windows::Storage;
using namespace qgl::content;
using namespace qgl::content::content_file_helpers;

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

         auto cf = new content_file(newFilePath);
         delete cf;
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

         content_file cf(newFilePath);

         CONTENT_METADATA_BUFFER meta(RESOURCE_TYPE_BRUSH,
                                      CONTENT_LOADER_ID_BRUSH,
                                      L"Brush");
         CONTENT_FILE_HEADER_BUFFER newHeader(meta);

         cf.header() = newHeader;

         Assert::IsTrue(cf.header() == newHeader,
                        L"The headers should be equal.");

      }

      TEST_METHOD(GetSize)
      {
         auto root = ApplicationData::Current().LocalFolder();
         winrt::hstring newFilePath = root.Path() +
            L"\\ContentFileGetSize.txt";
         DeleteFile(newFilePath.c_str());

         content_file cf(newFilePath);

         Assert::AreEqual(static_cast<size_t>(0), cf.size(),
                          L"The size should be 0.");

         CONTENT_METADATA_BUFFER meta(RESOURCE_TYPE_BRUSH,
                                      CONTENT_LOADER_ID_BRUSH,
                                      L"Brush");
         cf.push_back(meta, L"Shared Path");
         Assert::AreEqual(static_cast<size_t>(1), cf.size(),
                          L"The size should be 1.");

         content_data_buffer_t dataBuffer;
         cf.push_back(meta, dataBuffer);
         Assert::AreEqual(static_cast<size_t>(2), cf.size(),
                          L"The size should be 2.");
      }

      TEST_METHOD(SinglePushBackAndIterator)
      {
         auto root = ApplicationData::Current().LocalFolder();
         winrt::hstring newFilePath = root.Path() +
            L"\\ContentFileSinglePushBack.txt";
         DeleteFile(newFilePath.c_str());

         content_file cf(newFilePath);

         Assert::IsTrue(cf.begin() == cf.end(),
                        L"The beginning and end iterators should be equal.");

         content_data_buffer_t dataBuffer1;
         CONTENT_METADATA_BUFFER meta(RESOURCE_TYPE_BRUSH,
                                      CONTENT_LOADER_ID_BRUSH,
                                      L"Brush");

         cf.push_back(meta, dataBuffer1);

         Assert::IsTrue(cf.begin()->metadata() == meta,
                        L"The metadata should be equal.");
      }

      TEST_METHOD(MultiPushBackAndIterator)
      {
         auto root = ApplicationData::Current().LocalFolder();
         winrt::hstring newFilePath = root.Path() +
            L"\\ContentFileMultiPushBack.txt";
         DeleteFile(newFilePath.c_str());

         content_file cf(newFilePath);
         content_data_buffer_t dataBuffer1;
         content_data_buffer_t dataBuffer2;
         shared_content_data_buffer_t shared1(L"Shared Path 1");
         shared_content_data_buffer_t shared2(L"Shared Path 2");

         CONTENT_METADATA_BUFFER meta1(RESOURCE_TYPE_BRUSH,
                                       CONTENT_LOADER_ID_BRUSH,
                                       L"Brush");

         CONTENT_METADATA_BUFFER meta2(RESOURCE_TYPE_CAMERA,
                                       CONTENT_LOADER_ID_CAMERA,
                                       L"Camera");

         cf.push_back(meta1, dataBuffer1);
         cf.push_back(meta2, dataBuffer2);
         cf.push_back(meta1, shared1);
         cf.push_back(meta2, shared2);

         int i = 0;
         for (auto& entry : cf)
         {
            if (i & 1)
            {
               Assert::IsTrue(meta2 == entry.metadata(),
                              L"Entry should be meta2.");
            }
            else
            {
               Assert::IsTrue(meta1 == entry.metadata(),
                              L"Entry should be meta1.");
            }
            if (i > 1)
            {
               Assert::IsTrue(entry.shared(),
                              L"Entry should be shared.");
            }

            i++;
         }
      }

      TEST_METHOD(Flush)
      {
         auto root = ApplicationData::Current().LocalFolder();
         winrt::hstring newFilePath = root.Path() +
            L"\\ContentFileFlush.txt";
         DeleteFile(newFilePath.c_str());

         content_file cf(newFilePath);
         content_data_buffer_t dataBuffer1;
         content_data_buffer_t dataBuffer2;
         shared_content_data_buffer_t shared1(L"Shared Path 1");
         shared_content_data_buffer_t shared2(L"Shared Path 2");

         CONTENT_METADATA_BUFFER meta1(RESOURCE_TYPE_BRUSH,
                                       CONTENT_LOADER_ID_BRUSH,
                                       L"Brush");

         CONTENT_METADATA_BUFFER meta2(RESOURCE_TYPE_CAMERA,
                                       CONTENT_LOADER_ID_CAMERA,
                                       L"Camera");

         cf.push_back(meta1, dataBuffer1);
         cf.push_back(meta2, dataBuffer2);
         cf.push_back(meta1, shared1);
         cf.push_back(meta2, shared2);

         //This should not throw an exception.
         try
         {
            cf.flush();
         }
         catch (...)
         {
            Assert::Fail(L"An exception happened.");
         }
      }

      TEST_METHOD(FlushAndVerify)
      {
         auto root = ApplicationData::Current().LocalFolder();
         winrt::hstring newFilePath = root.Path() +
            L"\\ContentFileFlushAndVerify.hex";
         DeleteFile(newFilePath.c_str());

         CONTENT_METADATA_BUFFER headerMeta(RESOURCE_TYPE_BRUSH,
                                            CONTENT_LOADER_ID_BRUSH,
                                            L"Brush");

         content_file cf(newFilePath);
         cf.header() = CONTENT_FILE_HEADER_BUFFER(headerMeta);

         CONTENT_METADATA_BUFFER meta1(RESOURCE_TYPE_FLOAT,
                                       CONTENT_LOADER_ID_STRING,
                                       L"Red Value");
         CONTENT_METADATA_BUFFER meta2(RESOURCE_TYPE_FLOAT,
                                       CONTENT_LOADER_ID_STRING,
                                       L"Green Value");
         shared_content_data_buffer_t shared1(L"Shared Path 1");
         shared_content_data_buffer_t shared2(L"Shared Path 2");

         cf.push_back(meta1, shared1);
         cf.push_back(meta2, shared2);

         cf.flush();

         auto hndl = open_file_read(newFilePath);
         auto readHdr = content_file_helpers::load_header(hndl);
         Assert::IsTrue(readHdr == cf.header(),
                        L"The header is not correct.");

         auto readDictMeta =
            load_dictionary_metadata(hndl,
                                     readHdr.dictionary_offset());
         Assert::AreEqual(cf.size(), readDictMeta.count(),
                          L"The dictionary does not have the correct number"
                          " of entries.");

         auto readDictEntry =
            load_dictionary_entry(hndl,
                                  readHdr.dictionary_offset() +
                                  sizeof(readDictMeta));
         Assert::IsTrue(cf.front() == readDictEntry,
                        L"The dictionary entries are not equal.");
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

         content_file cf(newFilePath);
         cf.header() = CONTENT_FILE_HEADER_BUFFER(headerMeta);

         CONTENT_METADATA_BUFFER meta1(RESOURCE_TYPE_FLOAT,
                                       CONTENT_LOADER_ID_STRING,
                                       L"Red Value");
         CONTENT_METADATA_BUFFER meta2(RESOURCE_TYPE_FLOAT,
                                       CONTENT_LOADER_ID_STRING,
                                       L"Green Value");
         shared_content_data_buffer_t shared1(L"Shared Path 1");
         shared_content_data_buffer_t shared2(L"Shared Path 2");

         cf.push_back(meta1, shared1);
         cf.push_back(meta2, shared2);

         cf.flush();

         content_file cfOpen(newFilePath);

         Assert::IsTrue(cf.header() == cfOpen.header(),
                        L"The headers are not equal.");

         Assert::AreEqual(cf.size(), cfOpen.size(),
                          L"The content files do not have the same number of"
                          " entries.");

         auto writeDictIt = cf.begin();
         auto readDictIt = cfOpen.begin();

         while (writeDictIt != cf.end() && readDictIt != cfOpen.end())
         {
            Assert::IsTrue(*writeDictIt == *readDictIt,
                           L"The dictionary entries are not equal.");
            writeDictIt++;
            readDictIt++;
         }
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

         content_file cf(newFilePath);
         cf.header() = CONTENT_FILE_HEADER_BUFFER(headerMeta);

         CONTENT_METADATA_BUFFER meta1(RESOURCE_TYPE_FLOAT,
                                       CONTENT_LOADER_ID_STRING,
                                       L"Red Value");
         CONTENT_METADATA_BUFFER meta2(RESOURCE_TYPE_FLOAT,
                                       CONTENT_LOADER_ID_STRING,
                                       L"Green Value");
         shared_content_data_buffer_t shared1(L"Shared Path 1");
         shared_content_data_buffer_t shared2(L"Shared Path 2");

         cf.push_back(meta1, shared1);
         cf.push_back(meta2, shared2);

         cf.flush();


         CONTENT_METADATA_BUFFER meta3(RESOURCE_TYPE_FLOAT,
                                       CONTENT_LOADER_ID_STRING,
                                       L"Blue Value");
         content_data_buffer_t blueValue = { 0x00, 0x10, 0x20 };

         {
            content_file cfOpen(newFilePath);
            cfOpen.push_back(meta3, blueValue);
            cfOpen.flush();
         }
         
         {
            content_file cfOpen(newFilePath);
            Assert::IsTrue(cfOpen.back().metadata() == meta3,
                           L"The last dictionary entry is not correct.");
         }
      }

      private:
      winrt::fire_and_forget test_storage_file_constructor()
      {
         auto root = ApplicationData::Current().LocalFolder();
         auto f = co_await root.CreateFileAsync(
            L"CreateNewStorageContentFile.txt",
            CreationCollisionOption::ReplaceExisting);

         content_file cf(f);

         Assert::IsTrue(true, L"The test should pass.");
      }
   };
}