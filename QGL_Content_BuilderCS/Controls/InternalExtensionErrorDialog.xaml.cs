using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices.WindowsRuntime;
using Windows.Foundation;
using Windows.Foundation.Collections;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Controls.Primitives;
using Windows.UI.Xaml.Data;
using Windows.UI.Xaml.Input;
using Windows.UI.Xaml.Media;
using Windows.UI.Xaml.Navigation;

namespace QGL_Content_Builder.Controls
{
   public sealed partial class InternalExtensionErrorDialog : ContentDialog
   {
      public InternalExtensionErrorDialog(string extensionName)
      {
         Message = string.Format("There is a problem with the {0} extension." +
            "The operation has not been completed. Try reinstalling the extension.", extensionName);
         this.InitializeComponent();
      }

      public string Message { get; private set; }
   }
}
