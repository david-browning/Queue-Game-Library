#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace qgl::content;

namespace QGL_Content_UnitTests
{
   TEST_CLASS(StringItemTests)
   {
      public:
      TEST_METHOD(Construct)
      {
         string_item item("Test",
                          L"Name",
                          0,
                          RESOURCE_TYPES::RESOURCE_TYPE_BRUSH,
                          CONTENT_LOADER_IDS::CONTENT_LOADER_ID_BRUSH);

         Assert::AreEqual(0, strcmp("Test", item.c_str()),
                          L"The data string is not correct.");

         Assert::AreEqual(0, wcscmp(L"Name", item.name()),
                          L"The name is not correct.");

         Assert::AreEqual(RESOURCE_TYPES::RESOURCE_TYPE_BRUSH,
                          item.resource_type(),
                          L"The resource type is not correct.");

         Assert::AreEqual(CONTENT_LOADER_IDS::CONTENT_LOADER_ID_BRUSH,
                          item.loader_id(),
                          L"The content loader id is not correct.");

         Assert::AreEqual(static_cast<content_id>(0),
                          item.id(),
                          L"The item ID is not correct.");
      }

      TEST_METHOD(CopyConstructor)
      {
         string_item item("Test",
                          L"Name",
                          0,
                          RESOURCE_TYPES::RESOURCE_TYPE_BRUSH,
                          CONTENT_LOADER_IDS::CONTENT_LOADER_ID_BRUSH);

         string_item itemCopy(item);
         Assert::AreEqual(item.id(), itemCopy.id(),
                          L"The IDs should be equal");

         Assert::AreEqual(item.resource_type(), itemCopy.resource_type(),
                          L"The resource types are equal.");

         Assert::AreEqual(item.loader_id(), itemCopy.loader_id(),
                          L"The loader IDs should be equal.");

         Assert::AreEqual(0, strcmp(item.c_str(), itemCopy.c_str()),
                          L"The data is not equal.");

         Assert::AreEqual(0, wcscmp(item.name(), itemCopy.name()),
                          L"The names are not equal.");
      }


      /*
       Assume copy constructor is correct.
       */
      TEST_METHOD(MoveConstructor)
      {
         string_item item("Test",
                          L"Name",
                          0,
                          RESOURCE_TYPES::RESOURCE_TYPE_BRUSH,
                          CONTENT_LOADER_IDS::CONTENT_LOADER_ID_BRUSH);

         string_item itemCopy(item);
         string_item itemMove(std::move(itemCopy));
         Assert::AreEqual(item.id(), itemMove.id(),
                          L"The IDs should be equal");

         Assert::AreEqual(item.resource_type(), itemMove.resource_type(),
                          L"The resource types are equal.");

         Assert::AreEqual(item.loader_id(), itemMove.loader_id(),
                          L"The loader IDs should be equal.");

         Assert::AreEqual(0, strcmp(item.c_str(), itemMove.c_str()),
                          L"The data is not equal.");

         Assert::AreEqual(0, wcscmp(item.name(), itemMove.name()),
                          L"The names are not equal.");
      }

      TEST_METHOD(AssignOperator)
      {
         string_item item("Test",
                          L"Name",
                          0,
                          RESOURCE_TYPES::RESOURCE_TYPE_BRUSH,
                          CONTENT_LOADER_IDS::CONTENT_LOADER_ID_BRUSH);


         string_item itemCopy = item;
         Assert::AreEqual(item.id(), itemCopy.id(),
                          L"The IDs should be equal");

         Assert::AreEqual(item.resource_type(), itemCopy.resource_type(),
                          L"The resource types are equal.");

         Assert::AreEqual(item.loader_id(), itemCopy.loader_id(),
                          L"The loader IDs should be equal.");

         Assert::AreEqual(0, strcmp(item.c_str(), itemCopy.c_str()),
                          L"The data is not equal.");

         Assert::AreEqual(0, wcscmp(item.name(), itemCopy.name()),
                          L"The names are not equal.");
      }

      TEST_METHOD(OverwriteData)
      {
         string_item item("Test",
                          L"Name",
                          0,
                          RESOURCE_TYPES::RESOURCE_TYPE_BRUSH,
                          CONTENT_LOADER_IDS::CONTENT_LOADER_ID_BRUSH);

         item.data()[0] = L'B';

         Assert::AreEqual(0, strcmp("Best", item.c_str()),
                          L"The data should be equal.");
      }

      /*
       Assume assignment operator is correct.
       */
      TEST_METHOD(EqualityOperator)
      {
         string_item item("Test",
                          L"Name",
                          0,
                          RESOURCE_TYPES::RESOURCE_TYPE_BRUSH,
                          CONTENT_LOADER_IDS::CONTENT_LOADER_ID_BRUSH);

         string_item itemCopy = item;
         Assert::IsTrue(item == itemCopy,
                        L"The items should be equal.");

         string_item itemNE("Test",
                            L"Name2",
                            0,
                            RESOURCE_TYPES::RESOURCE_TYPE_BRUSH,
                            CONTENT_LOADER_IDS::CONTENT_LOADER_ID_BRUSH);

         Assert::IsFalse(item == itemNE,
                         L"The items should not be equal.");

         string_item itemNE2("Test2",
                             L"Name",
                             0,
                             RESOURCE_TYPES::RESOURCE_TYPE_BRUSH,
                             CONTENT_LOADER_IDS::CONTENT_LOADER_ID_BRUSH);

         Assert::IsFalse(item == itemNE2,
                         L"The itemNE 2 should not be equal.");
      }
   };
}