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

         auto handle = qgl::content::open_file_write(newFilePath);

         //Write to it.
         std::string buffer("Test");
         write_file_sync(handle, buffer.size(), 0, buffer.c_str());

         //Close the file.
         handle.close();

         //Open file for read write
         handle = open_file_readwrite(newFilePath);

         //Get file size.
         auto fileSize = file_size(handle);

         //Truncate file
         truncate_file(handle);

         //Close file
         handle.close();

         //Open file for read.
         handle = open_file_read(newFilePath);

         //Get file size.
         auto truncatedSize = file_size(handle);

         Assert::AreEqual(static_cast<size_t>(0), truncatedSize,
                          L"The file size should be 0.");
      }
   };
}