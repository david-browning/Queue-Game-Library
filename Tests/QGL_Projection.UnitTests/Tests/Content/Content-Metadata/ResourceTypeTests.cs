using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using QGL_Projection;

namespace QGL_Projection.UnitTests.Tests.Content.Content_Metadata
{
    [TestClass]
    public class ResourceTypeTests
    {
        [TestMethod]
        public void Construct()
        {
            ResourceType r = new ResourceType(1, "Some Name");
            Assert.AreEqual("Some Name", r.ToString(),
                "The name is not correct.");

            Assert.AreEqual(1, r.Value,
                "The resource type value is not correct.");
        }
    }
}
