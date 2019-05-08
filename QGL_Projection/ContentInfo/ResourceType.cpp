#include "pch.h"
#include "ResourceType.h"

namespace winrt::QGL_Projection::implementation
{
   ResourceType::ResourceType(uint16_t val,
                              hstring const& name,
                              array_view<QGL_Projection::ContentLoader const> loaders) :
      m_type(static_cast<qgl::content::RESOURCE_TYPES>(val)),
      m_name(name)
   {
      m_observableSupportedLoaders = winrt::single_threaded_observable_vector<IInspectable>();
      m_supportedLoaders = winrt::single_threaded_vector<QGL_Projection::ContentLoader>();
      for (auto it : loaders)
      {
         m_observableSupportedLoaders.Append(it);
         m_supportedLoaders.Append(it);
      }
   }

   uint16_t ResourceType::Value()
   {
      return static_cast<uint16_t>(m_type);
   }

   Windows::Foundation::Collections::IVector<QGL_Projection::ContentLoader> ResourceType::Loaders()
   {
      return m_supportedLoaders;
   }

   Windows::Foundation::Collections::IObservableVector<Windows::Foundation::IInspectable> ResourceType::ObservableLoaders()
   {
      return m_observableSupportedLoaders;
   }

   hstring ResourceType::ToString()
   {
      return m_name;
   }
}
