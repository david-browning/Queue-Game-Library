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
    public class ContentLoaderTests
    {
        [TestMethod]
        public void Construct()
        {
            ContentLoader loader = new ContentLoader(1, "String");
            Assert.AreEqual(1, loader.Value,
                "The value is not correct.");

            Assert.AreEqual("String", loader.ToString(),
                "The name is not correct.");
        }
    }
}
