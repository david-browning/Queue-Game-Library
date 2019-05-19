#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace winrt::Windows::Storage;
using namespace qgl::content;

namespace QGL_Content_UnitTests
{
   TEST_CLASS(WriteFileOffsetTests)
   {
      public:
       /*
       Create a file, write some data to it, close the file, re-open it and
       read the first half of the data.
       */
    /*  TEST_METHOD(WriteFirstPartAndVerify)
      {
         auto root = ApplicationData::Current().LocalFolder().Path();
         winrt::hstring newFilePath(root + L"\\WriteFirstPartAndVerify.txt");

         auto handle = open_file_write(newFilePath);
         write_file_sync(handle, data_size / 2, 0, data_to_rw);
         handle.close();

         handle = open_file_read(newFilePath);
         char* readBuffer[data_size / 2];
         DWORD bytesRead = 0;
         auto result = ReadFile(handle.get(), readBuffer, data_size / 2,
                                &bytesRead, nullptr);

         Assert::IsTrue(result != 0, L"Could not read the file.");

         Assert::IsTrue(memcmp(readBuffer, data_to_rw, data_size / 2) == 0,
                        L"The data is not the same.");
      }*/

      /*
       Create a file, write some data to it, close the file, re-open it and
       read the second half of the data.
       */
      TEST_METHOD(WriteSecondPartAndVerify)
      {
         auto root = ApplicationData::Current().LocalFolder().Path();
         winrt::hstring newFilePath(root + L"\\WriteSecondPartAndVerify.txt");

         auto handle = open_file_write(newFilePath);
         write_file_sync(handle, data_size / 2, data_size / 2,
                         data_to_rw + (data_size / 2));

         handle.close();

         handle = open_file_read(newFilePath);
         char readBuffer[data_size / 2] = { 0 };
         DWORD bytesRead = 0;
         OVERLAPPED offsetOverlapped;
         make_overlapped(data_size / 2, &offsetOverlapped);
         auto result = ReadFile(handle.get(), readBuffer, data_size / 2,
                                &bytesRead, &offsetOverlapped);

         Assert::IsTrue(result != 0, L"Could not read the file.");

         Assert::IsTrue(memcmp(readBuffer, data_to_rw + (data_size / 2),
                               data_size / 2) == 0,
                        L"The data is not correct.");
      }

      private:
      static const size_t data_size = 10;
      const char* data_to_rw = "HelloWorld";
   };
}