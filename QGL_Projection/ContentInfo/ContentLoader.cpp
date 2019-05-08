#include "pch.h"
#include "ContentLoader.h"

namespace winrt::QGL_Projection::implementation
{
   ContentLoader::ContentLoader(uint16_t val,
                                hstring const& name) :
      m_name(name),
      m_id(static_cast<qgl::content::CONTENT_LOADER_IDS>(val))
   {

   }

   uint16_t ContentLoader::Value()
   {
      return static_cast<uint16_t>(m_id);
   }

   hstring ContentLoader::ToString()
   {
      return m_name;
   }
}
