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
         guid gCopy{ m_g };

         basic_command<std::string> c{ std::move(gCopy), "1"};
         Assert::IsTrue(c.guid() == m_g,
                        L"GUIDs are not the same");
      }

      /*
       Create a command and use the copy constructor to create a copy. Verify
       the GUIDs are equal.
       */
      TEST_METHOD(CopyConstructor)
      {
         guid gCopy{ m_g };

         basic_command<std::string> c{ std::move(gCopy), "1"};
         basic_command<std::string> cCopy {c};

         Assert::IsTrue(c == cCopy, L"GUIDs are not the same.");
      }

      /*
       Create two commands with the same GUID. Create a third command by moving
       one of the same commands into the new one. Verify the original and moved
       GUIDs are equal.
       */
      TEST_METHOD(MoveConstructor)
      {
         guid gCopy{ m_g };

         basic_command<std::string> c{ std::move(gCopy), "1"};
         basic_command<std::string> cCopy {c};
         basic_command<std::string> cMove {std::move(cCopy)};

         Assert::IsTrue(c == cMove, L"GUIDs are not the same.");
      }

      /*
       Verify that calling execute changes some state.
       */
      TEST_METHOD(Execute)
      {
         basic_command<std::string> c {m_g, "1"};
         Assert::AreEqual(std::string{"1"}, c.state(),
                          L"State is not 1.");

         std::string newState{"2"};
         c.execute(newState);
         Assert::AreEqual(newState, c.state(),
                          L"State is not 2");
      }

      /*
       Verify that calling undo after calling execute restore the some state to
       its state before calling execute.
       */
      TEST_METHOD(Undo)
      {
         basic_command<std::string> c {m_g, "1"};
         std::string newState{"2"};
         c.execute(newState);
         c.undo();
         Assert::AreEqual(std::string{"1"}, c.state(),
                 L"State is not 1.");
      }

      /*
       Check that a command's function operator calls execute.
       */
      TEST_METHOD(FunctionOperator)
      {
         basic_command<std::string> c {m_g, "1"};
         Assert::AreEqual(std::string{"1"}, c.state(),
                          L"State is not 1.");

         std::string newState{"2"};
         c(newState);
         Assert::AreEqual(newState, c.state(),
                          L"State is not 2");
      }

      /*
       Create two commands with the same GUID. Check that the equality operator
       is correct.
       Create another command with a different GUID. Check that the not-equal
       operator is correct.
       */
      TEST_METHOD(EqualityOperator)
      {
         basic_command<std::string> c1 {m_g, "1"};
         basic_command<std::string> c2 {m_g, "2"};
         Assert::IsTrue(c1 == c2, L"Commands should be equal.");

         guid g2{ "2491363A8BC44C05B851035BC0099624" };
         basic_command<std::string> c3 {g2, "3"};
         Assert::IsFalse(c1 == c3, L"Commands should not be equal.");
         Assert::IsTrue(c1 != c3, L"Commands should not be equal (!=)");
      }

      private:
      static constexpr guid m_g{ "3491363A8BC44C05B851035BC0099624" };
   };
}