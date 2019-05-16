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
    Readings tests involve creating a buffer, writing it using tested APIs and
    reading using the API to test.
    Writing tests involve creating a buffer, writing using the API to test, and
    reading using tested APIs.
    These tests assume the File-Helper tests are correct.
    */
   TEST_CLASS(ContentFileHeaderRWTests)
   {
      public:
      /*
       Assume read_file_sync, open_file_read, and open_file_write are correct.
       */
      TEST_METHOD(WriteHeader)
      {
         CONTENT_METADATA_BUFFER hdrMeta(
            RESOURCE_TYPES::RESOURCE_TYPE_BRUSH,
            CONTENT_LOADER_IDS::CONTENT_LOADER_ID_BRUSH,
            L"Test");

         CONTENT_FILE_HEADER_BUFFER hdrToWrite(hdrMeta);

         auto root = ApplicationData::Current().LocalFolder().Path();
         winrt::hstring newFilePath(root + L"\\WriteHeader.txt");

         auto hndl = open_file_write(newFilePath);
         write_header(hndl, hdrToWrite);
         hndl.close();

         CONTENT_FILE_HEADER_BUFFER hdrToRead;
         hndl = open_file_read(newFilePath);
         read_file_sync(hndl, sizeof(hdrToRead), 0, &hdrToRead);

         Assert::IsTrue(hdrToWrite == hdrToRead,
                        L"The headers are not the same.");
      }

      /*
       Assume write_file_sync, open_file_read, and open_file_write are correct.
       */
      TEST_METHOD(ReadHeader)
      {
         CONTENT_METADATA_BUFFER hdrMeta(
            RESOURCE_TYPES::RESOURCE_TYPE_BRUSH,
            CONTENT_LOADER_IDS::CONTENT_LOADER_ID_BRUSH,
            L"Test");

         CONTENT_FILE_HEADER_BUFFER hdr(hdrMeta);

         auto root = ApplicationData::Current().LocalFolder().Path();
         winrt::hstring newFilePath(root + L"\\ReadHeader.txt");
         auto hndl = open_file_write(newFilePath);
         write_file_sync(hndl, sizeof(hdr), 0, &hdr);

         hndl.close();

         hndl = open_file_read(newFilePath);
         CONTENT_FILE_HEADER_BUFFER readHdr;
         read_file_sync(hndl, sizeof(hdr), 0, &readHdr);

         Assert::IsTrue(hdr == readHdr,
                        L"The headers are not the same.");
      }
   };
}