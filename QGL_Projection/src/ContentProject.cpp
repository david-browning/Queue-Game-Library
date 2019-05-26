#include "pch.h"
#include "ContentProject.h"
#include "ContentProject.g.h"
#include "include/ContentProjectEntry.h"
#include "ContentProjectEntry.g.h"
#include <winrt/Windows.Foundation.Collections.h>

using namespace winrt::Windows::UI::Xaml::Data;
using namespace winrt::Windows::Storage;

namespace winrt::QGL_Projection::implementation
{
   ContentProject::ContentProject()
   {
      Metadata(
         winrt::make<QGL_Projection::implementation::ContentMetadata>());
      Entries(winrt::single_threaded_observable_vector<IInspectable>());
   }

   bool ContentProject::LoadFromFileAsync(StorageFile const& f)
   {
      //Load the project
      qgl::content::content_project p(f);

      //Get an implementation pointer to this's metadata so we can overwrite
      //it with what was just loaded.
      auto metaImpl =
         winrt::get_self<QGL_Projection::implementation::ContentMetadata>(
            m_metadata);
      //Overwrite this's metadata
      metaImpl->FromBuffer(p.metadata());

      //Create projected content project entries and add them to this's
      //list of observable entries.
      for (auto& entry : p)
      {
         //Put the metadata from the project file into a projected metadata.
         auto toPushMeta = 
            winrt::make<QGL_Projection::implementation::ContentMetadata>();
         auto toPushMetaImpl = 
            winrt::get_self<QGL_Projection::implementation::ContentMetadata>(
               toPushMeta);
         toPushMetaImpl->FromBuffer(entry.first);

         //Create a projected content project entry
         QGL_Projection::ContentProjectEntry toPush =
            winrt::make<QGL_Projection::implementation::ContentProjectEntry>(
               entry.second,
               toPushMeta);

         //Append it to the end of the observable entries.
         m_observableEntries.Append(winrt::box_value(toPush));
      }

      return true;
   }

   bool ContentProject::SaveProjectFile(StorageFile const& f)
   {
      auto metaImpl = 
         winrt::get_self<QGL_Projection::implementation::ContentMetadata>(
            m_metadata);

      //Create the project.
      qgl::content::content_project p(f);

      //Set the metadata.
      auto metadataBuffer = metaImpl->ToBuffer();
      p.metadata() = metadataBuffer;

      //Add all the project entries in this to the project.
      for (auto& inspec : m_observableEntries)
      {
         //Cast the IInspectable to a ContentProjectEntry
         auto entry = 
            winrt::unbox_value<QGL_Projection::ContentProjectEntry>(inspec);

         auto entryMetaImpl =
            winrt::get_self<QGL_Projection::implementation::ContentMetadata>(
               entry.Metadata());

         p.emplace_back(entryMetaImpl->ToBuffer(), entry.FilePath());
      }

      //Flush
      p.flush();

      return false;
   }

   QGL_Projection::ContentMetadata ContentProject::Metadata()
   {
      assert(m_metadata != nullptr);
      return m_metadata;
   }

   void ContentProject::Metadata(QGL_Projection::ContentMetadata value)
   {
      if (m_metadata != value)
      {
         m_metadata = value;
         m_propertyChanged(*this, PropertyChangedEventArgs{ L"Metadata" });
      }
   }

   IObservableVector<IInspectable> ContentProject::Entries()
   {
      assert(m_observableEntries != nullptr);
      return m_observableEntries;
   }

   void ContentProject::Entries(IObservableVector<IInspectable> value)
   {
      if (m_observableEntries != value)
      {
         m_observableEntries = value;
         m_propertyChanged(*this, PropertyChangedEventArgs{ L"Entries" });
      }
   }

   winrt::event_token ContentProject::PropertyChanged(
      Data::PropertyChangedEventHandler const& handler)
   {
      return m_propertyChanged.add(handler);
   }

   void ContentProject::PropertyChanged(
      winrt::event_token const& token) noexcept
   {
      m_propertyChanged.remove(token);
   }
}
