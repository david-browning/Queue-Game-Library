#include "pch.h"
#include "ContentProject.h"

namespace winrt::QGL_Projection::implementation
{
    QGL_Projection::ContentInfo ContentProject::ContentInfo()
    {
        throw hresult_not_implemented();
    }

    void ContentProject::ContentInfo(QGL_Projection::ContentInfo const& value)
    {
        throw hresult_not_implemented();
    }

    Windows::Foundation::Collections::IObservableVector<Windows::Foundation::IInspectable> ContentProject::ObservableEntries()
    {
        throw hresult_not_implemented();
    }

    void ContentProject::ObservableEntries(Windows::Foundation::Collections::IObservableVector<Windows::Foundation::IInspectable> const& value)
    {
        throw hresult_not_implemented();
    }

    Windows::Foundation::IAsyncOperation<bool> ContentProject::LoadFromFileAsync(Windows::Storage::StorageFile const f, Windows::Foundation::Collections::IVector<QGL_Projection::ResourceType> const supported)
    {
        throw hresult_not_implemented();
    }

    Windows::Foundation::IAsyncOperation<bool> ContentProject::SaveToFileAsync(Windows::Storage::StorageFile const f)
    {
        throw hresult_not_implemented();
    }

    winrt::event_token ContentProject::PropertyChanged(Windows::UI::Xaml::Data::PropertyChangedEventHandler const& handler)
    {
        throw hresult_not_implemented();
    }

    void ContentProject::PropertyChanged(winrt::event_token const& token) noexcept
    {
        throw hresult_not_implemented();
    }
}
