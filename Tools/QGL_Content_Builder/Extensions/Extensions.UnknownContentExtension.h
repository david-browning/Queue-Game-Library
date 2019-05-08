#pragma once

#include "Extensions.UnknownContentExtension.g.h"

namespace winrt::QGL_Content_Builder::Extensions::implementation
{
    struct UnknownContentExtension : UnknownContentExtensionT<UnknownContentExtension>
    {
        UnknownContentExtension() = default;

        uint16_t ResourceType();
        uint16_t ContentLoaderID();
        hstring ContentExtension();
        hstring ContentLoaderName();
        hstring ResourceTypeName();
        Windows::Foundation::IAsyncAction Open(Windows::Storage::StorageFile const& f);
        Windows::Foundation::IAsyncAction Save(Windows::Storage::StorageFile const& f);
    };
}

namespace winrt::QGL_Content_Builder::Extensions::factory_implementation
{
    struct UnknownContentExtension : UnknownContentExtensionT<UnknownContentExtension, implementation::UnknownContentExtension>
    {
    };
}
