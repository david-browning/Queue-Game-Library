using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Windows.UI.Xaml.Controls;

namespace QGL_Content_Builder.Extensions
{
   public class IContentExtensionUI : Page
   {
      public IContentExtensionUI()
      {
      }

      public virtual QGL_Projection.IContentExtension Extension { get; }
   }
}
