using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using QGL_ContentBuilder.ViewModels;
using Windows.Foundation;
using Windows.Storage;
using Windows.Storage.AccessCache;
using Windows.Storage.Streams;

namespace QGL_ContentBuilder.Providers
{
   internal static class RecentFileProvider
   {
      /// <summary>
      /// Adds a file to the recently used list. 
      /// </summary>
      /// <param name="f">File to add.</param>
      public static void AddToRecentAccessList(StorageFile f)
      {
         var mru = StorageApplicationPermissions.MostRecentlyUsedList;

         foreach (var entry in mru.Entries)
         {
            if (entry.Metadata.Equals(f.Path))
            {
               mru.AddOrReplace(entry.Token, f, f.Path);
               return;
            }
         }

         mru.Add(f, f.Path);
      }

      /// <summary>
      /// Given an absolute file path, this returns a storage file at the 
      /// path.The file must have been added to the recently used list by
      /// calling RcentFileProvider.AddToRecentAccessList().
      /// </summary>
      /// <param name="filePath">Absolute file path.</param>
      /// <returns>An awaitable storage file. If the file does not have
      /// future access permissions, this return null.</returns>
      public static IAsyncOperation<StorageFile> GetRecentFileAsync(
          string filePath)
      {
         var mru = StorageApplicationPermissions.MostRecentlyUsedList;
         foreach (var entry in mru.Entries)
         {
            if (entry.Metadata.Equals(filePath))
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

      /// <summary>
      /// Adds a storage file to a list of files that can be accessed in 
      /// the future. This is useful so that the user does not need to 
      /// broker access to the file. It allows the app to read and write
      /// files without the user giving permission every time the app 
      /// launches.
      /// </summary>
      /// <remarks>If the future access list is full, this trims it to
      /// half the size.</remarks>
      /// <param name="f">The storage file to allow future access to.</param>
      public static void AddToFutureAccessList(StorageFile f)
      {
         if (StorageApplicationPermissions.FutureAccessList.Entries.Count > _max_fal_entries)
         {
            TrimFutureAccessList(_max_fal_entries / 2);
         }

         var fal = StorageApplicationPermissions.FutureAccessList;
         foreach (var entry in fal.Entries)
         {
            if (entry.Metadata.Equals(f.Path))
            {
               fal.AddOrReplace(entry.Token, f, f.Path);
               return;
            }
         }

         fal.Add(f, f.Path);
      }

      /// <summary>
      /// Given an absolute file path, this returns a storage file at the
      /// path. The file must have been given future access permissions by 
      /// using RecentFileProvider.AddToFutureAccessList().
      /// </summary>
      /// <param name="filePath">Absolute file path</param>
      /// <returns>An awaitable storage file. If the file does not have
      /// future access permissions, this return null.</returns>
      public static IAsyncOperation<StorageFile> GetFutureFileAsync(
          string filePath)
      {
         var fal = StorageApplicationPermissions.FutureAccessList;
         foreach (var entry in fal.Entries)
         {
            if (entry.Metadata.Equals(filePath))
            {
               return fal.GetFileAsync(entry.Token);
            }
         }

         return null;
      }

      /// <summary>
      /// Clears the future access list to 0 entries. 
      /// </summary>
      public static void ClearFutureAccessList()
      {
         var fal = StorageApplicationPermissions.FutureAccessList;
         fal.Clear();
      }

      public static async Task<ObservableCollection<RecentProjectViewModel>> 
         GetMostRecentlyUsedListAsync()
      {
         var ret = new ObservableCollection<RecentProjectViewModel>();
         var mru = StorageApplicationPermissions.MostRecentlyUsedList;
         foreach (var entry in mru.Entries)
         {
            try
            {
               var f = await mru.GetFileAsync(entry.Token);
               if (f != null)
               {
                  var recentFile = await GetRecentProjectAsync(f);
                  if (recentFile != null)
                  {
                     ret.Add(recentFile);
                  }
               }
            }
            catch (Exception)
            {
               mru.Remove(entry.Token);
            }
         }

         return ret;
      }

      public static async Task<RecentProjectViewModel> GetRecentProjectAsync(
         StorageFile f)
      {
         throw new NotImplementedException();
      }

      private static void TrimFutureAccessList(int trimTo)
      {
         var fal = StorageApplicationPermissions.FutureAccessList;
         while (trimTo > 0)
         {
            var lastIdx = fal.Entries.Count - 1;
            fal.Remove(fal.Entries[lastIdx].Token);
            trimTo--;
         }
      }

      private const int _max_fal_entries = 900;
   }
}
