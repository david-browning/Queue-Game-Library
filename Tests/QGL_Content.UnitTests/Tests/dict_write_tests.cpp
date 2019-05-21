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
   /*
    These tests involve creating a file, writing dictionary data to it using 
    a tested API and reading the data back using an API to test.
    Assume that File-Helper tests are correct.
    Assume that Dictionary tests are correct.
    */
   TEST_CLASS(DictionaryWriteTests)
   {
      public:
      TEST_METHOD(WriteDictEntry)
      {
         CONTENT_METADATA_BUFFER meta;
         CONTENT_DICTIONARY_ENTRY_BUFFER toWrite(sizeof(meta), meta, 64);

         auto root = ApplicationData::Current().LocalFolder().Path();
         winrt::hstring newFilePath(root + L"\\WriteDictEntry.txt");

         auto hndl = open_file_write(newFilePath);
         write_dictionary_entry(hndl, toWrite, 32);
         hndl.close();

         hndl = open_file_read(newFilePath);
         CONTENT_DICTIONARY_ENTRY_BUFFER toRead;
         read_file_sync(hndl, sizeof(toRead), 32, &toRead);

         Assert::IsTrue(toRead == toWrite,
                        L"The entries are not the same.");
      }
   };
}