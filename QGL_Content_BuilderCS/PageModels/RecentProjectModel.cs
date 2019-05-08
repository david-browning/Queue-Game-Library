using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace QGL_Content_Builder.PageModels
{
   public class RecentProjectModel
   {
      public RecentProjectModel(string path,
                                QGL_Projection.ContentInfo info)
      {
         GUID = info.Guid().ToString();
         ContentName = info.ContentName;
         Path = path;
      }

      public string ContentName { get; private set; }

      public string Path { get; private set; }

      public string GUID { get; private set; }
   }
}
