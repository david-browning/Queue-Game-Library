#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace qgl;

namespace QGL_Model_Unit_Tests
{
   TEST_CLASS(ICommandTests)
   {
      public:
      /*
       Create a command and inspect the GUID.
       */
      TEST_METHOD(Constructor)
      {

      }

      /*
       Create a command and use the copy constructor to create a copy. Verify 
       the GUIDs are equal.
       */
      TEST_METHOD(CopyConstructor)
      {

      }

      /*
       Create two commands with the same GUID. Create a third command by moving
       one of the same commands into the new one. Verify the original and moved 
       GUIDs are equal.
       */
      TEST_METHOD(MoveConstructor)
      {

      }

      /*
       Verify that calling execute changes some state.
       */
      TEST_METHOD(Execute)
      {
                    
      }

      /*
       Verify that calling undo after calling execute restore the some state to
       its state before calling execute.
       */
      TEST_METHOD(Undo)
      {

      }

      /*
       Check that a command's function operator calls execute.
       */
      TEST_METHOD(FunctionOperator)
      {

      }

      /*
       Create two commands with the same GUID. Check that the equality operator
       is correct.
       Create another command with a different GUID. Check that the not-equal
       operator is correct.
       */
      TEST_METHOD(EqualityOperator)
      {

      }

      /*
       Create two commands with different GUIDs. Check that the less than
       operator is correct.
       */
      TEST_METHOD(LessThanOperator)
      {

      }
   };
}