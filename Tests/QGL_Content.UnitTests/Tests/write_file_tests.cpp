#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace winrt::Windows::Storage;
using namespace qgl::content;

namespace QGL_Content_UnitTests
{
   /*
    These tests assume make_overlapped, make_default_security_attributes,
    make_default_open_file_params, open_file_read, and open_file_write
    are correct.
    */
   TEST_CLASS(WriteFileTests)
   {
      public:
      /*
       Create a file, write some data to it, close the file, re-open it and
       read the data.
       */
      TEST_METHOD(WriteEntireFileAndVerify)
      {
         auto root = ApplicationData::Current().LocalFolder().Path();
         winrt::hstring newFilePath(root + L"\\WriteEntireFileAndVerify.txt");

         auto handle = open_file_write(newFilePath);

         write_file_sync(handle, data_size, 0, data_to_rw);
         handle.close();

         handle = open_file_read(newFilePath);


         char* readBuffer[data_size];
         DWORD bytesRead = 0;
         auto result = ReadFile(handle.get(), readBuffer, data_size,
                                &bytesRead, nullptr);
         Assert::IsTrue(result != 0, L"Could not read the file.");

         Assert::IsTrue(memcmp(readBuffer, data_to_rw, data_size) == 0,
                        L"The data is not the same.");
      }
     
      private:
      static const size_t data_size = 10;
      const char* data_to_rw = "HelloWorld";
   };
}