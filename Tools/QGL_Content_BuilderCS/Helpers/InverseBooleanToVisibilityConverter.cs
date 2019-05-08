using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace QGL_Content_Builder.Helpers
{
   public class InverseBooleanToVisibilityConverter : Windows.UI.Xaml.Data.IValueConverter
   {
      public object Convert(object value, 
                           Type targetType, 
                           object parameter,
                           string language)
      {
         if((bool)value)
         {
            return Windows.UI.Xaml.Visibility.Collapsed;
         }
         else
         {
            return Windows.UI.Xaml.Visibility.Visible;
         }
      }

      public object ConvertBack(object value, 
                                Type targetType, 
                                object parameter, 
                                string language)
      {
         throw new NotImplementedException();
      }
   }
}
