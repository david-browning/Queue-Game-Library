using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using Windows.UI.Core;
using QGL_Projection;

namespace QGL_Projection.UnitTests.Tests.Content.Content_Metadata
{
    [TestClass]
    public class ContentMetadataTests
    {
        [TestMethod]
        public void ConstructDefault()
        {
            ContentMetadata metadata = new ContentMetadata();
            Assert.IsTrue(metadata.ContentObeyPhysics == false,
                "Metadata should not obey physics.");

            Assert.IsTrue(metadata.ContentVisible == true,
                "The content should be visible.");

            Assert.AreEqual("", metadata.ContentName,
                "The name should be empty.");

            Assert.AreEqual(0, metadata.ContentLoader,
               "The content loader should be 0.");

            Assert.AreEqual(0, metadata.ResourceType,
                "The resource type should be 0.");

            Assert.AreEqual(CompilerVersion.VersionLatest.Value,
                metadata.CompilerVersion,
                "The compiler version is not the latest.");

            ContentMetadata metadata2 = new ContentMetadata();
            Assert.AreNotEqual(metadata.Guid(), metadata2.Guid(),
                "The GUIDs should not be equal.");
        }

        [TestMethod]
        public void Copy()
        {
            dispatcher.RunAsync(CoreDispatcherPriority.Normal,
                () =>
                {
                    ContentMetadata metadata = new ContentMetadata();
                    metadata.ContentName = "New Name";
                    var metadata2 = metadata;

                    Assert.AreEqual(metadata, metadata2,
                        "The metadatas are not the same.");
                });
        }

        [TestMethod]
        public void VisibleGetSet()
        {
            dispatcher.RunAsync(CoreDispatcherPriority.Normal,
            () =>
            {
                ContentMetadata m = new ContentMetadata();
                m.ContentVisible = true;
                Assert.IsTrue(m.ContentVisible,
                    "Content should be visible.");

                m.ContentVisible = false;
                Assert.IsFalse(m.ContentVisible,
                    "The content should not be visible.");

            });
        }

        [TestMethod]
        public void ObeyPhysicsGetSet()
        {
            dispatcher.RunAsync(CoreDispatcherPriority.Normal,
            () =>
            {
                ContentMetadata m = new ContentMetadata();
                m.ContentObeyPhysics = true;
                Assert.IsTrue(m.ContentObeyPhysics,
                    "The content should obey physics.");

                m.ContentObeyPhysics = false;
                Assert.IsFalse(m.ContentObeyPhysics,
                    "Content should not obey physics.");
            });
        }

        [TestMethod]
        public void NameGetSet()
        {
            dispatcher.RunAsync(CoreDispatcherPriority.Normal,
            () =>
            {
                ContentMetadata m = new ContentMetadata();
                m.ContentName = "Test Name";
                Assert.AreEqual("Test Name", m.ContentName,
                    "The names are not equal");
            });
        }

        [TestMethod]
        public void LoaderIDGetSet()
        {
            dispatcher.RunAsync(CoreDispatcherPriority.Normal,
            () =>
            {
                ContentMetadata m = new ContentMetadata();
                m.ContentLoader = 65535;
                Assert.AreEqual(65535, m.ContentLoader,
                    "The loader ID is not correct.");
            });
        }

        [TestMethod]
        public void ResourceTypeGetSet()
        {
            dispatcher.RunAsync(CoreDispatcherPriority.Normal,
            () =>
            {
                ContentMetadata m = new ContentMetadata();
                m.ResourceType = 65534;
                Assert.AreEqual(65534, m.ResourceType,
                    "The resource type is not correct.");
            });
        }

        [TestMethod]
        public void VersionGetSet()
        {
            dispatcher.RunAsync(CoreDispatcherPriority.Normal,
            () =>
            {
                ContentMetadata m = new ContentMetadata();
                m.CompilerVersion = CompilerVersion.Version_0_1.Value;
                Assert.AreEqual(CompilerVersion.Version_0_1.Value,
                    m.CompilerVersion,
                    "The version is not correct.");
            });
        }

        private Windows.UI.Core.CoreDispatcher dispatcher =
            Windows.ApplicationModel.Core.CoreApplication.MainView.CoreWindow.Dispatcher;
    }
}
