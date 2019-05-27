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
                   hstring const& name);

      uint16_t Value();

      hstring ToString();

      private:
      winrt::hstring m_name;
      qgl::content::RESOURCE_TYPES m_type;
   };
}
namespace winrt::QGL_Projection::factory_implementation
{
   struct ResourceType : ResourceTypeT<ResourceType,
      implementation::ResourceType>
   {
   };
}
