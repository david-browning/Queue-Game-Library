#include "pch.h"
#include "CppUnitTest.h"
#include <winrt/Windows.ApplicationModel.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace winrt::Windows::Storage;
using namespace qgl::content;

namespace QGL_Content_UnitTests
{
   TEST_CLASS(ContentDataWriteTests)
   {
      public:
       TEST_METHOD(WriteContentData)
      {
         auto root = ApplicationData::Current().LocalFolder().Path();
         winrt::hstring newFilePath(root + L"\\WriteContentData.txt");

         //Create a header
         CONTENT_METADATA_BUFFER fileMetadata(RESOURCE_TYPE_BRUSH,
                                              CONTENT_LOADER_ID_BRUSH,
                                              L"Brush");
         CONTENT_FILE_HEADER_BUFFER hdr(fileMetadata);

         //Create a dictionary.
         content_dictionary dict;

         CONTENT_METADATA_BUFFER contentMetadata(RESOURCE_TYPE_STRING,
                                                 CONTENT_LOADER_ID_STRING,
                                                 L"String");
         static constexpr char* contentData = "Test";
         static constexpr auto contentDataSize = MemoryLength(contentData);
         static constexpr size_t contentOffset = 512;

         //Add something to the dictionary.
         CONTENT_DICTIONARY_ENTRY_BUFFER dictEntry(contentDataSize,
                                                   contentMetadata,
                                                   contentOffset);
         dict.push_back(dictEntry);

         auto hndl = open_file_write(newFilePath);

         //Write the header and dictionary.
         write_header(hndl, hdr);
         write_dictionary(hndl, hdr.dictionary_offset(), dict);

         //Write the content data using the API to test.
         write_content_data(hndl, dictEntry, contentData);

         //Close the file
         hndl.close();

         //Open the file
         hndl = open_file_read(newFilePath);

         //Read the header and dictionary.
         auto readHeader = load_header(hndl);
         auto readDict = load_dictionary(hndl, readHeader.dictionary_offset());
         auto readEntry = readDict[0];

         //Read the content data using already tested APIs.
         std::string readContent;
         readContent.resize(readEntry.size() + 1);
         read_file_sync(hndl, contentDataSize, 
                        contentOffset, 
                        readContent.data());

         Assert::IsTrue(memcmp(readContent.data(), 
                               contentData, 
                               contentDataSize) == 0,
                          L"The content data is not correct.");
      }
   };
}