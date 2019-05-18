#include "pch.h"
#include "ResourceType.h"

namespace winrt::QGL_Projection::implementation
{
   ResourceType::ResourceType(uint16_t enumValue,
                              hstring const& name,
                              array_view<uint16_t const> supportedLoaders) :
      m_type(static_cast<qgl::content::RESOURCE_TYPES>(enumValue)),
      m_name(name),
      m_supportedLoaderIDs(supportedLoaders.begin(), supportedLoaders.end())
   {
   }

   uint16_t ResourceType::Value()
   {
      return m_type;
   }

   com_array<uint16_t> ResourceType::SupportedLoaderIDs()
   {
      return com_array<uint16_t>(m_supportedLoaderIDs);
   }

   hstring ResourceType::ToString()
   {
      return m_name;
   }
}
