#include "pch.h"
#include "ContentProjectEntry.h"
using namespace winrt::Windows::Foundation;
using namespace winrt::Windows::UI::Xaml::Data;
using namespace winrt::Windows::Foundation::Collections;

namespace winrt::QGL_Projection::implementation
{
   ContentProjectEntry::ContentProjectEntry(
      hstring const& filePath,
      QGL_Projection::ContentMetadata const& meta) :
      m_filePath(filePath),
      m_metaData(meta)
   {
   }

   QGL_Projection::ContentMetadata ContentProjectEntry::Metadata()
   {
      return m_metaData;
   }

   void ContentProjectEntry::Metadata(QGL_Projection::ContentMetadata value)
   {
      if (m_metaData != value)
      {
         m_metaData = value;
         m_propertyChanged(*this, PropertyChangedEventArgs{ L"Metadata" });
      }
   }

   hstring ContentProjectEntry::FilePath()
   {
      return m_filePath;
   }

   void ContentProjectEntry::FilePath(hstring value)
   {
      if (m_filePath != value)
      {
         m_filePath = value;
         m_propertyChanged(*this, PropertyChangedEventArgs{ L"FilePath" });
      }
   }

   winrt::event_token ContentProjectEntry::PropertyChanged(
      Data::PropertyChangedEventHandler const& handler)
   {
      return m_propertyChanged.add(handler);
   }

   void ContentProjectEntry::PropertyChanged(
      winrt::event_token const& token) noexcept
   {
      m_propertyChanged.remove(token);
   }

   hstring ContentProjectEntry::ToString()
   {
      return m_metaData.ContentName();
   }
}
