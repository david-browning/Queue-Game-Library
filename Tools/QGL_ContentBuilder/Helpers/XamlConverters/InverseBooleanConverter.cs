using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace QGL_ContentBuilder.Helpers.XamlConverters
{
   public class InverseBooleanConverter : Windows.UI.Xaml.Data.IValueConverter
   {
      public object Convert(object value,
          Type targetType,
          object parameter,
          string language)
      {
         return !(bool)value;
      }

      public object ConvertBack(object value,
          Type targetType,
          object parameter,
          string language)
      {
         return !(bool)value;
      }
   }
}
