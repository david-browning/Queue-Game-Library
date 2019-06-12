#include "pch.h"
#include "CppUnitTest.h"
#include <winrt/Windows.Storage.h>
#include <winrt/Windows.ApplicationModel.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace winrt::Windows;
using namespace winrt::Windows::Storage;
using namespace qgl::content;

namespace QGL_Content_UnitTests
{
   /*
    Open a file using QGL functions and attempt to read or write using the
    Windows API. If opening a file throws an exception, or we cannot read or
    write to the file, then the test fails.
    This assumes make_default_security_attributes and
    make_default_open_file_params are correct.
    */
   TEST_CLASS(OpenFileHelperTests)
   {
      public:

      TEST_METHOD(OpenNewFileWrite)
      {
         auto root = ApplicationData::Current().LocalFolder().Path();
         winrt::hstring newFilePath(root + L"\\OpenNewFileWrite.txt");
         DeleteFile(newFilePath.c_str());

         file_handle handle;
         try
         {
            qgl::content::open_file_write(newFilePath.c_str(), &handle);

         }
         catch (winrt::hresult_error&)
         {
            Assert::Fail(L"Exception opening the file.");
         }

         std::string buffer = "Test";
         DWORD written = 0;
         auto result = WriteFile(handle.get(),
                                 buffer.c_str(),
                                 static_cast<DWORD>(buffer.size()),
                                 &written,
                                 nullptr);
         auto lastError = GetLastError();
         Assert::IsTrue(result != 0, L"Failed to write to the file.");

         char readBack[5];
         result = ReadFile(handle.get(), readBack, 4, &written, nullptr);
         lastError = GetLastError();

         Assert::IsTrue(result == 0,
                        L"Writing the file is supposed to fail.");

         Assert::IsTrue(lastError == ERROR_ACCESS_DENIED,
                        L"After attempting to read, the last error should"
                        L" be ACCESS_DENIED.");

         handle.close();
      }

      TEST_METHOD(OpenNewFileReadWrite)
      {
         auto root = ApplicationData::Current().LocalFolder().Path();
         winrt::hstring newFilePath(root + L"\\OpenNewFileReadWrite.txt");
         DeleteFile(newFilePath.c_str());

         file_handle handle;
         try
         {
            qgl::content::open_file_readwrite(newFilePath.c_str(), &handle);
         }
         catch (winrt::hresult_error&)
         {
            Assert::Fail(L"Exception opening the file.");
         }

         std::string buffer = "Test";
         DWORD written = 0;
         auto result = WriteFile(handle.get(),
                                 buffer.c_str(),
                                 static_cast<DWORD>(buffer.size()),
                                 &written,
                                 nullptr);

         Assert::IsTrue(result > 0, L"Failed to write to the file.");

         char readBack[5];
         result = ReadFile(handle.get(), readBack, 4, &written, nullptr);

         Assert::IsTrue(result != 0, L"Failed to read the file.");

         handle.close();
      }

      TEST_METHOD(OpenStorageFileReadWrite)
      {
         test_open_storage_file_rw();
      }

      private:
      winrt::fire_and_forget test_open_storage_file_rw()
      {
         auto root = ApplicationData::Current().LocalFolder();
         auto f = co_await root.CreateFileAsync(
            L"StorageFile.txt",
            CreationCollisionOption::ReplaceExisting);

         file_handle handle;
         try
         {
            open_file_readwrite_sf(f, &handle);
         }
         catch (winrt::hresult_error&)
         {
            Assert::Fail(L"Exception opening the file.");
         }

         std::string buffer = "Test";
         DWORD written = 0;
         auto result = WriteFile(handle.get(),
                                 buffer.c_str(),
                                 static_cast<DWORD>(buffer.size()),
                                 &written,
                                 nullptr);

         Assert::IsTrue(result > 0, L"Failed to write to the file.");

         char readBack[4];
         OVERLAPPED ovr;
         make_overlapped(0, &ovr);
         result = ReadFile(handle.get(), readBack, 4, &written, &ovr);

         Assert::IsTrue(result != 0, L"Failed to read the file.");

         handle.close();
      }
   };


}