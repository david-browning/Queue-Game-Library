using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using QGL_Projection;
using Windows.UI.Core;
using Windows.Storage;
using Windows.Foundation;

namespace QGL_Projection.UnitTests.Tests.Content.Content_Project
{
    [TestClass]
    public class ContentProjectTests
    {
        /*
         Test to make sure writing a project does not throw an exception.
         */
        [TestMethod]
        public async Task SaveProject()
        {
            var storageFolder = ApplicationData.Current.LocalFolder;

            var storageFile = await storageFolder.CreateFileAsync(
                "SaveProjectedContentProject.txt",
                CreationCollisionOption.ReplaceExisting);

            await dispatcher.RunAsync(CoreDispatcherPriority.Normal,
            () =>
            {
                QGL_Projection.ContentProject contentProject =
                    new ContentProject();

                contentProject.SaveProjectFile(storageFile);
            });
        }

        [TestMethod]
        public async Task SaveReadProject()
        {
            var storageFolder = ApplicationData.Current.LocalFolder;

            var storageFile = await storageFolder.CreateFileAsync(
                "SaveReadProjectedContentProject.txt",
                CreationCollisionOption.ReplaceExisting);

            await dispatcher.RunAsync(CoreDispatcherPriority.Normal,
            () =>
            {
                //Create a new project.
                QGL_Projection.ContentProject writeProject =
                    new ContentProject();

                ContentMetadata newProjectMeta = new ContentMetadata();
                newProjectMeta.ContentName = "New Project";
                newProjectMeta.CompilerVersion =
                    CompilerVersion.VersionLatest.Value;
                newProjectMeta.ContentLoader = 1;
                newProjectMeta.ResourceType = 2;
                writeProject.Metadata = newProjectMeta;

                ContentMetadata blueMetadata = new ContentMetadata();
                blueMetadata.ContentName = "Blue Float";
                ContentProjectEntry blueEntry = new ContentProjectEntry(
                    storageFolder.Path + "\\projectedBlue.txt",
                    blueMetadata);

                ContentMetadata redMetadata = new ContentMetadata();
                redMetadata.ContentName = "Red Float";
                ContentProjectEntry redEntry = new ContentProjectEntry(
                    storageFolder.Path + "\\projectedRed.txt",
                    redMetadata);

                ContentMetadata greenMetadata = new ContentMetadata();
                greenMetadata.ContentName = "Green Float";
                ContentProjectEntry greenEntry = new ContentProjectEntry(
                    storageFolder.Path + "\\projectedGreen.txt",
                    greenMetadata);

                writeProject.Entries.Add(redEntry);
                writeProject.Entries.Add(greenEntry);
                writeProject.Entries.Add(blueEntry);

                writeProject.SaveProjectFile(storageFile);

                ContentProject readProject = new ContentProject();
                readProject.LoadFromFileAsync(storageFile);

                Assert.IsTrue(newProjectMeta.Equals(readProject.Metadata),
                    "The metadata is not correct.");

                for (int i = 0; i < writeProject.Entries.Count; i++)
                {
                    var eEntry = (ContentProjectEntry)writeProject.Entries[i];
                    var aEntry = (ContentProjectEntry)readProject.Entries[i];

                    Assert.AreEqual(eEntry.FilePath,
                        aEntry.FilePath,
                        "The paths are not equal.");

                    Assert.IsTrue(eEntry.Metadata.Equals(
                        aEntry.Metadata),
                        "The meta data entries are not equal.");
                }
            });
        }

        private Windows.UI.Core.CoreDispatcher dispatcher =
            Windows.ApplicationModel.Core.CoreApplication.MainView.CoreWindow.Dispatcher;
    }
}