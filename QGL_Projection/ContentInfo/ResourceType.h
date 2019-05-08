#pragma once
#include "ResourceType.g.h"
#include <QGLContent.h>

namespace winrt::QGL_Projection::implementation
{
   struct ResourceType : ResourceTypeT<ResourceType>
   {
      ResourceType() = delete;
      ResourceType(uint16_t val,
                   hstring const& name,
                   array_view<QGL_Projection::ContentLoader const> loaders);

      uint16_t Value();
      Windows::Foundation::Collections::IVector<QGL_Projection::ContentLoader> Loaders();
      Windows::Foundation::Collections::IObservableVector<Windows::Foundation::IInspectable> ObservableLoaders();
      hstring ValueName()
      {
         return ToString();
      }
      hstring ToString();

      private:
      hstring m_name;
      qgl::content::RESOURCE_TYPES m_type;
      Windows::Foundation::Collections::IVector<QGL_Projection::ContentLoader> m_supportedLoaders;
      Windows::Foundation::Collections::IObservableVector<Windows::Foundation::IInspectable> m_observableSupportedLoaders;
   };
}

namespace winrt::QGL_Projection::factory_implementation
{
   struct ResourceType : ResourceTypeT<ResourceType, implementation::ResourceType>
   {
   };
}
