using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Windows.Storage;

namespace QGL_ContentBuilder.Helpers
{
   static class NavigationHelpers
   {
      public static void SetAutoNavigatePage(string pageName)
      {
         var applicationData = ApplicationData.Current.LocalSettings;
         applicationData.Values[_auto_navigate_key] = pageName;
      }

      public static void ClearAutoNavigatePage()
      {
         var applicationData = ApplicationData.Current.LocalSettings;
         applicationData.Values[_auto_navigate_key] = _auto_navigate_no_path;
      }

      public static bool IsAutoNavigatePageSet()
      {
         var applicationData = ApplicationData.Current.LocalSettings;
         var page = (string)applicationData.Values[_auto_navigate_key];
         return !page.Equals(_auto_navigate_no_path);
      }

      public static string GetAutoNavigatePage()
      {
         var applicationData = ApplicationData.Current.LocalSettings;
         return (string)applicationData.Values[_auto_navigate_key];
      }

      public static void ClearProjectPath()
      {
         var applicationData = ApplicationData.Current.LocalSettings;
         applicationData.Values[_project_path_key] = _project_path_no_path;
      }

      public static void SetProjectPath(string path)
      {
         var applicationData = ApplicationData.Current.LocalSettings;
         applicationData.Values[_project_path_key] = path;
      }

      public static bool IsProjectPathSet()
      {
         var applicationData = ApplicationData.Current.LocalSettings;
         var setPath = (string)applicationData.Values[_project_path_key];
         return !setPath.Equals(_project_path_no_path);
      }

      /// <summary>
      /// The project path must already be added to the app data container.
      /// </summary>
      public static string GetProjectPath()
      {
         var applicationData = ApplicationData.Current.LocalSettings;
         return (string)applicationData.Values[_project_path_key];
      }

      private static readonly string _project_path_key = "PROJECT_PATH";
      private static readonly string _project_path_no_path = "NO_PROJECT";
      private static readonly string _auto_navigate_key = "AUTO_NAVIGATE";
      private static readonly string _auto_navigate_no_path = "NO_PAGE";
   }
}
