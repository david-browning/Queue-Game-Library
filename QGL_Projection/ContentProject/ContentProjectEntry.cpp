#include "pch.h"
#include "ContentProjectEntry.h"
#include "ContentInfo/ContentInfo.h"
#include "ContentInfo.g.h"
#include <QGLContent.h>
#include <winrt/Windows.Storage.h>
#include <winrt/Windows.Storage.FileProperties.h>

using namespace winrt::Windows::Foundation;
using namespace winrt::Windows::UI::Xaml::Data;
using namespace winrt::Windows::Foundation::Collections;

namespace winrt::QGL_Projection::implementation
{
   ContentProjectEntry::ContentProjectEntry(hstring const& filePath, 
                                            QGL_Projection::ContentInfo const & info) :
      m_filePath(filePath),
      m_dataSize(0),
      m_info(info)
   {
   }

   QGL_Projection::ContentInfo ContentProjectEntry::ContentInfo()
   {
      return m_info;
   }

   void ContentProjectEntry::ContentInfo(QGL_Projection::ContentInfo const& value)
   {
      if (m_info != value)
      {
         m_info = value;
         m_propertyChanged(*this, PropertyChangedEventArgs{ L"ContentInfo" });
      }
   }

   hstring ContentProjectEntry::FilePath()
   {
      return m_filePath;
   }

   hstring ContentProjectEntry::ToString()
   {
      return m_info.ContentName();
   }

   IAsyncAction ContentProjectEntry::ChangeFile(winrt::Windows::Storage::StorageFile const& newFile)
   {
      m_filePath = newFile.Path();
      auto basicProps = co_await newFile.GetBasicPropertiesAsync();
      m_dataSize = basicProps.Size();

      m_propertyChanged(*this, PropertyChangedEventArgs{ L"FilePath" });
      m_propertyChanged(*this, PropertyChangedEventArgs{ L"DataSize" });
   }

   winrt::event_token ContentProjectEntry::PropertyChanged(PropertyChangedEventHandler const& handler)
   {
      return m_propertyChanged.add(handler);
   }

   void ContentProjectEntry::PropertyChanged(winrt::event_token const& token) noexcept
   {
      m_propertyChanged.remove(token);
   }
}
