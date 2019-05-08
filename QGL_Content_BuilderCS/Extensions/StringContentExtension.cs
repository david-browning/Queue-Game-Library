using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Windows.Foundation;
using Windows.Storage;

namespace QGL_Content_Builder.Extensions
{
   public class StringContentExtension : QGL_Projection.IContentExtension
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
         get { return ".txt"; }
      }

      public ushort ContentLoaderID
      {
         get { return 1; }
      }

      public string ContentLoaderName
      {
         get { return "String"; }
      }

      public ushort ResourceType
      {
         get { return 1; }
      }

      public string ResourceTypeName
      {
         get { return "String"; }
      }
   }
}
