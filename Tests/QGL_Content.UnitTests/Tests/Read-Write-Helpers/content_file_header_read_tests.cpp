#include "pch.h"
#include "CppUnitTest.h"
#include <winrt/Windows.ApplicationModel.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace winrt::Windows;
using namespace winrt::Windows::Storage;
using namespace qgl::content;

namespace QGL_Content_UnitTests
{
   TEST_CLASS(ContentFileHeaderReadTests)
   {
      public:
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