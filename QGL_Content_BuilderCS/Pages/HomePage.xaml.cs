using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices.WindowsRuntime;
using Windows.Foundation;
using Windows.Foundation.Collections;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using System.Threading.Tasks;
using Windows.UI.Xaml.Controls.Primitives;
using Windows.UI.Xaml.Data;
using Windows.UI.Xaml.Input;
using Windows.UI.Xaml.Media;
using Windows.UI.Xaml.Navigation;

namespace QGL_Content_Builder.Pages
{
   public sealed partial class HomePage : Page
   {
      public HomePage()
      {
         ViewModel = new PageModels.HomePageVM(new Extensions.ExtensionProvider());
         this.InitializeComponent();
      }

      private void ClearRecentButtonClicked(object sender, RoutedEventArgs e)
      {
         ViewModel.Clear();
      }

      private void OpenButtonClicked(object sender, RoutedEventArgs e)
      {
         open_project();
      }

      private void NewButtonClicked(object sender, RoutedEventArgs e)
      {
         new_project();
      }

      private void RecentSelectionChanged(object sender, SelectionChangedEventArgs e)
      {
         var listView = (ListView)sender;
         var item = (PageModels.RecentProjectModel)listView.SelectedItem;
         selection_changed(item.Path);
      }

      protected override void OnNavigatedTo(NavigationEventArgs e)
      {
         populate();
         base.OnNavigatedTo(e);
      }

      private async Task new_project()
      {
         var file = await Helpers.FileHelpers.GetProjectUsingSavePickerAsync();
         if(file != null)
         {
            Helpers.FileHelpers.AddToRecentAccessList(file);
            Helpers.FileHelpers.AddToFutureAccessList(file);

            Helpers.AppDataHelpers.SetAutoNavigatePage("EditorPage");
            Helpers.AppDataHelpers.SetProjectPath(file.Path);

            Frame.Navigate(typeof(Pages.EditorPage));
         }
      }

      private async Task open_project()
      {
         var file = await Helpers.FileHelpers.GetProjectUsingOpenPickerAsync();
         if(file != null)
         {
            Helpers.FileHelpers.AddToRecentAccessList(file);
            Helpers.FileHelpers.AddToFutureAccessList(file);

            Helpers.AppDataHelpers.SetAutoNavigatePage("EditorPage");
            Helpers.AppDataHelpers.SetProjectPath(file.Path);

            Frame.Navigate(typeof(Pages.EditorPage));
         }
      }

      private async Task selection_changed(string filePath)
      {
         var file = await Helpers.FileHelpers.GetRecentFileAsync(filePath);
         if(file != null)
         {
            Helpers.FileHelpers.AddToRecentAccessList(file);
            Helpers.FileHelpers.AddToFutureAccessList(file);

            Helpers.AppDataHelpers.SetAutoNavigatePage("EditorPage");
            Helpers.AppDataHelpers.SetProjectPath(file.Path);

            Frame.Navigate(typeof(Pages.EditorPage));
         }
      }

      private async Task populate()
      {
         ViewModel.RecentFilesPopulating = true;
         await ViewModel.PopulateAsync();
         ViewModel.RecentFilesPopulating = false;
      }

      private QGL_Content_Builder.PageModels.HomePageVM ViewModel { get; set; }

   }
}
