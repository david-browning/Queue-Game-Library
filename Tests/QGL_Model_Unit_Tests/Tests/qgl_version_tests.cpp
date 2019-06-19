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
       Create a version and use its accessors to verify the constructor works.
       */
      TEST_METHOD(Construct)
      {

      }
      /*
       Create a version, create a copy using the copy constructor.
       Check that the members are equal.
       */
      TEST_METHOD(CopyConstruct)
      {

      }

      /*
       Assume copy constructor is correct.
       Create a version, copy it using the copy constructor. Check that ==
       returns true.
       Create another version. Check that == returns false.
       */
      TEST_METHOD(EqualityOperator)
      {

      }

      /*
       Assume equality operator is correct.
       Create two different versions. Assign one to the other using =. Verify
       the overwritten version is the same.
       */
      TEST_METHOD(AssignOperator)
      {

      }

      /*
       Assume equality operator is correct.
       */
      TEST_METHOD(MoveConstruct)
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
         size_t expectedHash;
      }
   };
}