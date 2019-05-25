#include "pch.h"
#include "ContentProject.h"
#include "ContentProject.g.h"
#include <winrt/Windows.Foundation.Collections.h>

using namespace winrt::Windows::UI::Xaml::Data;

namespace winrt::QGL_Projection::implementation
{
   ContentProject::ContentProject()
   {
      Metadata( 
         winrt::make<QGL_Projection::implementation::ContentMetadata>());
      Entries(winrt::single_threaded_observable_vector<IInspectable>());
   }

   IAsyncOperation<bool> ContentProject::LoadFromFileAsync(
      Windows::Storage::StorageFile f)
   {
      throw hresult_not_implemented();
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
