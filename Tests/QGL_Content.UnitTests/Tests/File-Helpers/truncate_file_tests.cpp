#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace QGL_Content_UnitTests
{
   /*
    Truncating tests pass if, after truncating the file, the file size is 0.
    */
   TEST_CLASS(TunrcateFileTests)
   {
      public:
      TEST_METHOD(TruncateNoFile)
      {

      }

      TEST_METHOD(TruncateExistingFile)
      {

      }
   };
}