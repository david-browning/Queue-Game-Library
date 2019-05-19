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

      /*
       Assumes content_dictionary is correct.
       */
      TEST_METHOD(ReadDictionary)
      {
         //Create a dictionary.
         content_dictionary toWrite;
         CONTENT_METADATA_BUFFER meta(RESOURCE_TYPES::RESOURCE_TYPE_BRUSH,
                                      CONTENT_LOADER_ID_BRUSH,
                                      L"Brush");
         CONTENT_DICTIONARY_ENTRY_BUFFER entry1(sizeof(meta), meta, 256);
         toWrite.push_back(entry1);

         //Write it at the correct offsets using tested APIs.
         CONTENT_FILE_HEADER_BUFFER hdr;
         const auto dictMetaOffset = hdr.dictionary_offset();
         auto toWriteMeta = toWrite.buffer();
         auto entryOffset = dictMetaOffset +
            sizeof(CONTENT_DICTIONARY_METADATA_BUFFER);

         auto root = ApplicationData::Current().LocalFolder().Path();
         winrt::hstring newFilePath(root + L"\\ReadDictionary.txt");

         auto hndl = open_file_write(newFilePath);
         write_file_sync(hndl, sizeof(toWriteMeta), 
                         dictMetaOffset, &toWriteMeta);
         write_dictionary_entry(hndl, toWrite.at(0), entryOffset);
         hndl.close();



         //Read it back using tested APIs.
         hndl = open_file_read(newFilePath);
         auto  toRead = load_dictionary(hndl, dictMetaOffset);

         //Check the dictionaries are equal.
         Assert::IsTrue(toWrite.buffer() == toRead.buffer(),
                        L"Buffers should be equal.");

         Assert::IsTrue(toWrite == toRead,
                        L"The dictionaries should be equal.");
      }
   };
}