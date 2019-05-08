using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices.WindowsRuntime;
using System.Threading.Tasks;
using Windows.Foundation;
using Windows.Foundation.Collections;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Controls.Primitives;
using Windows.UI.Xaml.Data;
using Windows.UI.Xaml.Input;
using Windows.UI.Xaml.Media;
using Windows.UI.Xaml.Navigation;

namespace QGL_Content_Builder.Pages
{
   public sealed partial class EditorPage : Page
   {
      public EditorPage()
      {
         ViewModel = new PageModels.EditorPage(new Extensions.ExtensionProvider());
         this.InitializeComponent();
      }

      private PageModels.EditorPage ViewModel { get; set; }

      private async Task loadProject(string path)
      {
         ViewModel.IsProjectLoading = true;
         var loaded = await ViewModel.LoadProjectAsync(path);
         if(!loaded)
         {
            var dialogBox = new Controls.CannotOpenProjectDialog();
            var result = await dialogBox.ShowAsync();
            if(result == ContentDialogResult.None || result == ContentDialogResult.Secondary)
            {
               //User chose not to open the file.
               if(Frame.CanGoBack)
               {
                  Frame.GoBack();
               }
            }
            else
            {
               throw new NotImplementedException();
            }
         }

         ViewModel.IsProjectLoading = false;
      }

      private async Task saveProject(string path)
      {
         ViewModel.IsProjectSaving = true;
         var saved = await ViewModel.SaveProjectAsync(path);
         if(!saved)
         {
            var dialogBox = new Controls.CannotSaveProjectDialog();
            await dialogBox.ShowAsync();
         }

         ViewModel.IsProjectSaving = false;
      }

      private void addNewEntry()
      {
         var newEntry = new QGL_Projection.ContentProjectEntry("Select a File",
                                                               new QGL_Projection.ContentInfo());
         newEntry.ContentInfo.ResourceType = ViewModel.AvailableExtensions.ResourceTypes[0];
         newEntry.ContentInfo.ContentLoader = newEntry.ContentInfo.ResourceType.Loaders[0];

         ViewModel.Project.ObservableEntries.Add(newEntry);
      }

      private void removeEntryAt(int idx)
      {
         ViewModel.Project.ObservableEntries.RemoveAt(idx);
      }

      private async Task saveDictionaryEntry(QGL_Projection.ContentProjectEntry entry)
      {
         var file = await Helpers.FileHelpers.GetFutureFileAsync(entry.FilePath);
         int x = 0;
         if(file != null)
         {
            var extension = ViewModel.AvailableExtensions.GetExtension(entry.ContentInfo);
            if(extension != null)
            {
               try
               {
                  await extension.Extension.Save(file);
               }
               catch(Exception)
               {
                  Controls.InternalExtensionErrorDialog dialog = 
                     new Controls.InternalExtensionErrorDialog(extension.Extension.ContentLoaderName);
                  await dialog.ShowAsync();
               }
            }
            else
            {
               Controls.ExtensionNotInstalledDialog dialog = new Controls.ExtensionNotInstalledDialog();
               await dialog.ShowAsync();
            }
         }
         else
         {
            Controls.NoFileSetDialog dialog = new Controls.NoFileSetDialog();
            await dialog.ShowAsync();
         }
      }

      private async Task newDictionaryEntryFile(QGL_Projection.ContentInfo info)
      {
         var extension = ViewModel.AvailableExtensions.GetLoaderExtension(info);
         var file = await Helpers.FileHelpers.PickNewDictionaryEntryFileAsync(info, extension);
         if(file != null)
         {
            Helpers.FileHelpers.AddToFutureAccessList(file);
            await ViewModel.SelectedProjectEntry.ChangeFile(file);
         }
      }

      private async Task browseDictionaryEntryFile()
      {
         var file = await Helpers.FileHelpers.PickDictionaryEntryFileAsync();
         if(file != null)
         {
            Helpers.FileHelpers.AddToFutureAccessList(file);
            await ViewModel.SelectedProjectEntry.ChangeFile(file);
         }
      }

      void messageBoxNoFile()
      {
         Controls.NoFileSetDialog dialog = new Controls.NoFileSetDialog();
         dialog.ShowAsync();
      }

      private void Page_Loaded(object sender, RoutedEventArgs e)
      {
         if(Helpers.AppDataHelpers.IsProjectPathSet())
         {
            loadProject(Helpers.AppDataHelpers.GetProjectPath());
         }
         else
         {
            messageBoxNoFile();
            if(Frame.CanGoBack)
            {
               Frame.GoBack();
            }
         }
      }

      protected override async void OnNavigatingFrom(NavigatingCancelEventArgs e)
      {

         base.OnNavigatingFrom(e);
      }

      private void OnSaveProjectButtonClicked(object sender, RoutedEventArgs e)
      {
         if(Helpers.AppDataHelpers.IsProjectPathSet())
         {
            saveProject(Helpers.AppDataHelpers.GetProjectPath());
         }
         else
         {
            messageBoxNoFile();
         }
      }

      private void OnAddDictionaryEntryButtonClicked(object sender, RoutedEventArgs e)
      {
         addNewEntry();
      }

      private void OnSaveDictionaryEntryButtonClick(object sender, RoutedEventArgs e)
      {
         var entry = ViewModel.SelectedProjectEntry;
         if(entry != null)
         {
            saveDictionaryEntry(entry);
         }
      }

      private void OnDeleteDictionaryEntryButtonClick(object sender, RoutedEventArgs e)
      {
         var idx = m_dictionaryEntryListView.SelectedIndex;
         if(idx != -1)
         {
            removeEntryAt(idx);
            if(m_deleteEntryConfirmFlyout != null)
            {
               m_deleteEntryConfirmFlyout.Hide();
            }
         }
      }

      private void OnBrowseDictionaryEntryFileButtonClick(object sender, RoutedEventArgs e)
      {
         browseDictionaryEntryFile();
      }

      private void OnNewDictionaryEntryFileButtonClick(object sender, RoutedEventArgs e)
      {
         var info = ViewModel.SelectedProjectEntry.ContentInfo;
         newDictionaryEntryFile(info);
      }

      private void OnDeleteMultipleEntriesButtonClick(object sender, RoutedEventArgs e)
      {

      }

      private void ResourceTypeBoxSelectionChanged(object sender, SelectionChangedEventArgs e)
      {
         ViewModel.Project.ContentInfo.ContentLoader = ViewModel.Project.ContentInfo.ResourceType.Loaders[0];
      }

      private void DictionaryResourcetypeBoxSelectionChanged(object sender, SelectionChangedEventArgs e)
      {
         ViewModel.SelectedProjectEntry.ContentInfo.ContentLoader = ViewModel.SelectedProjectEntry.ContentInfo.ResourceType.Loaders[0];
      }
   }
}
