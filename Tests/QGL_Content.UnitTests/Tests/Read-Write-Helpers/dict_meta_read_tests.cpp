#include "pch.h"
#include "CppUnitTest.h"
#include <winrt/Windows.ApplicationModel.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace winrt::Windows;
using namespace winrt::Windows::Storage;
using namespace qgl::content;
using namespace qgl::content::content_file_helpers;

namespace QGL_Content_UnitTests
{
   TEST_CLASS(DictionaryMetaReadTests)
   {
      public:
      /*
       Assume write_file_sync, open_file_read, and open_file_write are correct.
       */
      TEST_METHOD(ReadDictionaryMetadata)
      {
         //Create a dictionary metadata buffer.
         CONTENT_FILE_HEADER_BUFFER hdr;
         CONTENT_DICTIONARY_METADATA_BUFFER toWrite(8, 32, 1);

         auto root = ApplicationData::Current().LocalFolder().Path();
         winrt::hstring newFilePath(root + L"\\ReadDictionaryMetadata.txt");

         auto hndl = open_file_write(newFilePath);
         write_file_sync(hndl, sizeof(toWrite),
                         hdr.dictionary_offset(), &toWrite);
         hndl.close();

         hndl = open_file_read(newFilePath);
         CONTENT_DICTIONARY_METADATA_BUFFER toRead =
            load_dictionary_metadata(hndl, hdr.dictionary_offset());

         Assert::IsTrue(toRead == toWrite,
                        L"The buffers are not the same.");
      }
   };
}