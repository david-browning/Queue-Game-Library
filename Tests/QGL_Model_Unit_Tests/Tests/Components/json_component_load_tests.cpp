#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace QGL_Model_Unit_Tests
{
   TEST_CLASS(JSONComponentLoadTests)
   {
      TEST_METHOD(CreateJSONComponent)
      {
         // Create a component using this JSON:
         auto json = "{\n\t\"id\": \"5B8B3826ECB84825A3508D149359A72E\",\n\t\"name\": \"rigid_body_param\",\n\t\"description\": \"Rigid Body Details\",\n\t\"parameters\": [\n\t\t{\n\t\t\t\"name\": \"mass\",\n\t\t\t\"description\": \"Mass in Kg\",\n\t\t\t\"type\": \"urational32\",\n\t\t\t\"size\": 1\n\t\t},\n\t\t{\n\t\t\t\"name\": \"collision_sphere\",\n\t\t\t\"description\": \"Bounding Sphere\",\n\t\t\t\"type\": \"compound\",\n\t\t\t\"parameters\": [\n\t\t\t\t{\n\t\t\t\t\t\"name\": \"radius\",\n\t\t\t\t\t\"description\": \"Sphere Radius in M\",\n\t\t\t\t\t\"type\": \"urational32\",\n\t\t\t\t\t\"size\": 1\n\t\t\t\t},\n\t\t\t\t{\n\t\t\t\t\t\"name\": \"pos\",\n\t\t\t\t\t\"description\": \"Sphere center in M\",\n\t\t\t\t\t\"type\": \"urational32\",\n\t\t\t\t\t\"size\": 3\n\t\t\t\t}\n\t\t\t]\n\t\t},\n\t\t{\n\t\t\t\"name\": \"cor\",\n\t\t\t\"description\": \"Coefficient of Restitution\",\n\t\t\t\"type\": \"urational8\",\n\t\t\t\"size\": 1\n\t\t}\n\t]\n}";
         qgl::components::icomponent_metadata* compMeta = nullptr;
         auto result = qgl::components::make_component_from_json(
            json, &compMeta);
         Assert::IsTrue(SUCCEEDED(result), L"Could not load JSON");

         auto compMeta_p = qgl::make_unique<qgl::components::icomponent_metadata>(
            compMeta);

         std::string compName{ "rigid_body_param" };
         auto nameLen = compMeta_p->name(nullptr);
         Assert::IsTrue(nameLen > 0, L"Name length is 0");
         std::vector<char> nameBuffer(static_cast<size_t>(nameLen), '\0');
         nameLen = compMeta_p->name(nameBuffer.data());
         Assert::IsTrue(nameLen > 0, L"Name length is 0");
         Assert::IsTrue(strcmp(compName.c_str(), nameBuffer.data()) == 0,
                        L"Names are not equal.");

         std::string compDesc{ "Rigid Body Details" };
         auto descLen = compMeta_p->description(nullptr);
         Assert::IsTrue(descLen > 0, L"Description length is 0");
         std::vector<char> descBuffer(descLen, '\0');
         descLen = compMeta_p->description(descBuffer.data());
         Assert::IsTrue(descLen > 0, L"Description length is 0");
         Assert::IsTrue(strcmp(compDesc.c_str(), descBuffer.data()) == 0,
                        L"Descriptions are not equal");

         // Check the GUID
         qgl::guid expectedGuid{ "5B8B3826ECB84825A3508D149359A72E" };
         qgl::guid compGuid;
         compMeta_p->id(&compGuid);
         Assert::IsTrue(compGuid == expectedGuid, L"GUIDs are not the same");

         // Get the number of parameters
         auto numParams = compMeta_p->size();
         Assert::AreEqual(static_cast<uint32_t>(3), numParams,
                          L"Incorrect number of parameters.");

         std::array<std::string, 3> paramNames =
         {
            "mass",
            "collision_sphere",
            "cor"
         };

         std::array<std::string, 3> paramDescs =
         {
            "Mass in Kg",
            "Bounding Sphere",
            "Coefficient of Restitution"
         };

         std::array<qgl::components::content_param_types, 3> paramTypes =
         {
            qgl::components::known_param_types::known_urational32,
            qgl::components::known_param_types::known_compound,
            qgl::components::known_param_types::known_urational8,
         };

         // Iterate through all the components. None should throw an error
         for (uint32_t i = 0; i < numParams; i++)
         {
            qgl::components::icomponent_param_metadata* param_p = nullptr;
            result = compMeta_p->param(i, &param_p);
            Assert::IsTrue(SUCCEEDED(result));

            std::vector<char> strBuffer;

            auto bufSize = param_p->name(nullptr);
            Assert::IsTrue(bufSize > 0, L"Name buffer is 0");
            strBuffer.resize(bufSize);
            param_p->name(strBuffer.data());
            Assert::IsTrue(strcmp(strBuffer.data(), paramNames[i].c_str()) == 0,
                           L"Name is not equal");

            bufSize = param_p->description(nullptr);
            Assert::IsTrue(bufSize > 0, L"Description buffer is 0");
            strBuffer.resize(bufSize);
            param_p->description(strBuffer.data());
            Assert::IsTrue(strcmp(strBuffer.data(), paramDescs[i].c_str()) == 0,
                           L"Descriptions are not equal");

            Assert::IsTrue(param_p->type() == paramTypes[i],
                           L"Param types are not the same");

            param_p->release();
         }
      }
   };
}