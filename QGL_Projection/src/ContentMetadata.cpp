#include "pch.h"
#include "ContentMetadata.h"

namespace winrt::QGL_Projection::implementation
{
   bool ContentMetadata::ContentVisible()
   {
      return m_metadata.content_visible();
   }

   void ContentMetadata::ContentVisible(bool value)
   {
      if (m_metadata.content_visible() != value)
      {
         m_metadata.content_visible(value);
         m_propertyChanged(*this,
                           PropertyChangedEventArgs{ L"ContentVisible" });
      }
   }

   bool ContentMetadata::ContentObeyPhysics()
   {
      return m_metadata.obey_physics();
   }

   void ContentMetadata::ContentObeyPhysics(bool value)
   {
      if (m_metadata.obey_physics() != value)
      {
         m_metadata.obey_physics(value);
         m_propertyChanged(*this,
                           PropertyChangedEventArgs{ L"ContentObeyPhysics" });
      }
   }

   hstring ContentMetadata::ContentName()
   {
      return m_metadata.name();
   }

   void ContentMetadata::ContentName(hstring value)
   {
      if (m_metadata.name() != value)
      {
         m_metadata.name(value);
         m_propertyChanged(*this,
                           PropertyChangedEventArgs{ L"ContentName" });
      }
   }

   uint16_t ContentMetadata::ResourceType()
   {
      return m_metadata.resource_type();
   }

   void ContentMetadata::ResourceType(uint16_t value)
   {
      if (m_metadata.resource_type() != value)
      {
         m_metadata.resource_type() = value;
         m_propertyChanged(*this,
                           PropertyChangedEventArgs{ L"ResourceType" });
      }
   }

   uint16_t ContentMetadata::ContentLoader()
   {
      return m_metadata.loader_id();
   }

   void ContentMetadata::ContentLoader(uint16_t value)
   {
      if (m_metadata.loader_id() != value)
      {
         m_metadata.loader_id() = value;
         m_propertyChanged(*this,
                           PropertyChangedEventArgs{ L"ContentLoader" });
      }
   }

   uint32_t ContentMetadata::CompilerVersion()
   {
      return m_metadata.version();
   }

   void ContentMetadata::CompilerVersion(uint32_t value)
   {
      if (m_metadata.version() != value)
      {
         m_metadata.version() = value;
         m_propertyChanged(*this,
                           PropertyChangedEventArgs{ L"CompilerVersion" });
      }
   }

   winrt::guid ContentMetadata::Guid()
   {
      return m_metadata.guid();
   }

   bool ContentMetadata::Equals(QGL_Projection::ContentMetadata const& m)
   {
      //Get the implementation pointer.
      auto metaImpl =
         winrt::get_self<QGL_Projection::implementation::ContentMetadata>(
            m);
      return m_metadata == metaImpl->m_metadata;
   }

   winrt::event_token ContentMetadata::PropertyChanged(PropertyChangedEventHandler const& handler)
   {
      return m_propertyChanged.add(handler);
   }

   void ContentMetadata::PropertyChanged(winrt::event_token const& token) noexcept
   {
      m_propertyChanged.remove(token);
   }
}
