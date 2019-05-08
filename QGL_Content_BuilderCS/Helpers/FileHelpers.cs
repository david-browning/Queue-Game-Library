using QGL_Content_Builder.PageModels;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Windows.Foundation;
using Windows.Storage;
using Windows.Storage.Pickers;
using Windows.Storage.Streams;
using Windows.Storage.AccessCache;

namespace QGL_Content_Builder.Helpers
{
   public static class FileHelpers
   {
      public static IAsyncOperation<StorageFile> GetProjectUsingOpenPickerAsync()
      {
         FileOpenPicker pckr = new FileOpenPicker();
         pckr.SuggestedStartLocation = PickerLocationId.ComputerFolder;
         pckr.FileTypeFilter.Add(QGL_PROJECT_EXTENSION);

         return pckr.PickSingleFileAsync();
      }

      public static IAsyncOperation<StorageFile> GetProjectUsingSavePickerAsync()
      {
         FileSavePicker picker = new FileSavePicker();
         picker.SuggestedFileName = QGL_PROJECT_SUGGESTED_NAME;
         picker.SuggestedStartLocation = PickerLocationId.ComputerFolder;
         picker.FileTypeChoices.Add(QGL_PROJECT_EXTENSION_NAME, new List<string>(){ QGL_PROJECT_EXTENSION});

         return picker.PickSaveFileAsync();
      }

      public static IAsyncOperation<StorageFile> PickDictionaryEntryFileAsync()
      {
         FileOpenPicker picker = new FileOpenPicker();
         picker.FileTypeFilter.Add("*");
         return picker.PickSingleFileAsync();
      }

      public static IAsyncOperation<StorageFile> PickNewDictionaryEntryFileAsync(QGL_Projection.ContentInfo info,
                                                                                  string extension)
      {
         string fileName = info.ResourceType.ToString() + '-' + info.ContentLoader.ToString();

         FileSavePicker picker = new FileSavePicker();
         picker.SuggestedFileName = fileName;
         picker.FileTypeChoices.Add(info.ContentLoader.ToString(), new List<string>() { extension });

         return picker.PickSaveFileAsync();
      }

      public static async Task<ObservableCollection<RecentProjectModel>> GetMostRecentlyUsedListAsync(IList<QGL_Projection.ResourceType> supported)
      {
         var ret = new ObservableCollection<RecentProjectModel>();
         var mru = StorageApplicationPermissions.MostRecentlyUsedList;
         foreach(var entry in mru.Entries)
         {
            try
            {
               var f = await mru.GetFileAsync(entry.Token);
               if(f != null)
               {
                  var recentFile = await GetRecentProjectAsync(f, supported);
                  if(recentFile != null)
                  {
                     ret.Add(recentFile);
                  }
               }
            }
            catch(Exception)
            {
               mru.Remove(entry.Token);
            }
         }

         return ret;
      }

      public static void AddToRecentAccessList(StorageFile f)
      {
         var mru = StorageApplicationPermissions.MostRecentlyUsedList;
         foreach(var entry in mru.Entries)
         {
            if(entry.Metadata.Equals(f.Path))
            {
               mru.AddOrReplace(entry.Token, f, f.Path);
               return;
            }
         }

         mru.Add(f, f.Path);
      }

      public static IAsyncOperation<StorageFile> GetRecentFileAsync(string filePath)
      {
         var mru = StorageApplicationPermissions.MostRecentlyUsedList;
         foreach(var entry in mru.Entries)
         {
            if(entry.Metadata.Equals(filePath))
            {
               return mru.GetFileAsync(entry.Token);
            }
         }

         return null;
      }

      public static void ClearRecentFiles()
      {
         StorageApplicationPermissions.MostRecentlyUsedList.Clear();
      }

      public static void AddToFutureAccessList(StorageFile f)
      {
         if(StorageApplicationPermissions.FutureAccessList.Entries.Count > MAX_FAL_ENTRIES)
         {
            TrimFutureAccessList(MAX_FAL_ENTRIES / 2);
         }

         var fal = StorageApplicationPermissions.FutureAccessList;
         foreach(var entry in fal.Entries)
         {
            if(entry.Metadata.Equals(f.Path))
            {
               fal.AddOrReplace(entry.Token, f, f.Path);
               return;
            }
         }

         fal.Add(f, f.Path);
      }

      public static IAsyncOperation<StorageFile> GetFutureFileAsync(string filePath)
      {
         var fal = StorageApplicationPermissions.FutureAccessList;
         foreach(var entry in fal.Entries)
         {
            if(entry.Metadata.Equals(filePath))
            {
               return fal.GetFileAsync(entry.Token);
            }
         }

         return null;
      }

      public static void ClearFutureAccessList()
      {
         var fal = StorageApplicationPermissions.FutureAccessList;
         fal.Clear();
      }

      private static void TrimFutureAccessList(int trimTo)
      {
         var fal = StorageApplicationPermissions.FutureAccessList;
         while(trimTo > 0)
         {
            var lastIdx = fal.Entries.Count - 1;
            fal.Remove(fal.Entries[lastIdx].Token);
            trimTo--;
         }
      }

      public static async Task<RecentProjectModel> GetRecentProjectAsync(StorageFile f, IList<QGL_Projection.ResourceType> supported)
      {
         //Get a buffer to the file.
         IRandomAccessStream stream = await f.OpenAsync(FileAccessMode.Read);

         //Offset the 8-byte magic number.
         IInputStream inputStream = stream.GetInputStreamAt(sizeof(UInt64));
         if(stream.Size == 0)
         {
            return null;
         }

         DataReader dataReader = new DataReader(inputStream);
         dataReader.UnicodeEncoding = Windows.Storage.Streams.UnicodeEncoding.Utf16LE;
         dataReader.ByteOrder = ByteOrder.LittleEndian;

         await dataReader.LoadAsync((uint)stream.Size);
         var info = QGL_Projection.ContentInfo.ReadContentInfoFromBuffer(dataReader, supported);
         var ret = new RecentProjectModel(f.Path, info);

         return ret;
      }

      private const string QGL_PROJECT_SUGGESTED_NAME = "New Project";
      private const string QGL_PROJECT_EXTENSION_NAME = "QGL Content Project";
      private const string QGL_PROJECT_EXTENSION = ".qcproj";
      private const int MAX_FAL_ENTRIES = 900;
   }
}
