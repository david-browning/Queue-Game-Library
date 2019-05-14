#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace qgl::content;

namespace QGL_Content_UnitTests
{
   TEST_CLASS(ContentMetadataTests)
   {
      public:
      /*
       Assumes class getters are correct.
       */
      TEST_METHOD(DefaultConstructor)
      {
         CONTENT_METADATA_BUFFER m;
         Assert::IsTrue(RESOURCE_TYPES::RESOURCE_TYPE_UNKNOWN ==
                        static_cast<RESOURCE_TYPES>(m.resource_type()),
                        L"The resource type should be unknown.");

         Assert::IsTrue(CONTENT_LOADER_IDS::CONTENT_LOADER_ID_UNKNOWN ==
                        static_cast<CONTENT_LOADER_IDS>(m.loader_id()),
                        L"The loader should be unknown.");

         Assert::IsTrue(winrt::hstring() == m.name(),
                        L"The name should be empty.");

         Assert::AreEqual(qgl::QGL_VERSION_LATEST,
                          m.version(),
                          L"The version is not correct.");

         Assert::IsTrue(m.content_visible(),
                        L"The content should be visible.");

         Assert::IsFalse(m.obey_physics(),
                         L"The content should not obey physics.");
      }

      TEST_METHOD(MakeConstructor)
      {
         CONTENT_METADATA_BUFFER m(RESOURCE_TYPES::RESOURCE_TYPE_BRUSH,
                                   CONTENT_LOADER_IDS::CONTENT_LOADER_ID_BRUSH,
                                   L"Name",
                                   4);

         Assert::IsTrue(RESOURCE_TYPES::RESOURCE_TYPE_BRUSH ==
                        static_cast<RESOURCE_TYPES>(m.resource_type()),
                        L"The resource type is not correct.");

         Assert::IsTrue(CONTENT_LOADER_IDS::CONTENT_LOADER_ID_BRUSH ==
                        static_cast<CONTENT_LOADER_IDS>(m.loader_id()),
                        L"The loader is not correct.");

         Assert::IsTrue(winrt::to_hstring(L"Name") == m.name(),
                        L"The name is not correct.");

         Assert::AreEqual(qgl::QGL_VERSION_LATEST,
                          m.version(),
                          L"The version is not correct.");

         Assert::IsTrue(m.content_visible(),
                        L"The content should be visible.");

         Assert::IsFalse(m.obey_physics(),
                         L"The content should not obey physics.");
      }

      TEST_METHOD(MakeConstructorWithHString)
      {
         CONTENT_METADATA_BUFFER m(RESOURCE_TYPES::RESOURCE_TYPE_BRUSH,
                                   CONTENT_LOADER_IDS::CONTENT_LOADER_ID_BRUSH,
                                   L"Name");

         Assert::IsTrue(RESOURCE_TYPES::RESOURCE_TYPE_BRUSH ==
                        static_cast<RESOURCE_TYPES>(m.resource_type()),
                        L"The resource type is not correct.");

         Assert::IsTrue(CONTENT_LOADER_IDS::CONTENT_LOADER_ID_BRUSH ==
                        static_cast<CONTENT_LOADER_IDS>(m.loader_id()),
                        L"The loader is not correct.");

         Assert::IsTrue(winrt::to_hstring(L"Name") == m.name(),
                        L"The name is not correct.");

         Assert::AreEqual(qgl::QGL_VERSION_LATEST,
                          m.version(),
                          L"The version is not correct.");

         Assert::IsTrue(m.content_visible(),
                        L"The content should be visible.");

         Assert::IsFalse(m.obey_physics(),
                         L"The content should not obey physics.");
      }

      TEST_METHOD(CopyConstructor)
      {
         CONTENT_METADATA_BUFFER m(RESOURCE_TYPES::RESOURCE_TYPE_BRUSH,
                                   CONTENT_LOADER_IDS::CONTENT_LOADER_ID_BRUSH,
                                   L"Name");

         CONTENT_METADATA_BUFFER metaCopy(m);

         Assert::IsTrue(RESOURCE_TYPES::RESOURCE_TYPE_BRUSH ==
                        static_cast<RESOURCE_TYPES>(metaCopy.resource_type()),
                        L"The resource type is not correct.");

         Assert::IsTrue(CONTENT_LOADER_IDS::CONTENT_LOADER_ID_BRUSH ==
                        static_cast<CONTENT_LOADER_IDS>(metaCopy.loader_id()),
                        L"The loader is not correct.");

         Assert::IsTrue(winrt::to_hstring(L"Name") == metaCopy.name(),
                        L"The name is not correct.");

         Assert::AreEqual(qgl::QGL_VERSION_LATEST,
                          metaCopy.version(),
                          L"The version is not correct.");

         Assert::IsTrue(metaCopy.content_visible(),
                        L"The content should be visible.");

         Assert::IsFalse(metaCopy.obey_physics(),
                         L"The content should not obey physics.");

      }

      TEST_METHOD(MoveConstructor)
      {
         CONTENT_METADATA_BUFFER m(RESOURCE_TYPES::RESOURCE_TYPE_BRUSH,
                                   CONTENT_LOADER_IDS::CONTENT_LOADER_ID_BRUSH,
                                   L"Name");

         CONTENT_METADATA_BUFFER metaMove(std::move(m));

         Assert::IsTrue(RESOURCE_TYPES::RESOURCE_TYPE_BRUSH ==
                        static_cast<RESOURCE_TYPES>(metaMove.resource_type()),
                        L"The resource type is not correct.");

         Assert::IsTrue(CONTENT_LOADER_IDS::CONTENT_LOADER_ID_BRUSH ==
                        static_cast<CONTENT_LOADER_IDS>(metaMove.loader_id()),
                        L"The loader is not correct.");

         Assert::IsTrue(winrt::to_hstring(L"Name") == metaMove.name(),
                        L"The name is not correct.");

         Assert::AreEqual(qgl::QGL_VERSION_LATEST,
                          metaMove.version(),
                          L"The version is not correct.");

         Assert::IsTrue(metaMove.content_visible(),
                        L"The content should be visible.");

         Assert::IsFalse(metaMove.obey_physics(),
                         L"The content should not obey physics.");
      }

      TEST_METHOD(ContentVisibleGet)
      {
         CONTENT_METADATA_BUFFER m(RESOURCE_TYPES::RESOURCE_TYPE_BRUSH,
                                   CONTENT_LOADER_IDS::CONTENT_LOADER_ID_BRUSH,
                                   L"Name");

         Assert::IsTrue(m.content_visible(),
                        L"Content should be visible.");
      }

      /*
       Assumes content_visible getter is correct.
       */
      TEST_METHOD(ContentVisibleSetFalse)
      {
         CONTENT_METADATA_BUFFER m(RESOURCE_TYPES::RESOURCE_TYPE_BRUSH,
                                   CONTENT_LOADER_IDS::CONTENT_LOADER_ID_BRUSH,
                                   L"Name");
         m.content_visible(false);
         Assert::IsFalse(m.content_visible(),
                         L"Content should not be visible.");
      }

      /*
       Assumes content_visible getter is correct.
       */
      TEST_METHOD(ContentVisibleSetTrue)
      {
         CONTENT_METADATA_BUFFER m(RESOURCE_TYPES::RESOURCE_TYPE_BRUSH,
                                   CONTENT_LOADER_IDS::CONTENT_LOADER_ID_BRUSH,
                                   L"Name");
         m.content_visible(true);
         Assert::IsTrue(m.content_visible(),
                         L"Count should be visible.");
      }

      TEST_METHOD(ObeyPhysicsGet)
      {
         CONTENT_METADATA_BUFFER m(RESOURCE_TYPES::RESOURCE_TYPE_BRUSH,
                                   CONTENT_LOADER_IDS::CONTENT_LOADER_ID_BRUSH,
                                   L"Name");
         Assert::IsFalse(m.obey_physics(),
                         L"Content should not obey physics.");
      }

      /*
       Assumes obey_physics getter is correct.
       */
      TEST_METHOD(ObeyPhysicsSetTrue)
      {
         CONTENT_METADATA_BUFFER m(RESOURCE_TYPES::RESOURCE_TYPE_BRUSH,
                                   CONTENT_LOADER_IDS::CONTENT_LOADER_ID_BRUSH,
                                   L"Name");
         m.obey_physics(true);
         Assert::IsTrue(m.obey_physics(),
                        L"Content should obey physics.");
      }

      /*
       Assumes obey_physics getter is correct.
       */
      TEST_METHOD(ObeyPhysicsSetFalse)
      {
         CONTENT_METADATA_BUFFER m(RESOURCE_TYPES::RESOURCE_TYPE_BRUSH,
                                   CONTENT_LOADER_IDS::CONTENT_LOADER_ID_BRUSH,
                                   L"Name");
         m.obey_physics(false);
         Assert::IsFalse(m.obey_physics(),
                         L"Content should not obey physics.");
      }

      TEST_METHOD(NameGet)
      {
         CONTENT_METADATA_BUFFER m(RESOURCE_TYPES::RESOURCE_TYPE_BRUSH,
                                   CONTENT_LOADER_IDS::CONTENT_LOADER_ID_BRUSH,
                                   L"Name");

         Assert::IsTrue(m.name() == L"Name",
                        L"The name is not correct.");
      }

      /*
       Assumes name getter is correct.
       */
      TEST_METHOD(NameSet)
      {
         CONTENT_METADATA_BUFFER m(RESOURCE_TYPES::RESOURCE_TYPE_BRUSH,
                                   CONTENT_LOADER_IDS::CONTENT_LOADER_ID_BRUSH,
                                   L"Name");
         m.name(L"NewName");
         Assert::IsTrue(m.name() == L"NewName",
                        L"The name is not correct.");
      }

      TEST_METHOD(ResourceTypeGet)
      {
         CONTENT_METADATA_BUFFER m(RESOURCE_TYPES::RESOURCE_TYPE_BRUSH,
                                   CONTENT_LOADER_IDS::CONTENT_LOADER_ID_BRUSH,
                                   L"Name");
         Assert::IsTrue(RESOURCE_TYPES::RESOURCE_TYPE_BRUSH ==
                        m.resource_type(),
                        L"The resource type is not correct.");
      }

      /*
       Assumes resource_type const is correct.
       */
      TEST_METHOD(ResourceTypeSet)
      {
         CONTENT_METADATA_BUFFER m(RESOURCE_TYPES::RESOURCE_TYPE_BRUSH,
                                   CONTENT_LOADER_IDS::CONTENT_LOADER_ID_BRUSH,
                                   L"Name");
         m.resource_type() = RESOURCE_TYPES::RESOURCE_TYPE_CAMERA;

         Assert::IsTrue(RESOURCE_TYPES::RESOURCE_TYPE_CAMERA ==
                        m.resource_type(),
                        L"The resource type is not correct.");
      }

      TEST_METHOD(LoaderIDGet)
      {
         CONTENT_METADATA_BUFFER m(RESOURCE_TYPES::RESOURCE_TYPE_BRUSH,
                                   CONTENT_LOADER_IDS::CONTENT_LOADER_ID_BRUSH,
                                   L"Name");
         Assert::IsTrue(CONTENT_LOADER_IDS::CONTENT_LOADER_ID_BRUSH ==
                        m.loader_id(),
                        L"The loader ID is not correct.");
      }

      /*
       Assumes loader_id const is correct.
       */
      TEST_METHOD(LoaderIDSet)
      {
         CONTENT_METADATA_BUFFER m(RESOURCE_TYPES::RESOURCE_TYPE_BRUSH,
                                   CONTENT_LOADER_IDS::CONTENT_LOADER_ID_BRUSH,
                                   L"Name");
         m.loader_id() = CONTENT_LOADER_IDS::CONTENT_LOADER_ID_CAMERA;

         Assert::IsTrue(CONTENT_LOADER_IDS::CONTENT_LOADER_ID_CAMERA ==
                        m.loader_id(),
                        L"The loader ID is not correct.");
      }

      TEST_METHOD(VersionGet)
      {
         CONTENT_METADATA_BUFFER m(RESOURCE_TYPES::RESOURCE_TYPE_BRUSH,
                                   CONTENT_LOADER_IDS::CONTENT_LOADER_ID_BRUSH,
                                   L"Name");

         Assert::AreEqual(qgl::QGL_VERSION_LATEST,
                          m.version(),
                          L"The version is not correct.");
      }

      TEST_METHOD(UniqueGUIDs)
      {
         CONTENT_METADATA_BUFFER m1(RESOURCE_TYPES::RESOURCE_TYPE_BRUSH,
                                    CONTENT_LOADER_IDS::CONTENT_LOADER_ID_BRUSH,
                                    L"Name");

         CONTENT_METADATA_BUFFER m2(RESOURCE_TYPES::RESOURCE_TYPE_BRUSH,
                                    CONTENT_LOADER_IDS::CONTENT_LOADER_ID_BRUSH,
                                    L"Name");

         Assert::IsTrue(m1.guid() != m2.guid(),
                        L"The GUIDs should not be equal.");
      }

      TEST_METHOD(AssignOperator)
      {
         CONTENT_METADATA_BUFFER m(RESOURCE_TYPES::RESOURCE_TYPE_BRUSH,
                                   CONTENT_LOADER_IDS::CONTENT_LOADER_ID_BRUSH,
                                   L"Name");

         auto metaCopy = m;

         Assert::IsTrue(RESOURCE_TYPES::RESOURCE_TYPE_BRUSH ==
                        static_cast<RESOURCE_TYPES>(metaCopy.resource_type()),
                        L"The resource type is not correct.");

         Assert::IsTrue(CONTENT_LOADER_IDS::CONTENT_LOADER_ID_BRUSH ==
                        static_cast<CONTENT_LOADER_IDS>(metaCopy.loader_id()),
                        L"The loader is not correct.");

         Assert::IsTrue(winrt::to_hstring(L"Name") == metaCopy.name(),
                        L"The name is not correct.");

         Assert::AreEqual(qgl::QGL_VERSION_LATEST,
                          metaCopy.version(),
                          L"The version is not correct.");

         Assert::IsTrue(metaCopy.content_visible(),
                        L"The content should be visible.");

         Assert::IsFalse(metaCopy.obey_physics(),
                         L"The content should not obey physics.");
      }

      TEST_METHOD(EqualityOperator)
      {
         CONTENT_METADATA_BUFFER m1(RESOURCE_TYPES::RESOURCE_TYPE_BRUSH,
                                    CONTENT_LOADER_IDS::CONTENT_LOADER_ID_BRUSH,
                                    L"Name");

         CONTENT_METADATA_BUFFER m2 = m1;

         Assert::IsTrue(m1 == m2,
                        L"The metadatas should be equal.");

         CONTENT_METADATA_BUFFER m3(RESOURCE_TYPES::RESOURCE_TYPE_BRUSH,
                                    CONTENT_LOADER_IDS::CONTENT_LOADER_ID_BRUSH,
                                    L"Name2");

         Assert::IsFalse(m1 == m3,
                         L"The metadatas should not be equal.");
      }
   };
}