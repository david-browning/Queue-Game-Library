#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace qgl::content;

namespace QGL_Content_UnitTests
{
   TEST_CLASS(FileHelperTests)
   {
      public:
      TEST_METHOD(FileExists)
      {
         auto here = installed_path();
         
         auto notExists = here + L"/noFile.noFile";
         Assert::IsFalse(file_exists(notExists),
                         L"The file should not exist.");
      }

      TEST_METHOD(DirExists)
      {
         auto here = installed_path();
         Assert::IsTrue(dir_exists(here),
                        L"This path should exist");

         auto notHere = here + L"/NOTEXISTS";
         Assert::IsFalse(dir_exists(notHere),
                         L"Directory should not exist.");
      }
   };
}