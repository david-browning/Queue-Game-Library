using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Windows.Foundation;
using Windows.Storage;

namespace QGL_Content_Builder.Extensions
{
   public class UnknownContentExtension : QGL_Projection.IContentExtension
   {
      public IAsyncAction Open(StorageFile f)
      {
         throw new NotImplementedException();
      }

      public IAsyncAction Save(StorageFile f)
      {
         throw new NotImplementedException();
      }

      public string ContentExtension
      {
         get { return ".unkn"; }
      }

      public ushort ContentLoaderID
      {
         get { return 0; }
      }

      public string ContentLoaderName
      {
         get { return "Unknown"; }
      }

      public ushort ResourceType
      {
         get { return 0; }
      }

      public string ResourceTypeName
      {
         get { return "Unknown"; }
      }
   }
}
