using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Windows.Storage;

namespace QGL_Content_Builder.Helpers
{
   internal static class AppDataHelpers
   {
      public static void SetAutoNavigatePage(string pageName)
      {
         ApplicationDataContainer applicationData = ApplicationData.Current.LocalSettings;
         applicationData.Values[AUTO_NAVIGATE_KEY] = pageName;
      }

      public static void ClearAutoNavigatePage()
      {
         ApplicationDataContainer applicationData = ApplicationData.Current.LocalSettings;
         applicationData.Values[AUTO_NAVIGATE_KEY] = AUTO_NAVIGATE_NO_PAGE;
      }

      public static bool IsAutoNavigatePageSet()
      {
         ApplicationDataContainer applicationData = ApplicationData.Current.LocalSettings;
         var page = (string)applicationData.Values[AUTO_NAVIGATE_KEY];
         return !page.Equals(AUTO_NAVIGATE_NO_PAGE);
      }

      public static string GetAutoNavigatePage()
      {
         ApplicationDataContainer applicationData = ApplicationData.Current.LocalSettings;
         return (string)applicationData.Values[AUTO_NAVIGATE_KEY];
      }

      public static void ClearProjectPath()
      {
         ApplicationDataContainer applicationData = ApplicationData.Current.LocalSettings;
         applicationData.Values[PROJECT_PATH_KEY] = PROJECT_PATH_NO_PATH;
      }

      public static void SetProjectPath(string path)
      {
         ApplicationDataContainer applicationData = ApplicationData.Current.LocalSettings;
         applicationData.Values[PROJECT_PATH_KEY] = path;
      }

      public static bool IsProjectPathSet()
      {
         ApplicationDataContainer applicationData = ApplicationData.Current.LocalSettings;
         var setPath = (string)applicationData.Values[PROJECT_PATH_KEY];
         return !setPath.Equals(PROJECT_PATH_NO_PATH);
      }

      /// <summary>
      /// The project path must already be added to the app data container.
      /// </summary>
      public static string GetProjectPath()
      {
         ApplicationDataContainer applicationData = ApplicationData.Current.LocalSettings;
         return (string)applicationData.Values[PROJECT_PATH_KEY];
      }

      private static readonly string PROJECT_PATH_KEY = "PROJECT_PATH";
      private static readonly string PROJECT_PATH_NO_PATH = "NO_PROJECT";
      private static readonly string AUTO_NAVIGATE_KEY = "AUTO_NAVIGATE";
      private static readonly string AUTO_NAVIGATE_NO_PAGE = "NO_PAGE";
   }
}
