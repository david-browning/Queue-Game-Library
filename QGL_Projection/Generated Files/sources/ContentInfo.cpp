#include "pch.h"
#include "ContentInfo.h"

namespace winrt::QGL_Projection::implementation
{
    bool ContentInfo::ContentVisible()
    {
        throw hresult_not_implemented();
    }

    void ContentInfo::ContentVisible(bool value)
    {
        throw hresult_not_implemented();
    }

    bool ContentInfo::ContentObeyPhysics()
    {
        throw hresult_not_implemented();
    }

    void ContentInfo::ContentObeyPhysics(bool value)
    {
        throw hresult_not_implemented();
    }

    hstring ContentInfo::ContentName()
    {
        throw hresult_not_implemented();
    }

    void ContentInfo::ContentName(hstring const& value)
    {
        throw hresult_not_implemented();
    }

    QGL_Projection::ResourceType ContentInfo::ResourceType()
    {
        throw hresult_not_implemented();
    }

    void ContentInfo::ResourceType(QGL_Projection::ResourceType const& value)
    {
        throw hresult_not_implemented();
    }

    QGL_Projection::ContentLoader ContentInfo::ContentLoader()
    {
        throw hresult_not_implemented();
    }

    void ContentInfo::ContentLoader(QGL_Projection::ContentLoader const& value)
    {
        throw hresult_not_implemented();
    }

    QGL_Projection::CompilerVersion ContentInfo::CompilerVersion()
    {
        throw hresult_not_implemented();
    }

    void ContentInfo::CompilerVersion(QGL_Projection::CompilerVersion const& value)
    {
        throw hresult_not_implemented();
    }

    winrt::guid ContentInfo::Guid()
    {
        throw hresult_not_implemented();
    }

    winrt::event_token ContentInfo::PropertyChanged(Windows::UI::Xaml::Data::PropertyChangedEventHandler const& handler)
    {
        throw hresult_not_implemented();
    }

    void ContentInfo::PropertyChanged(winrt::event_token const& token) noexcept
    {
        throw hresult_not_implemented();
    }

    void ContentInfo::WriteContentInfoToBuffer(QGL_Projection::ContentInfo const& info, Windows::Storage::Streams::DataWriter const& r)
    {
        throw hresult_not_implemented();
    }

    QGL_Projection::ContentInfo ContentInfo::ReadContentInfoFromBuffer(Windows::Storage::Streams::DataReader const& r, Windows::Foundation::Collections::IVector<QGL_Projection::ResourceType> const& supportedResourceTypes)
    {
        throw hresult_not_implemented();
    }
}
