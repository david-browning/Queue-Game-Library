using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace QGL_Content_Builder.Extensions
{
   public class ExtensionProvider : INotifyPropertyChanged
   {
      public ExtensionProvider()
      {
         ContentExtensions = new ObservableCollection<IContentExtensionUI>();
         ContentExtensions.Add(new UnknownCEUI());
         ContentExtensions.Add(new StringCEUI());
         ReloadResourceTypes();
      }

      public event PropertyChangedEventHandler PropertyChanged = delegate { };

      public void ReloadResourceTypes()
      {
         ResourceTypes = new ObservableCollection<QGL_Projection.ResourceType>();
         foreach(var extension in ContentExtensions)
         {
            var resourceType = ResourceTypes.SingleOrDefault(x => x.Value == extension.Extension.ResourceType);
            if(resourceType == null)
            {
               var resource = new QGL_Projection.ResourceType(extension.Extension.ResourceType,
                                                              extension.Extension.ResourceTypeName,
                                                              new List<QGL_Projection.ContentLoader>()
                                                              {
                                                                 new QGL_Projection.ContentLoader(extension.Extension.ContentLoaderID, 
                                                                                                  extension.Extension.ContentLoaderName)
                                                              }.ToArray());

               ResourceTypes.Add(resource);
            }
            else
            {
               var contentLoader = new QGL_Projection.ContentLoader(extension.Extension.ContentLoaderID, extension.Extension.ContentLoaderName);

               resourceType.Loaders.Add(contentLoader);
               resourceType.ObservableLoaders.Add(contentLoader);
            }
         }
      }

      public void AddExtension(IContentExtensionUI extension)
      {
         var existingExtension = ContentExtensions.SingleOrDefault(x => x.Extension.ResourceType == extension.Extension.ResourceType &&
                                                          x.Extension.ContentLoaderID == extension.Extension.ContentLoaderID);

         if(existingExtension == null)
         {
            ContentExtensions.Add(extension);
         }
      }

      /// <summary>
      ///
      /// </summary>
      /// <param name="info"></param>
      /// <returns>An IContentExtension that matches the content info. Null if no extension is available.</returns>
      public IContentExtensionUI GetExtension(QGL_Projection.ContentInfo info)
      {
         var existingExtension = ContentExtensions.SingleOrDefault(x => x.Extension.ResourceType == info.ResourceType.Value &&
                                                                   x.Extension.ContentLoaderID == info.ContentLoader.Value);

         return existingExtension;
      }

      /// <summary>
      /// Returns true if there is a content extension that supports info.
      /// </summary>
      /// <param name="info"></param>
      /// <returns></returns>
      public bool CheckSupport(QGL_Projection.ContentInfo info)
      {
         var extension = ContentExtensions.SingleOrDefault(x => x.Extension.ResourceType == info.ResourceType.Value &&
                                                           x.Extension.ContentLoaderID == info.ContentLoader.Value);

         return extension != null;
      }

      /// <summary>
      /// 
      /// </summary>
      /// <param name="info"></param>
      /// <returns>Returns a file extension to use for the content info. Returns ".unkn" if there is no
      /// available extension.</returns>
      public string GetLoaderExtension(QGL_Projection.ContentInfo info)
      {
         var extension = ContentExtensions.SingleOrDefault(x => x.Extension.ContentLoaderID == info.ContentLoader.Value);
         if(extension != null)
         {
            return extension.Extension.ContentExtension;
         }

         return ".unkn";
      }

      public ObservableCollection<QGL_Projection.ResourceType> ResourceTypes
      {
         get { return m_resourceTypes; }
         private set
         {
            if(m_resourceTypes != value)
            {
               m_resourceTypes = value;
               PropertyChanged(this, new PropertyChangedEventArgs("ResourceTypes"));
            }
         }
      }

      public ObservableCollection<IContentExtensionUI> ContentExtensions
      {
         get { return m_contentExtensions; }
         private set
         {
            if(m_contentExtensions != value)
            {
               m_contentExtensions = value;
               PropertyChanged(this, new PropertyChangedEventArgs("ContentExtensions"));
            }
         }
      }


      private ObservableCollection<QGL_Projection.ResourceType> m_resourceTypes;
      private ObservableCollection<IContentExtensionUI> m_contentExtensions;
   }
}
