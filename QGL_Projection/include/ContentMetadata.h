#pragma once
#include "ContentMetadata.g.h"
#include <QGLContent.h>

using namespace winrt::Windows::UI::Xaml::Data;

namespace winrt::QGL_Projection::implementation
{
   struct ContentMetadata : ContentMetadataT<ContentMetadata>
   {
      ContentMetadata() = default;

      bool ContentVisible();

      void ContentVisible(bool value);

      bool ContentObeyPhysics();

      void ContentObeyPhysics(bool value);

      hstring ContentName();

      void ContentName(hstring value);

      uint16_t ResourceType();

      void ResourceType(uint16_t value);

      uint16_t ContentLoader();

      void ContentLoader(uint16_t value);

      uint32_t CompilerVersion();

      void CompilerVersion(uint32_t value);

      winrt::guid Guid();

      winrt::event_token PropertyChanged(
         PropertyChangedEventHandler const& handler);

      void PropertyChanged(winrt::event_token const& token) noexcept;

      private:
      winrt::event<PropertyChangedEventHandler> m_propertyChanged;
      qgl::content::CONTENT_METADATA_BUFFER m_metadata;
   };
}
