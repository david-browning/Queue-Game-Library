using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using QGL_Projection;

namespace QGL_ContentBuilder.ViewModels
{
   class RecentProjectViewModel
   {
      public RecentProjectViewModel(string path,
                                    ContentMetadata meta)
      {
         GUID = meta.Guid().ToString();
         Path = path;
         ContentName = meta.ContentName;
      }

      public string ContentName { get; private set; }

      public string Path { get; private set; }

      public string GUID { get; private set; }
   }
}
