using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace QGL_Content_Builder.PageModels
{
   public class HomePageVM : INotifyPropertyChanged
   {
      public HomePageVM(Extensions.ExtensionProvider provider)
      {
         m_provider = provider;
         RecentFiles = new ObservableCollection<RecentProjectModel>();
         RecentFilesPopulating = false;
      }

      public async Task PopulateAsync()
      {
         RecentFilesPopulating = true;
         RecentFiles = await Helpers.FileHelpers.GetMostRecentlyUsedListAsync(m_provider.ResourceTypes);
         RecentFilesPopulating = false;
      }

      public ObservableCollection<RecentProjectModel> RecentFiles
      {
         get
         {
            return m_recentFiles;
         }
         private set
         {
            if(m_recentFiles != value)
            {
               m_recentFiles = value;
               PropertyChanged(this, new PropertyChangedEventArgs("RecentFiles"));
            }
         }
      }

      public bool RecentFilesPopulating
      {
         get
         {
            return m_recentPopulating;
         }
         set
         {
            if(m_recentPopulating != value)
            {
               m_recentPopulating = value;
               PropertyChanged(this, new PropertyChangedEventArgs("RecentFilesPopulating"));
            }
         }
      }

      public void Clear()
      {
         Helpers.FileHelpers.ClearRecentFiles();
         RecentFiles.Clear();
      }

      public event PropertyChangedEventHandler PropertyChanged = delegate { };

      public void OnPropertyChanged(string propertyName)
      {
         this.PropertyChanged(this, new PropertyChangedEventArgs(propertyName));
      }

      private ObservableCollection<RecentProjectModel> m_recentFiles;
      private bool m_recentPopulating;
      private Extensions.ExtensionProvider m_provider;
   }
}
