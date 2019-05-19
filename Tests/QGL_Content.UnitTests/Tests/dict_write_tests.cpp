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

      /*
       Assumes content_dictionary is correct.
       */
      TEST_METHOD(WriteDictionary)
      {
                
         //Create a dictionary.
         content_dictionary toWrite;
         CONTENT_METADATA_BUFFER meta(RESOURCE_TYPES::RESOURCE_TYPE_BRUSH,
                                      CONTENT_LOADER_ID_BRUSH,
                                      L"Brush");
         CONTENT_DICTIONARY_ENTRY_BUFFER entry1(sizeof(meta), meta, 256);
         toWrite.push_back(entry1);

         //Write it using the API to test.
         CONTENT_FILE_HEADER_BUFFER hdr;
         const auto dictMetaOffset = hdr.dictionary_offset();
         auto toWriteMeta = toWrite.buffer();
         auto entryOffset = dictMetaOffset +
            sizeof(CONTENT_DICTIONARY_METADATA_BUFFER);

         auto root = ApplicationData::Current().LocalFolder().Path();
         winrt::hstring newFilePath(root + L"\\WriteDictionary.txt");

         auto hndl = open_file_write(newFilePath);
         write_dictionary(hndl, dictMetaOffset, toWrite);
         hndl.close();

         //Read it back using tested APIs.
         hndl = open_file_read(newFilePath);
         
         CONTENT_DICTIONARY_METADATA_BUFFER toReadMeta;
         read_file_sync(hndl, sizeof(toReadMeta), dictMetaOffset, &toReadMeta);
         
         content_dictionary toRead(toReadMeta);
         CONTENT_DICTIONARY_ENTRY_BUFFER toReadEntry;
         read_file_sync(hndl, sizeof(toReadEntry), entryOffset, &toReadEntry);
         toRead.at(0) = toReadEntry;

         //Check that the dictionaries are equal.
         Assert::IsTrue(toWrite == toRead,
                        L"The dictionaries should be equal.");
      }
   };
}