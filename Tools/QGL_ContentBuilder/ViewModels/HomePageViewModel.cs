using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace QGL_ContentBuilder.ViewModels
{
   class HomePageViewModel : INotifyPropertyChanged
   {
      public HomePageViewModel()
      {
         throw new NotImplementedException();
      }

      public async Task PopulateRecentFilesAsync()
      {
         throw new NotImplementedException();
      }

      public ObservableCollection<RecentProjectViewModel> RecentFiles
      {
         get;
         set;
      }

      public bool RecentFilesPopulating
      {
         get;
         set;
      }

      public void ClearRecentFiles()
      {
         throw new NotImplementedException();
      }

      public event PropertyChangedEventHandler PropertyChanged = delegate { };
   }
}
