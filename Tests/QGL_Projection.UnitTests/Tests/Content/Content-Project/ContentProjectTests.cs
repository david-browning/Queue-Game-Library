using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using QGL_Projection;
using Windows.UI.Core;
using Windows.Storage;

namespace QGL_Projection.UnitTests.Tests.Content.Content_Project
{
    [TestClass]
    public class ContentProjectTests
    {
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

        private Windows.UI.Core.CoreDispatcher dispatcher =
            Windows.ApplicationModel.Core.CoreApplication.MainView.CoreWindow.Dispatcher;
    }
}