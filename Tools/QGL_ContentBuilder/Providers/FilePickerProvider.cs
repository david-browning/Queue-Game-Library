using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using QGL_Projection;
using Windows.Foundation;
using Windows.Storage;
using Windows.Storage.Pickers;

namespace QGL_ContentBuilder.Providers
{
   static class FilePickerProvider
   {
      public static IAsyncOperation<StorageFile> PickOpenProjectFileAsync()
      {
         FileOpenPicker pckr = new FileOpenPicker();
         pckr.SuggestedStartLocation = PickerLocationId.ComputerFolder;
         pckr.FileTypeFilter.Add(_qgl_project_extension);

         return pckr.PickSingleFileAsync();
      }

      public static IAsyncOperation<StorageFile> PickSaveProjectFileAsync()
      {
         FileSavePicker picker = new FileSavePicker();
         picker.SuggestedFileName = _qg_project_suggested_name;
         picker.SuggestedStartLocation = PickerLocationId.ComputerFolder;
         picker.FileTypeChoices.Add(_qgl_project_extension_name,
                                    new List<string>()
                                    {
                                       _qgl_project_extension
                                    });

         return picker.PickSaveFileAsync();
      }

      public static IAsyncOperation<StorageFile> PickOpenEntryFileAsync()
      {
         FileOpenPicker picker = new FileOpenPicker();
         picker.FileTypeFilter.Add("*");
         return picker.PickSingleFileAsync();
      }

      public static IAsyncOperation<StorageFile> PickSaveEntryFileAsync(
         ContentMetadata metadata,
         string extension)
      {
         string fileName = string.Format("{0}-{1}",
                                         metadata.ResourceType.ToString(),
                                         metadata.ContentLoader.ToString());

         FileSavePicker picker = new FileSavePicker();
         picker.SuggestedFileName = fileName;
         picker.FileTypeChoices.Add(metadata.ContentLoader.ToString(),
                                    new List<string>()
                                    {
                                       extension
                                    });

         return picker.PickSaveFileAsync();
      }

      private const string _qg_project_suggested_name = "New Project";
      private const string _qgl_project_extension_name = "QGL Content Project";
      private const string _qgl_project_extension = ".qcproj";
   }
}
