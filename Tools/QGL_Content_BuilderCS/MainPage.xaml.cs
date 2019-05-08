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
using QGL_Content_Builder.Helpers;
using System.ComponentModel;

// The Blank Page item template is documented at https://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x409

namespace QGL_Content_Builder
{
   /// <summary>
   /// An empty page that can be used on its own or navigated to within a Frame.
   /// </summary>
   public sealed partial class MainPage : Page, INotifyPropertyChanged
   {
      public MainPage()
      {
         //Do not auto-navigate or set a project path.
         AppDataHelpers.ClearAutoNavigatePage();
         AppDataHelpers.ClearProjectPath();
         this.InitializeComponent();

         //Navigate to the home page and set the selected navigation item
         navigate("QGL_Content_Builder.Pages.HomePage");
         m_navView.SelectedItem = m_navView.MenuItems[0];
      }
      
      private void navigate(string pageName)
      {
         var t = Type.GetType(pageName);
         m_contentFrame.Navigate(t);
      }

      private void M_navView_ItemInvoked(NavigationView sender, NavigationViewItemInvokedEventArgs args)
      {
         string navTo;
         if(args.IsSettingsInvoked)
         {
            navTo = "QGL_Content_Builder.Pages.SettingsPage";
         }
         else
         {
            var tag = (string)args.InvokedItemContainer.Tag;
            navTo = "QGL_Content_Builder.Pages." + tag;
         }


         navigate(navTo);
      }

      private void M_contentFrame_Navigating(object sender, NavigatingCancelEventArgs e)
      {
         m_navView.IsBackEnabled = m_contentFrame.CanGoBack;
         var lastSelectedItem = (NavigationViewItem)m_navView.SelectedItem;

         if(AppDataHelpers.IsAutoNavigatePageSet())
         {
            if(AppDataHelpers.GetAutoNavigatePage() == "EditorPage")
            {
               m_navView.SelectedItem = m_navView.MenuItems[1];
               AppDataHelpers.ClearAutoNavigatePage();
            }
            //Else, check other auto navigate pages.
         }
      }

      public bool EnableProjectEditorNavigation
      {
         get { return AppDataHelpers.IsProjectPathSet(); }
      }

      public event PropertyChangedEventHandler PropertyChanged = delegate { };
   }
}
