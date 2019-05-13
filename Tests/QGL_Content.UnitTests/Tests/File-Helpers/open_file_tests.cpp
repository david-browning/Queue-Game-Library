#include "pch.h"
#include "CppUnitTest.h"
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
    This assumes fill_security_attributes and 
    fill_createfile_extended_parameters are correct.
    */
   TEST_CLASS(OpenFileHelperTests)
   {
      public:

      TEST_METHOD(OpenNewFileWrite)
      {
         auto root = ApplicationData::Current().LocalFolder().Path();
         winrt::hstring newFilePath(root + L"\\OpenNewFileWrite.txt");

         SECURITY_ATTRIBUTES sa = qgl::content::fill_security_attributes();
         auto openParameters = fill_createfile_extended_parameters(&sa);

         winrt::file_handle handle;
         try
         {
            handle = qgl::content::open_file_write(newFilePath,
                                                   openParameters);

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

         Assert::IsTrue(result != 0, L"Failed to write to the file.");

         char readBack[5];
         result = ReadFile(handle.get(), readBack, 4, &written, nullptr);
         auto lastError = GetLastError();

         Assert::IsTrue(result == 0,
                        L"Writing the file is supposed to fail.");

         Assert::IsTrue(lastError == ERROR_ACCESS_DENIED,
                        L"After attempting to read, the last error should"
                        L" be ACCESS_DENIED.");

         handle.close();

         DeleteFile(newFilePath.c_str());

      }

      TEST_METHOD(OpenNewFileReadWrite)
      {
         auto root = ApplicationData::Current().LocalFolder().Path();
         winrt::hstring newFilePath(root + L"\\OpenNewFileReadWrite.txt");

         SECURITY_ATTRIBUTES sa = qgl::content::fill_security_attributes();
         auto openParameters = fill_createfile_extended_parameters(&sa);

         winrt::file_handle handle;
         try
         {
            handle = qgl::content::open_file_readwrite(newFilePath,
                                                       openParameters);
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

         DeleteFile(newFilePath.c_str());
      }
   };
}