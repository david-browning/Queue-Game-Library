using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace QGL_Content_Builder.PageModels
{
    public class ContentEntryVM : INotifyPropertyChanged
    {
      public ContentEntryVM(QGL_Projection.ContentProjectEntry entry)
      {
         ProjectEntry = entry;
         IsChecked = false;
      }

      public QGL_Projection.ContentProjectEntry ProjectEntry
      {
         get { return m_projectEntry; }
         set
         {
            if(m_projectEntry != value)
            {
               m_projectEntry = value;
               PropertyChanged(this, new PropertyChangedEventArgs("ProjectEntry"));
            }
         }
      }

      public bool IsChecked
      {
         get { return m_isChecked; }
         set
         {
            if(m_isChecked != value)
            {
               m_isChecked = value;
               PropertyChanged(this, new PropertyChangedEventArgs("IsChecked"));
            }
         }
      }

      private QGL_Projection.ContentProjectEntry m_projectEntry;
      private bool m_isChecked;

      public event PropertyChangedEventHandler PropertyChanged = delegate { };
   }
}
