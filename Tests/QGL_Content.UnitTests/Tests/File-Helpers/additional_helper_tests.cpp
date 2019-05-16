#include "pch.h"
#include "CppUnitTest.h"
#include <winrt/Windows.ApplicationModel.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace winrt::Windows::Storage;
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
         auto handle = open_file_write(newFilePath);

         //Write data to the file.
         write_file_sync(handle, bufferSize, 0, buffer);

         //Close the file.
         handle.close();

         //Open the file with read permissions.
         handle = open_file_read(newFilePath);

         //Get file size.
         auto fileSize = file_size(handle);

         Assert::AreEqual(bufferSize, fileSize,
                          L"The file size is not correct.");
      }
   };
}