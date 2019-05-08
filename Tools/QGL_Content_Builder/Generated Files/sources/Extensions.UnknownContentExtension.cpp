#include "pch.h"
#include "Extensions.UnknownContentExtension.h"

namespace winrt::QGL_Content_Builder::Extensions::implementation
{
    uint16_t UnknownContentExtension::ResourceType()
    {
        throw hresult_not_implemented();
    }

    uint16_t UnknownContentExtension::ContentLoaderID()
    {
        throw hresult_not_implemented();
    }

    hstring UnknownContentExtension::ContentExtension()
    {
        throw hresult_not_implemented();
    }

    hstring UnknownContentExtension::ContentLoaderName()
    {
        throw hresult_not_implemented();
    }

    hstring UnknownContentExtension::ResourceTypeName()
    {
        throw hresult_not_implemented();
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
