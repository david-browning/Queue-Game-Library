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

// The Blank Page item template is documented at https://go.microsoft.com/fwlink/?LinkId=234238

namespace QGL_Content_Builder.Extensions
{
   /// <summary>
   /// An empty page that can be used on its own or navigated to within a Frame.
   /// </summary>
   public sealed partial class UnknownCEUI : IContentExtensionUI
   {
      public UnknownCEUI()
      {
         m_ce = new UnknownContentExtension();
         this.InitializeComponent();
      }

      public override IContentExtension Extension
      {
         get { return m_ce; }
      }

      private UnknownContentExtension m_ce;
   }
}
