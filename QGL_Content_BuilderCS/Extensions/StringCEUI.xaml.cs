using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices.WindowsRuntime;
using QGL_Projection;
using Windows.Foundation;
using Windows.Foundation.Collections;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Controls.Primitives;
using Windows.UI.Xaml.Data;
using Windows.UI.Xaml.Input;
using Windows.UI.Xaml.Media;
using Windows.UI.Xaml.Navigation;

namespace QGL_Content_Builder.Extensions
{
   public sealed partial class StringCEUI : IContentExtensionUI
   {
      public StringCEUI()
      {
         m_extension = new StringContentExtension();
         this.InitializeComponent();
      }

      public override IContentExtension Extension
      {
         get { return m_extension; }
      }

      private StringContentExtension m_extension;
   }
}
