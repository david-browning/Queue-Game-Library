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
       Test passes if we can open a file using the security parameters.
       */
      TEST_METHOD(CreateDefaultSecurityParameters)
      {
         auto root = ApplicationData::Current().LocalFolder().Path();
         winrt::hstring newFilePath(root + 
                                    L"\\CreateDefaultSecurityParameters.txt");

         SECURITY_ATTRIBUTES sa = qgl::content::make_default_security_attributes();
         CREATEFILE2_EXTENDED_PARAMETERS openParams = { 0 };
         openParams.dwSize = sizeof(CREATEFILE2_EXTENDED_PARAMETERS);
         openParams.dwFileAttributes = FILE_ATTRIBUTE_NORMAL;
         openParams.dwFileFlags = FILE_FLAG_SEQUENTIAL_SCAN;
         openParams.dwSecurityQosFlags = SECURITY_ANONYMOUS;
         openParams.hTemplateFile = nullptr;
         openParams.lpSecurityAttributes = &sa;

         try
         {
            auto handle = open_file_write(newFilePath, openParams);
         }
         catch (winrt::hresult_error&)
         {
            Assert::Fail(L"Exception opening the file.");
         }
      }

      /*
       Test passes if we can open a file using the create file parameters.
       Assumes make_default_security_attributes is correct.
       */
      TEST_METHOD(CreateDefaultOpenParameters)
      {
         auto root = ApplicationData::Current().LocalFolder().Path();
         winrt::hstring newFilePath(root +
                                    L"\\CreateDefaultOpenParameters.txt");

         SECURITY_ATTRIBUTES sa = qgl::content::make_default_security_attributes();
         auto openParams = make_default_open_file_params(&sa);

         try
         {
            auto handle = open_file_write(newFilePath, openParams);
         }
         catch (winrt::hresult_error&)
         {
            Assert::Fail(L"Exception opening the file.");
         }
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
         SECURITY_ATTRIBUTES sa = qgl::content::make_default_security_attributes();
         auto openParams = make_default_open_file_params(&sa);

         //Open a file for writing.
         auto handle = open_file_write(newFilePath, openParams);

         //Write data to the file.
         write_file_sync(handle, bufferSize, 0, buffer);

         //Close the file.
         handle.close();

         //Open the file with read permissions.
         handle = open_file_read(newFilePath, openParams);

         //Get file size.
         auto fileSize = file_size(handle);

         Assert::AreEqual(bufferSize, fileSize,
                          L"The file size is not correct.");
      }
   };
}