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
   TEST_CLASS(DictionaryReadTests)
   {
      public:
      TEST_METHOD(ReadDictEntry)
      {
         CONTENT_METADATA_BUFFER meta;
         CONTENT_DICTIONARY_ENTRY_BUFFER toWrite(sizeof(meta), meta, 64);

         auto root = ApplicationData::Current().LocalFolder().Path();
         winrt::hstring newFilePath(root + L"\\ReadDictEntry.txt");

         auto hndl = open_file_write(newFilePath);
         write_file_sync(hndl, sizeof(toWrite), 32, &toWrite);
         hndl.close();

         hndl = open_file_read(newFilePath);
         auto toRead = load_dictionary_entry(hndl, 32);

         Assert::IsTrue(toRead == toWrite,
                        L"The entries are not the same.");
      }
   };
}