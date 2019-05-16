#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace winrt::Windows::Storage;
using namespace qgl::content;

namespace QGL_Content_UnitTests
{
   /*
    These tests assume make_overlapped, make_default_security_attributes,
    make_default_open_file_params, write_file_sync, open_file_read, 
    and open_file_write are correct.
    */
   TEST_CLASS(ReadFileTests)
   {
      public:
      /*
       Create a file, write some data to it, close the file, and read it back.
       */
      TEST_METHOD(ReadEntireFile)
      {
         auto root = ApplicationData::Current().LocalFolder().Path();
         winrt::hstring newFilePath(root + L"\\ReadEntireFile.txt");

         auto handle = open_file_write(newFilePath);
         write_file_sync(handle, data_size / 2, 0, data_to_rw);
         handle.close();

         handle = open_file_read(newFilePath);
         auto fileData = file_data(handle);

         Assert::IsTrue(memcmp(fileData.data(), 
                               data_to_rw, 
                               fileData.size()) == 0,
                        L"The file data is not correct.");
      }
       
      /*
       Create a file, write some data to it, close the file, and read the
       first half back.
       */
      TEST_METHOD(ReadFileFirstPart)
      {
         auto root = ApplicationData::Current().LocalFolder().Path();
         winrt::hstring newFilePath(root + L"\\ReadFileFirstPart.txt");

         auto handle = open_file_write(newFilePath);
         write_file_sync(handle, data_size / 2, 0, data_to_rw);
         handle.close();

         handle = open_file_read(newFilePath);
         char fileData[data_size / 2];
         read_file_sync(handle, data_size / 2, 0, fileData);

         Assert::IsTrue(memcmp(fileData, data_to_rw, data_size / 2) == 0,
                        L"The file data is not correct.");
      }

      /*
       Create a file, write some data to it, close the file, and read the
       second half back.
       */
      TEST_METHOD(ReadFileSecondPart)
      {
         auto root = ApplicationData::Current().LocalFolder().Path();
         winrt::hstring newFilePath(root + L"\\ReadFileSecondPart.txt");

         auto handle = open_file_write(newFilePath);
         write_file_sync(handle, 
                         data_size / 2,
                         data_size / 2, 
                         data_to_rw + (data_size / 2));
         handle.close();

         handle = open_file_read(newFilePath);
         char fileData[data_size / 2];
         read_file_sync(handle, data_size / 2, data_size / 2, fileData);

         Assert::IsTrue(memcmp(fileData, data_to_rw + (data_size / 2),
                               data_size / 2) == 0,
                        L"The file data is incorrect.");
      }

      private:
      static const size_t data_size = 10;
      const char* data_to_rw = "HelloWorld";
   };
}