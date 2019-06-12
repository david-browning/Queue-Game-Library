#include "pch.h"
#include "CppUnitTest.h"
#include <winrt/Windows.Storage.h>
#include <winrt/Windows.ApplicationModel.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace winrt::Windows::Storage;
using namespace winrt::Windows;
using namespace qgl::content;

namespace QGL_Content_UnitTests
{
   TEST_CLASS(FileHelperTests)
   {
      public:
      /*
       Testing an overlapped means checking that the high and low byte offsets
       are correct.
       */
      TEST_METHOD(CreateOverlapped)
      {
         size_t largeSize = 0xFFFF'FFF8'FFFF'FFFE;
         DWORD expectedUpperBytes = 0xFFFF'FFF8;
         DWORD expectedLowerBytes = 0xFFFF'FFFE;

         OVERLAPPED overlapped;
         qgl::content::make_overlapped(largeSize, &overlapped);

         Assert::AreEqual(expectedLowerBytes, overlapped.Offset,
                          L"The low bytes are not correct.");

         Assert::AreEqual(expectedUpperBytes, overlapped.OffsetHigh,
                          L"The high bytes are not correct.");

         Assert::IsTrue(overlapped.hEvent == nullptr,
                        L"The overlapped event should be null.");
      }

      /*
       Create a file with some known size. Check if the function to get the file
       size returns the expected size.
       Assumes open_file_read and open_file_write are correct.
       */
      TEST_METHOD(FileSize)
      {
         static constexpr size_t bufferSize = 8;
         char buffer[bufferSize];

         auto root = ApplicationData::Current().LocalFolder().Path();
         winrt::hstring newFilePath(root + L"\\FileSize.txt");

         //Open a file for writing.
         file_handle handle;
         auto hr = open_file_write(newFilePath.c_str(), &handle);
         Assert::IsTrue(SUCCEEDED(hr), L"HRESULT failed.");

         //Write data to the file.
         hr = write_file_sync(&handle, bufferSize, 0, buffer);
         Assert::IsTrue(SUCCEEDED(hr), L"HRESULT failed.");

         //Close the file.
         handle.close();

         //Open the file with read permissions.
         hr = open_file_read(newFilePath.c_str(), &handle);
         Assert::IsTrue(SUCCEEDED(hr), L"HRESULT failed.");

         //Get file size.
         size_t fileSize = 0;
         hr = file_size(&handle, &fileSize);
         Assert::IsTrue(SUCCEEDED(hr), L"HRESULT failed.");

         Assert::AreEqual(bufferSize, fileSize,
                          L"The file size is not correct.");
      }

      TEST_METHOD(StorageFileSize)
      {
         test_storage_file_size();
      }

      private:
      winrt::fire_and_forget test_storage_file_size()
      {
         auto root = ApplicationModel::Package::Current().InstalledLocation();

         auto f = co_await root.CreateFileAsync(
            L"Tests\\Test-Files\\CheckMe.txt",
            CreationCollisionOption::OpenIfExists);

         size_t fileSize = 0;
         auto hr = file_size_sf(f, &fileSize);
         Assert::IsTrue(SUCCEEDED(hr), L"HRESULT failed.");

         Assert::AreEqual(static_cast<size_t>(8), fileSize,
                          L"The file size is not correct.");
      }
   };
}