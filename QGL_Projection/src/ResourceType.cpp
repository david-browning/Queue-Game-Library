#include "pch.h"
#include "ResourceType.h"

namespace winrt::QGL_Projection::implementation
{
   ResourceType::ResourceType(uint16_t enumValue,
                              hstring const& name) :
      m_type(static_cast<qgl::content::RESOURCE_TYPES>(enumValue)),
      m_name(name)
   {
   }

   uint16_t ResourceType::Value()
   {
      return m_type;
   }

   hstring ResourceType::ToString()
   {
      return m_name;
   }
}
