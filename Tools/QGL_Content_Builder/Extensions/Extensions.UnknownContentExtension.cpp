#include "pch.h"
#include "Extensions.UnknownContentExtension.h"
#include <QGLContent.h>

namespace winrt::QGL_Content_Builder::Extensions::implementation
{
   uint16_t UnknownContentExtension::ResourceType()
   {
      return qgl::content::RESOURCE_TYPES::RESOURCE_TYPE_UNKNOWN;
   }

   uint16_t UnknownContentExtension::ContentLoaderID()
   {
      return qgl::content::CONTENT_LOADER_IDS::CONTENT_LOADER_ID_UNKNOWN;
   }

   hstring UnknownContentExtension::ContentExtension()
   {
      return L".unkn";
   }

   hstring UnknownContentExtension::ContentLoaderName()
   {
      return L"Unknown";
   }

   hstring UnknownContentExtension::ResourceTypeName()
   {
      return L"Unknown";
   }

   Windows::Foundation::IAsyncAction UnknownContentExtension::Open(Windows::Storage::StorageFile const& f)
   {
      throw hresult_not_implemented();
   }

   Windows::Foundation::IAsyncAction UnknownContentExtension::Save(Windows::Storage::StorageFile const& f)
   {
      throw hresult_not_implemented();
   }
}
