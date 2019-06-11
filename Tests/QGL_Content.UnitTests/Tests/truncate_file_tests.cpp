#include "pch.h"
#include "CppUnitTest.h"
#include <winrt/Windows.ApplicationModel.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace winrt::Windows::Storage;
using namespace qgl::content;

namespace QGL_Content_UnitTests
{
   /*
    Truncating tests pass if, after truncating the file, the file size is 0.
    This assumes open_file_write, read_file_sync, write_file_sync, 
    make_default_security_attributes, file_size, and 
    make_default_open_file_params are correct.
    */
   TEST_CLASS(TunrcateFileTests)
   {
      public:
      TEST_METHOD(TruncateExistingFile)
      {
         //Create a file.
         auto root = ApplicationData::Current().LocalFolder().Path();
         winrt::hstring newFilePath(root + L"\\TruncateExistingFile.txt");
         DeleteFile(newFilePath.c_str());

         file_handle handle;
         auto hr = qgl::content::open_file_write(newFilePath.c_str(), &handle);
         Assert::IsTrue(SUCCEEDED(hr), L"HRESULT failed.");

         //Write to it.
         std::string buffer("Test");
         hr = write_file_sync(&handle, buffer.size(), 0, buffer.c_str());
         Assert::IsTrue(SUCCEEDED(hr), L"HRESULT failed.");

         //Get file size.
         size_t fileSize = 0;
         hr = file_size(&handle, &fileSize);
         Assert::IsTrue(SUCCEEDED(hr), L"HRESULT failed.");

         //Truncate file
         hr = truncate_file(&handle);
         Assert::IsTrue(SUCCEEDED(hr), L"HRESULT failed.");

         //Get file size.
         size_t truncatedSize = 0;
         hr = file_size(&handle, &truncatedSize);
         Assert::IsTrue(SUCCEEDED(hr), L"HRESULT failed.");

         Assert::AreEqual(static_cast<size_t>(0), truncatedSize,
                          L"The file size should be 0.");
      }
   };
}