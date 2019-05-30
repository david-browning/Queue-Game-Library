#include "pch.h"
#include "CppUnitTest.h"
#include <winrt/Windows.ApplicationModel.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace QGL_Content_UnitTests
{
   TEST_CLASS(FileAttributeTests)
   {
      public:
      /*
       Check if the application directory exists. Should always be true.
       */
      TEST_METHOD(DirectoryExistsTrue)
      {
         auto installDir = winrt::Windows::ApplicationModel::
            Package::Current().InstalledLocation().Path();

         Assert::IsTrue(qgl::content::dir_exists(installDir.c_str()),
                        L"The install directory should exist.");
      }

      /*
       Check if a non-existent directory exists in the application install
       folder. Should always be false.
       */
      TEST_METHOD(DirectoryExistsFalse)
      {
         auto installDir = winrt::Windows::ApplicationModel::
            Package::Current().InstalledLocation().Path();

         auto dirPath = installDir + L"\\NO_DIR";
         Assert::IsFalse(qgl::content::dir_exists(dirPath.c_str()),
                         L"The directory should not exist.");
      }

      /*
       Test if an existing file is a directory. This should always be false.
       */
      TEST_METHOD(DirectoryExistsNotADirectory)
      {
         auto installDir = winrt::Windows::ApplicationModel::
            Package::Current().InstalledLocation().Path();

         auto fileToCheck = installDir + L"\\Tests\\Test-Files\\CheckMe.txt";

         Assert::IsFalse(qgl::content::dir_exists(fileToCheck.c_str()),
                         L"This is not a directory.");
      }

      /*
       Test if a file exists.
       */
      TEST_METHOD(FileExistsTrue)
      {
         auto fileToCheck = winrt::Windows::ApplicationModel::Package::
            Current().InstalledLocation().Path() + 
            L"\\Tests\\Test-Files\\CheckMe.txt";
         Assert::IsTrue(qgl::content::file_exists(fileToCheck.c_str()),
                        L"The file in the install path should exist.");


         auto rootPath = winrt::Windows::Storage::ApplicationData::Current().
            LocalFolder().Path();

         fileToCheck = rootPath + L"\\FileExistsTrue.txt";
         qgl::content::file_handle handle;
         qgl::content::open_file_write(fileToCheck.c_str(), &handle);
         handle.close();

         Assert::IsTrue(qgl::content::file_exists(fileToCheck.c_str()),
                        L"The file should exist.");
      }

      /*
       Tests if a file that does not exist exists. This should be false.
       */
      TEST_METHOD(FileExistsFalse)
      {
         auto installDir = winrt::Windows::ApplicationModel::
            Package::Current().InstalledLocation().Path();

         auto fileToCheck = installDir + L"NO-FILE.txt";
         Assert::IsFalse(qgl::content::file_exists(fileToCheck.c_str()),
                         L"The file should not exist.");
      }

      /*
       Test if a directory is a file and exists. This should be false.
       */
      TEST_METHOD(FileExistsNotAFile)
      {
         auto installDir = winrt::Windows::ApplicationModel::
            Package::Current().InstalledLocation().Path();

         Assert::IsFalse(qgl::content::file_exists(installDir.c_str()),
                         L"This is not a file.");
      }
   };
}