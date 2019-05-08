#pragma once

#include "ContentProjectEntry.g.h"

namespace winrt::QGL_Projection::implementation
{
    struct ContentProjectEntry : ContentProjectEntryT<ContentProjectEntry>
    {
        ContentProjectEntry() = delete;
        ContentProjectEntry(hstring const& filePath, QGL_Projection::ContentInfo const& info);

        QGL_Projection::ContentInfo ContentInfo();
        void ContentInfo(QGL_Projection::ContentInfo const& value);
        hstring FilePath();
        Windows::Foundation::IAsyncAction ChangeFile(Windows::Storage::StorageFile const& newFile);
        hstring ToString();
        winrt::event_token PropertyChanged(Windows::UI::Xaml::Data::PropertyChangedEventHandler const& handler);
        void PropertyChanged(winrt::event_token const& token) noexcept;
    };
}

namespace winrt::QGL_Projection::factory_implementation
{
    struct ContentProjectEntry : ContentProjectEntryT<ContentProjectEntry, implementation::ContentProjectEntry>
    {
    };
}
