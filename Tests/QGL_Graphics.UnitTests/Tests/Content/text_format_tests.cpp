#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace qgl::graphics;

namespace QGL_Graphics_UnitTests
{
   TEST_CLASS(TextFormatTests)
   {
      TEST_METHOD(DefaultBuffer)
      {
         descriptors::text_format_descriptor desc;
         auto fac_p = helpers::make_text_factory();
         text_format f{ desc, fac_p.get() };
      }
   };
}