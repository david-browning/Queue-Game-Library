#pragma once
#include "ContentProject.g.h"
#include "include/ContentMetadata.h"

using namespace winrt::Windows::Foundation;
using namespace winrt::Windows::Foundation::Collections;
using namespace winrt::Windows::UI::Xaml;

namespace winrt::QGL_Projection::implementation
{
   struct ContentProject : ContentProjectT<ContentProject>
   {
      ContentProject();

      IAsyncOperation<bool> LoadFromFileAsync(Windows::Storage::StorageFile f);

      QGL_Projection::ContentMetadata Metadata();

      void Metadata(QGL_Projection::ContentMetadata value);

      IObservableVector<IInspectable> Entries();

      void Entries(IObservableVector<IInspectable> value);

      winrt::event_token PropertyChanged(
         Data::PropertyChangedEventHandler const& handler);

      void PropertyChanged(winrt::event_token const& token) noexcept;

      private:
      QGL_Projection::ContentMetadata m_metadata{ nullptr };
      IObservableVector<IInspectable> m_observableEntries;
      winrt::event<Data::PropertyChangedEventHandler> m_propertyChanged;
   };
}
namespace winrt::QGL_Projection::factory_implementation
{
   struct ContentProject : ContentProjectT<ContentProject, implementation::ContentProject>
   {
   };
}
