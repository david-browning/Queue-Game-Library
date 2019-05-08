#include "pch.h"
#include "ContentProjectEntry.h"

namespace winrt::QGL_Projection::implementation
{
    ContentProjectEntry::ContentProjectEntry(hstring const& filePath, QGL_Projection::ContentInfo const& info)
    {
        throw hresult_not_implemented();
    }

    QGL_Projection::ContentInfo ContentProjectEntry::ContentInfo()
    {
        throw hresult_not_implemented();
    }

    void ContentProjectEntry::ContentInfo(QGL_Projection::ContentInfo const& value)
    {
        throw hresult_not_implemented();
    }

    hstring ContentProjectEntry::FilePath()
    {
        throw hresult_not_implemented();
    }

    Windows::Foundation::IAsyncAction ContentProjectEntry::ChangeFile(Windows::Storage::StorageFile const& newFile)
    {
        throw hresult_not_implemented();
    }

    hstring ContentProjectEntry::ToString()
    {
        throw hresult_not_implemented();
    }

    winrt::event_token ContentProjectEntry::PropertyChanged(Windows::UI::Xaml::Data::PropertyChangedEventHandler const& handler)
    {
        throw hresult_not_implemented();
    }

    void ContentProjectEntry::PropertyChanged(winrt::event_token const& token) noexcept
    {
        throw hresult_not_implemented();
    }
}
