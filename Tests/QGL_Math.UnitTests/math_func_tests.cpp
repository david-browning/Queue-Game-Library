#include "pch.h"
#include <CppUnitTest.h>
#include <CppUnitTestLogger.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace qgl;
using namespace qgl::math;

namespace QGL_Math_Unit_Tests
{
   TEST_CLASS(MiscHelperTests)
   {
      TEST_METHOD(SmallestInt)
      {
         static_assert(smallest(9, 1, 4, 0, -1) == -1,
            "Smallest did not return -1.");
      }
   };
}