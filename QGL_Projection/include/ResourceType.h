#pragma once
#include "ResourceType.g.h"
#include <winrt/Windows.Foundation.Collections.h>
#include <QGLContent.h>

using namespace winrt::Windows::Foundation::Collections;

namespace winrt::QGL_Projection::implementation
{
   struct ResourceType : ResourceTypeT<ResourceType>
   {
      ResourceType() = delete;

      ResourceType(uint16_t enumValue,
                   hstring const& name,
                   array_view<uint16_t const> supportedLoaders);

      uint16_t Value();

      com_array<uint16_t> SupportedLoaderIDs();

      hstring ToString();

      private:
      winrt::hstring m_name;
      qgl::content::RESOURCE_TYPES m_type;
      std::vector<uint16_t> m_supportedLoaderIDs;
   };
}
namespace winrt::QGL_Projection::factory_implementation
{
   struct ResourceType : ResourceTypeT<ResourceType,
      implementation::ResourceType>
   {
   };
}
