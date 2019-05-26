using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using QGL_Projection;
using Windows.UI.Core;

namespace QGL_Projection.UnitTests.Tests.Content.Content_Project
{
    [TestClass]
    public class ContentProjectEntryTests
    {
        [TestMethod]
        public void Construct()
        {
            var metadata = new ContentMetadata();
            ContentProjectEntry e1 = new ContentProjectEntry("C:\\Test.txt",
                metadata);

            Assert.AreEqual(metadata, e1.Metadata,
                "The metadata is not correct.");

            Assert.AreEqual("C:\\Test.txt", e1.FilePath,
                "The file path is not correct.");
        }

        [TestMethod]
        public async Task GetSetFilePath()
        {
            await dispatcher.RunAsync(CoreDispatcherPriority.Normal,
            () =>
            {
                ContentProjectEntry e1 = new ContentProjectEntry("C:\\Test.txt",
                        new ContentMetadata());

                Assert.AreEqual("C:\\Test.txt", e1.FilePath,
                    "The file path is not correct.");

                var newPath = "D:\\TestFile.txt";
                e1.FilePath = newPath;

                Assert.AreEqual(newPath, e1.FilePath);
            });
        }

        [TestMethod]
        public async Task GetSetMetadata()
        {
            await dispatcher.RunAsync(CoreDispatcherPriority.Normal,
            () =>
            {
                ContentProjectEntry e1 = new ContentProjectEntry("C:\\Test.txt",
                        new ContentMetadata());

                e1.Metadata.ResourceType = 1;
                Assert.AreEqual(1, e1.Metadata.ResourceType,
                    "The resource type is not correct.");

                var newMetadata = new ContentMetadata();
                newMetadata.ContentName = "New Content Name";

                e1.Metadata = newMetadata;
                Assert.AreEqual(newMetadata, e1.Metadata,
                    "The metadata is not correct.");
            });
        }

        [TestMethod]
        public async Task ToStringTest()
        {
            await dispatcher.RunAsync(CoreDispatcherPriority.Normal,
            () =>
            {
                var metadata = new ContentMetadata();
                metadata.ContentName = "Content Name";
                ContentProjectEntry e1 = new ContentProjectEntry("C:\\Test.txt",
                    metadata);

                Assert.AreEqual("Content Name", e1.ToString(),
                    "The ToString() is not correct.");
            });
        }

        private Windows.UI.Core.CoreDispatcher dispatcher =
          Windows.ApplicationModel.Core.CoreApplication.MainView.CoreWindow.Dispatcher;
    }
}
