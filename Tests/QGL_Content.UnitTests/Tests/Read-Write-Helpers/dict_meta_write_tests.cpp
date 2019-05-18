#include "pch.h"
#include "CppUnitTest.h"
#include <winrt/Windows.ApplicationModel.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace winrt::Windows;
using namespace winrt::Windows::Storage;
using namespace qgl::content;

namespace QGL_Content_UnitTests
{
   /*
    These tests read and write the dictionary metadata buffer to files.
    Reading tests involve creating a buffer, writing it using tested APIs, and
    reading the buffer using the API to test.
    Writing tests involve creating a buffer, writing it using the API to test,
    and reading it using tested APIs.
    */
   TEST_CLASS(DictionaryMetaWriteTests)
   {
      public:
      public:
      /*
       Assume read_file_sync, open_file_read, and open_file_write are correct.
       */
      TEST_METHOD(WriteDictionaryMetadata)
      {
         //Create a dictionary metadata buffer.
         CONTENT_FILE_HEADER_BUFFER hdr;
         CONTENT_DICTIONARY_METADATA_BUFFER toWrite(8, 32, 1);

         auto root = ApplicationData::Current().LocalFolder().Path();
         winrt::hstring newFilePath(root + L"\\WriteDictionaryMetadata.txt");

         auto hndl = open_file_write(newFilePath);
         write_dictionary_metadata(hndl, toWrite, hdr.dictionary_offset());
         hndl.close();

         CONTENT_DICTIONARY_METADATA_BUFFER toRead;
         hndl = open_file_read(newFilePath);
         read_file_sync(hndl, sizeof(toRead), hdr.dictionary_offset(), &toRead);

         Assert::IsTrue(toRead == toWrite,
                        L"The buffers are not the same.");
      }
   };
}