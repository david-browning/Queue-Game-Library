#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace QGL_Model_Unit_Tests
{
   TEST_CLASS(ModuleComponentTests)
   {
      TEST_METHOD(LoadModelComponents)
      {
         // Try to get the components in the QGL_MODEL DLL
         qgl::imodule* m = nullptr;
         auto result = qgl::make_module(
            L"QGL_Model.dll", qgl::QGL_VERSION_LATEST, &m);

         Assert::IsTrue(SUCCEEDED(result), 
                        L"Failed to load QGL_Model.dll");
         auto m_p = qgl::make_unique<qgl::imodule>(m);


         // Get the address of the module's component provider
         qgl::components::get_component_provider_functor providerProc;
         result = qgl::components::component_prodiver_proc(
            m_p.get(),
            &providerProc);
         Assert::IsTrue(SUCCEEDED(result),
                        L"Failed to get the provider proc");

         qgl::components::icomponent_provider* provider = nullptr;
         result = providerProc(qgl::QGL_VERSION_LATEST, &provider);
         Assert::IsTrue(SUCCEEDED(result), 
                        L"Failed to call the provider proc.");
         auto provider_p = qgl::make_unique<qgl::components::icomponent_provider>(provider);

         // Now try calling QGL_Model's dedicated provider proc.
         qgl::components::icomponent_provider* provider2 = nullptr;
         result = qgl::components::qgl_get_components(
            qgl::QGL_VERSION_LATEST, 
            &provider2);
         Assert::IsTrue(SUCCEEDED(result), 
                        L"Failed to call the dedicated provider proc.");
         auto provider2_p = qgl::make_unique<qgl::components::icomponent_provider>(provider2);
      }
   };
}