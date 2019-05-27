using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices.WindowsRuntime;
using QGL_ContentBuilder.Helpers;
using Windows.Foundation;
using Windows.Foundation.Collections;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Controls.Primitives;
using Windows.UI.Xaml.Data;
using Windows.UI.Xaml.Input;
using Windows.UI.Xaml.Media;
using Windows.UI.Xaml.Navigation;

// The Blank Page item template is documented at 
//https://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x409

namespace QGL_ContentBuilder
{
    /// <summary>
    /// An empty page that can be used on its own or navigated to within a 
    /// Frame.
    /// </summary>
    public sealed partial class MainPage : Page, INotifyPropertyChanged
    {
        public MainPage()
        {
         //Do not auto-navigate or set a project path.
         NavigationHelpers.ClearAutoNavigatePage();
         NavigationHelpers.ClearProjectPath();
         this.InitializeComponent();

         //Navigate to the home page and set the selected navigation item
         Navigate("QGL_Content_Builder.Pages.HomePage");
         _navView.SelectedItem = _navView.MenuItems[0];
      }

      public bool EnableProjectEditorNavigation
      {
         get { return NavigationHelpers.IsProjectPathSet(); }
      }

      public event PropertyChangedEventHandler PropertyChanged = delegate { };

      private void Navigate(string pageName)
      {
         var t = Type.GetType(pageName);
         _contentFrame.Navigate(t);
      }

      private void _navView_ItemInvoked(NavigationView sender,
                                        NavigationViewItemInvokedEventArgs args)
      {
         string navTo;
         if(args.IsSettingsInvoked)
         {
            navTo = "QGL_ContentBuilder.Pages.SettingsPage";
         }
         else
         {
            var tag = (string)args.InvokedItemContainer.Tag;
            navTo = string.Format("QGL_ContentBuilder.Pages.{0}", tag);
         }

         Navigate(navTo);
      }

      private void _contentFrame_Navigating(object sender,
                                            NavigatingCancelEventArgs e)
      {
         _navView.IsBackEnabled = _contentFrame.CanGoBack;
         var lastSelectedItem = (NavigationViewItem)_navView.SelectedItem;

         if (NavigationHelpers.IsAutoNavigatePageSet())
         {
            if (NavigationHelpers.GetAutoNavigatePage() == "EditorPage")
            {
               _navView.SelectedItem = _navView.MenuItems[1];
               NavigationHelpers.ClearAutoNavigatePage();
            }
            //Else, check other auto navigate pages.
         }
      }
   }
}
