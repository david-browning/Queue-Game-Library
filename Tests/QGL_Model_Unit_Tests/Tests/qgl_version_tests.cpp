#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace qgl;

namespace QGL_Model_Unit_Tests
{
   TEST_CLASS(QGLVersionTests)
   {
      public:

      /*
       Construct a qgl_version_t using make_version.
       Test the major version is correct by calling version_major().
       */
      TEST_METHOD(VersionMajor)
      {

      }

      /*
       Construct a qgl_version_t using make_version.
       Test the minor version is correct by calling version_minor().
       */
      TEST_METHOD(VersionMinor)
      {

      }

      /*
       Test version_os() is correct.
       */
      TEST_METHOD(OSFlagsCorrect)
      {

      }

      /*
       Use make_win_version() to create a version.
       Verify that version_os() returns the Windows flag. 
       */
      TEST_METHOD(MakeWindowsVersion)
      {

      }

      /*
       Test that the hash of a version is correct.
       */
      TEST_METHOD(HashesCorrect)
      {
         
      }
   };
}