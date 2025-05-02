#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace qgl;

namespace QGL_Model_Unit_Tests
{
   class test_command : public qgl::icommand<std::string>
   {
      public:
      test_command(const qgl::guid& id, std::string name) :
         m_name(std::move(name)), icommand(id)
      {
      }

      void execute(std::string&) override 
      { 
         /* no-op */ 
      }

      void undo() override
      {

      }

      std::string name() const { return m_name; }

      private:
      std::string m_name;
   };

   TEST_CLASS(CommandLocatorTests)
   {
      public:

      TEST_METHOD(RegisterAndLocate)
      {
         command_locator<std::string> locator;
         auto id = guid("00112233445566778899AABBCCDDEEFF");

         test_command cmd(id, "Test1");
         locator.add(&cmd);

         auto* found = locator.get(id);
         Assert::IsNotNull(found);
         Assert::AreEqual(
            "Test1", 
            static_cast<const test_command*>(found)->name().c_str());
      }

      TEST_METHOD(ContainsAndAt)
      {
         command_locator<std::string> locator;
         auto id = guid("01010101010101010101010101010101");

         test_command cmd(id, "CmdX");
         locator.add(&cmd);

         Assert::IsTrue(locator.contains(id));
         Assert::AreEqual(
            "CmdX", 
            static_cast<const test_command*>(locator.get(id))->name().c_str());
      }

      TEST_METHOD(AtThrowsIfMissing)
      {
         command_locator<std::string> locator;
         auto missing = guid("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA");

         try
         {
            locator.get(missing);
            Assert::Fail(L"Expected exception not thrown.");
         }
         catch (const std::out_of_range&)
         {
            // Success
         }
      }

      TEST_METHOD(Clear_RemovesAll)
      {
         command_locator<std::string> locator;
         auto id = guid("00112233445566778899AABBCCDDEEFF");

         test_command cmd(id, "ShouldBeCleared");
         locator.add(&cmd);
         Assert::IsTrue(locator.contains(id));
         locator.clear();
         Assert::IsFalse(locator.contains(id));
      }
   };
}