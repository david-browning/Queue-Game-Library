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
            var loaders = new ushort[]
            {
                0, 1, 2
            };

            ResourceType r = new ResourceType(1, "Some Name", loaders);
            Assert.AreEqual("Some Name", r.ToString(),
                "The name is not correct.");

            Assert.AreEqual(1, r.Value,
                "The resource type value is not correct.");

            var q = from a1 in loaders
                    join a2 in r.SupportedLoaderIDs on a1 equals a2
                    select a2;

            Assert.IsTrue(r.SupportedLoaderIDs.Length == loaders.Length &&
                q.Count() == loaders.Length,
                "The list is not correct.");
        }
    }
}
