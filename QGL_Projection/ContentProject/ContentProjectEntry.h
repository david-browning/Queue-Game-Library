#pragma once
#include "ContentProjectEntry.g.h"

namespace winrt::QGL_Projection::implementation
{
    struct ContentProjectEntry : ContentProjectEntryT<ContentProjectEntry>
    {
        ContentProjectEntry(hstring const& filePath,
                            QGL_Projection::ContentInfo const& info);

        QGL_Projection::ContentInfo ContentInfo();
        void ContentInfo(QGL_Projection::ContentInfo const& value);

        hstring FilePath();

        hstring ToString();

        winrt::Windows::Foundation::IAsyncAction ChangeFile(winrt::Windows::Storage::StorageFile const& newFile);

        winrt::event_token PropertyChanged(Windows::UI::Xaml::Data::PropertyChangedEventHandler const& handler);
        void PropertyChanged(winrt::event_token const& token) noexcept;

       private:
       winrt::event<Windows::UI::Xaml::Data::PropertyChangedEventHandler> m_propertyChanged;

       /*
        The content info for this entry.
        */
       QGL_Projection::ContentInfo m_info;

       /*
        Size, in bytes, of the content buffer. This is updated whenever the file path is set.
        */
       uint64_t m_dataSize;

       /*
        Path to the content buffer.
        */
       hstring m_filePath;
    };
}

namespace winrt::QGL_Projection::factory_implementation
{
    struct ContentProjectEntry : ContentProjectEntryT<ContentProjectEntry, implementation::ContentProjectEntry>
    {
    };
}
