using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace QGL_Content_Builder.PageModels
{
   public class EditorPage : INotifyPropertyChanged
   {
      public EditorPage(Extensions.ExtensionProvider extensionProvider)
      {
         Project = new QGL_Projection.ContentProject();
         IsProjectSaving = false;
         IsProjectLoading = false;
         IsDictionarySelectionMode = false;
         AvailableExtensions = extensionProvider;
         Project.ContentInfo.ResourceType = AvailableExtensions.ResourceTypes[0];
         Project.ContentInfo.ContentLoader = Project.ContentInfo.ResourceType.Loaders[0];
      }

      public Extensions.ExtensionProvider AvailableExtensions
      {
         get { return m_extensions; }
         private set
         {
            if(m_extensions != value)
            {
               m_extensions = value;
               PropertyChanged(this, new PropertyChangedEventArgs("AvailableExtensions"));
            }
         }
      }

      public QGL_Projection.ContentProject Project
      {
         get { return m_project; }
         set
         {
            if(m_project != value)
            {
               m_project = value;
               PropertyChanged(this, new PropertyChangedEventArgs("Project"));
            }
         }
      }

      public bool IsProjectLoading
      {
         get { return m_isProjectLoading; }
         set
         {
            if(m_isProjectLoading != value)
            {
               m_isProjectLoading = value;
               PropertyChanged(this, new PropertyChangedEventArgs("IsProjectLoading"));
            }
         }
      }

      public bool IsProjectSaving
      {
         get { return m_isProjectSaving; }
         set
         {
            if(m_isProjectSaving != value)
            {
               m_isProjectSaving = value;
               PropertyChanged(this, new PropertyChangedEventArgs("IsProjectSaving"));
            }
         }
      }

      public bool IsDictionarySelectionMode
      {
         get { return m_isDictionaryEntrySelectionMode; }
         set
         {
            if(m_isDictionaryEntrySelectionMode != value)
            {
               m_isDictionaryEntrySelectionMode = value;
               PropertyChanged(this, new PropertyChangedEventArgs("IsDictionarySelectionMode"));
            }
         }
      }

      public QGL_Projection.ContentProjectEntry SelectedProjectEntry
      {
         get { return m_selectedProjectEntry; }
         set
         {
            if(m_selectedProjectEntry != value)
            {
               m_selectedProjectEntry = value;
               PropertyChanged(this, new PropertyChangedEventArgs("SelectedProjectEntry"));
               PropertyChanged(this, new PropertyChangedEventArgs("IsEntrySelected"));
            }
         }
      }

      public bool IsEntrySelected
      {
         get { return SelectedProjectEntry != null; }
      }

      public async Task<bool> LoadProjectAsync(string filePath)
      {
         m_isProjectLoading = true;
         var file = await Helpers.FileHelpers.GetFutureFileAsync(filePath);
         var success = await Project.LoadFromFileAsync(file, AvailableExtensions.ResourceTypes);
         IsProjectLoading = false;
         PropertyChanged(this, new PropertyChangedEventArgs("Project"));
         return success;
      }

      public async Task<bool> SaveProjectAsync(string filePath)
      {
         IsProjectSaving = true;
         var file = await Helpers.FileHelpers.GetFutureFileAsync(filePath);
         var ret = await Project.SaveToFileAsync(file);
         IsProjectSaving = false;
         return ret;
      }

      private bool m_isProjectLoading;
      private bool m_isProjectSaving;
      private bool m_isDictionaryEntrySelectionMode;
      private QGL_Projection.ContentProject m_project;
      private Extensions.ExtensionProvider m_extensions;
      private QGL_Projection.ContentProjectEntry m_selectedProjectEntry;

      public event PropertyChangedEventHandler PropertyChanged = delegate { };
   }
}
