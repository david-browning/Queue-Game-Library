#include "pch.h"
#include "CppUnitTest.h"
#include <set>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace qgl;
using namespace qgl::math;

namespace QGL_Math_Unit_Tests
{
   TEST_CLASS(RandomTests)
   {
      TEST_METHOD(RandomInt)
      {
         auto i1 = random::uniform_int(1, 255);
         auto i2 = random::uniform_int(1, 255);
         Assert::AreNotEqual(i1, i2, L"Ints should not be the same.");
      }

      TEST_METHOD(RandomFloat)
      {
         auto f1 = random::uniform_real(1.0f, 255.0f);
         auto f2 = random::uniform_real(1.0f, 255.0f);
         Assert::AreNotEqual(f1, f2, L"Floats should not be the same.");
      }

      TEST_METHOD(ChooseRandom)
      {
         std::set<int> numberSet = { 1, 2, 3, 4, 5 };
         auto num = random::choose(numberSet.begin(), numberSet.end());
         Assert::IsTrue(num <= 5,
                        L"A number not in the set was chosen.");

         std::vector<int> numbers = { 1, 2, 3, 4, 5, 6, 7 };
         num = random::choose(numbers);
         Assert::IsTrue(num <= 7,
                        L"A number not in the vector was chosen.");
      }

      TEST_METHOD(NameGenerator)
      {
         std::vector<std::string> names = {
            "David", "AJ", "Claire", "Christie", "Justin"
         };

         std::set<std::string> nameSet{ names.begin(), names.end() };
         random::name_generator gen{ names.begin(), names.end() };
         auto name = gen.get();
         Assert::IsTrue(nameSet.count(name) == 1,
                        L"Name was not found in the set.");
      }
   };
}