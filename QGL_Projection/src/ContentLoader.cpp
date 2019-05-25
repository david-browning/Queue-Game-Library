#include "pch.h"
#include "ContentLoader.h"

namespace winrt::QGL_Projection::implementation
{
   ContentLoader::ContentLoader(uint16_t enumValue, hstring const& name) :
      m_id(static_cast<qgl::content::CONTENT_LOADER_IDS>(enumValue)),
      m_name(name)
   {
   }

   uint16_t ContentLoader::Value()
   {
      return m_id;
   }

   hstring ContentLoader::ToString()
   {
      return m_name;
   }
}
