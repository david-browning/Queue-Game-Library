#pragma once
#include "ContentProjectEntry.g.h"
#include "ContentMetadata.h"
using namespace winrt::Windows::UI::Xaml;

namespace winrt::QGL_Projection::implementation
{
   struct ContentProjectEntry : ContentProjectEntryT<ContentProjectEntry>
   {
      ContentProjectEntry() = delete;

      ContentProjectEntry(hstring const& filePath,
                          QGL_Projection::ContentMetadata const& meta);

      QGL_Projection::ContentMetadata Metadata();

      void Metadata(QGL_Projection::ContentMetadata value);

      hstring FilePath();

      void FilePath(hstring value);

      winrt::event_token PropertyChanged(
         Data::PropertyChangedEventHandler const& handler);

      void PropertyChanged(winrt::event_token const& token) noexcept;

      hstring ToString();

      private:
      winrt::event<Data::PropertyChangedEventHandler> m_propertyChanged;

      winrt::hstring m_filePath;

      QGL_Projection::ContentMetadata m_metaData;
   };
}
namespace winrt::QGL_Projection::factory_implementation
{
   struct ContentProjectEntry : ContentProjectEntryT<ContentProjectEntry,
      implementation::ContentProjectEntry>
   {
   };
}
