using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using QGL_Projection;

namespace QGL_ContentBuilder.Providers
{
   class ContentExtensionProvider
   {
      /// <summary>
      /// Given a resource type, this searches the available content extensions
      /// and returns a list of all content loaders that are supported by the
      /// given resource type.
      /// </summary>
      /// <param name="r"></param>
      /// <returns>List of content loaders. The list will be empty if there
      /// are no content loaders supported by the resource type.</returns>
      public IList<ContentLoader> GetSupportedContentLoaders(ResourceType r)
      {
         throw new NotImplementedException();
      }

      /// <summary>
      /// Searches the available content extensions to find a content loader 
      /// with the given ID.
      /// </summary>
      /// <param name="loaderID"></param>
      /// <returns>A content loader with the given loaderID. Null if no content
      /// loader with the given ID exists.</returns>
      public ContentLoader LoaderFromID(UInt16 loaderID)
      {
         throw new NotImplementedException();
      }

      /// <summary>
      /// Searches the available content extensions to find a resource type
      /// with the given ID.
      /// </summary>
      /// <param name="resourceType"></param>
      /// <returns>A resource type with the given resourceType. Null if no
      /// content loader with the given ID exists.</returns>
      public ResourceType ResourceTypeFromID(UInt16 resourceType)
      {
         throw new NotImplementedException();
      }

      public void AddContentExtension(IContentExtension extension)
      {
         throw new NotImplementedException();
      }

      public void RemoveContentExtension(IContentExtension extension)
      {
         throw new NotImplementedException();
      }

      private List<IContentExtension> _contentExtensions;
   }
}
