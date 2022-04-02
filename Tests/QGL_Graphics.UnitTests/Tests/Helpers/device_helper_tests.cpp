#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace qgl::graphics;

namespace QGL_Graphics_UnitTests
{
   /*
    For most of these, just make sure they don't throw an exception.
    */
   TEST_CLASS(DeviceHelperTests)
   {
      public:
      TEST_METHOD(DXGIFormatSize)
      {
         Assert::AreEqual(size_t(16),
                          helpers::format_size(DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT),
                          L"DXGI_FORMAT_R32G32B32A32_FLOAT is not 16 bytes.");
      }

      TEST_METHOD(MakeGPUFactory)
      {
         try
         {
            auto factory = helpers::make_gpu_factory(true);
         }
         catch (...)
         {
            Assert::Fail(L"Exception thrown.");
         }
      }

      TEST_METHOD(Make2DFactory)
      {
         try
         {
            auto factory = helpers::make_2d_factory(true);
         }
         catch (...)
         {
            Assert::Fail(L"Exception thrown.");
         }
      }

      TEST_METHOD(MakeTextFactory)
      {
         try
         {
            auto factory = helpers::make_text_factory();
         }
         catch (...)
         {
            Assert::Fail(L"Exception thrown.");
         }
      }

      TEST_METHOD(EnumHardwareAdapters)
      {
         try
         {
            auto factory = helpers::make_gpu_factory(true);
            auto adapters = helpers::enum_hardware_adapters(
               factory.get(), D3D_FEATURE_LEVEL_12_0);

            Assert::IsTrue(adapters.size() > 0, L"There are no adapters.");
         }
         catch (...)
         {
            Assert::Fail(L"Exception thrown.");
         }
      }

      TEST_METHOD(Make3DDevice)
      {
         try
         {
            auto factory = helpers::make_gpu_factory(true);
            auto adapters = helpers::enum_hardware_adapters(
               factory.get(), D3D_FEATURE_LEVEL_12_0);

            auto dev = helpers::make_3d_device(
               adapters.front().get(), D3D_FEATURE_LEVEL_12_0);
         }
         catch (...)
         {
            Assert::Fail(L"Exception thrown.");
         }
      }
   };
}