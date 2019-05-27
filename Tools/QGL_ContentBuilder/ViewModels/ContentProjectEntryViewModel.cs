using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using QGL_Projection;
using Windows.UI.Xaml.Data;

namespace QGL_ContentBuilder.ViewModels
{
   class ContentProjectEntryViewModel : INotifyPropertyChanged
   {
      public ContentProjectEntryViewModel(ContentProjectEntry entry)
      {
         ProjectEntry = entry;
         IsChecked = false;
      }

      public ContentProjectEntry ProjectEntry
      {
         get { return _entry; }
         set
         {
            if (_entry != value)
            {
               _entry = value;
               PropertyChanged(this,
                               new PropertyChangedEventArgs("ProjectEntry"));
            }
         }
      }

      public bool IsChecked
      {
         get { return _isChecked; }
         set
         {
            if(_isChecked != value)
            {
               _isChecked = value;
               PropertyChanged(this,
                               new PropertyChangedEventArgs("IsChecked"));
            }
         }
      }

      public event PropertyChangedEventHandler PropertyChanged = delegate { };

      private bool _isChecked;
      private ContentProjectEntry _entry;
   }
}
