#include "pch.h"
#include "include/Content-Buffers/qgl_content_metadata_buffer.h"
#include <objbase.h>

qgl::content::CONTENT_METADATA_BUFFER::CONTENT_METADATA_BUFFER() :
    m_flags1(DEFAULT_FLAGS),
    m_reserved1(0),
    m_reserved2(0),
    m_type(RESOURCE_TYPES::RESOURCE_TYPE_UNKNOWN),
    m_loaderID(CONTENT_LOADER_IDS::CONTENT_LOADER_ID_UNKNOWN),
    m_compilerVersion(qgl::QGL_VERSION_LATEST)
{
    MemorySet(m_name, L'\0', CONTENT_MAX_NAME_LEN);
    winrt::check_hresult(CoCreateGuid(&m_guid));
}

qgl::content::CONTENT_METADATA_BUFFER::CONTENT_METADATA_BUFFER(
    RESOURCE_TYPES type,
    CONTENT_LOADER_IDS loaderID,
    const wchar_t* name,
    size_t nameLength) :
    m_flags1(DEFAULT_FLAGS),
    m_reserved1(0),
    m_reserved2(0),
    m_type(type),
    m_loaderID(loaderID),
    m_compilerVersion(qgl::QGL_VERSION_LATEST)
{
    if (nameLength <= CONTENT_MAX_NAME_LEN)
    {
        MemoryCopy(m_name, name, nameLength);
        m_name[nameLength] = L'\0';
    }
    else
    {
        throw std::domain_error("The name is too long.");
    }

    winrt::check_hresult(CoCreateGuid(&m_guid));
}

qgl::content::CONTENT_METADATA_BUFFER::CONTENT_METADATA_BUFFER(
    RESOURCE_TYPES type,
    CONTENT_LOADER_IDS loaderID,
    const winrt::hstring& name) :
    CONTENT_METADATA_BUFFER(type, loaderID, name.c_str(), name.size())
{

}

qgl::content::CONTENT_METADATA_BUFFER::CONTENT_METADATA_BUFFER(
    const CONTENT_METADATA_BUFFER& r) :
    m_flags1(r.m_flags1),
    m_reserved1(r.m_reserved1),
    m_reserved2(r.m_reserved2),
    m_type(r.m_type),
    m_loaderID(r.m_loaderID),
    m_compilerVersion(r.version())
{
    MemoryCopy<wchar_t>(m_name, r.m_name, CONTENT_MAX_NAME_LEN);
    MemoryCopy<GUID>(&m_guid, &r.m_guid, 1);
}

qgl::content::CONTENT_METADATA_BUFFER::CONTENT_METADATA_BUFFER(
    CONTENT_METADATA_BUFFER&& r) :
    m_flags1(r.m_flags1),
    m_reserved1(r.m_reserved1),
    m_reserved2(r.m_reserved2),
    m_type(r.m_type),
    m_loaderID(r.m_loaderID),
    m_compilerVersion(r.version())
{
    MemoryCopy<wchar_t>(m_name, r.m_name, CONTENT_MAX_NAME_LEN);
    MemoryCopy<GUID>(&m_guid, &r.m_guid, 1);
}

void qgl::content::CONTENT_METADATA_BUFFER::name(const winrt::hstring& n)
{
    auto len = n.size();
    if (len <= CONTENT_MAX_NAME_LEN)
    {
        MemoryCopy<wchar_t>(m_name, n.c_str(), len);
        m_name[len] = L'\0';
    }
    else
    {
        throw std::domain_error("The name is too long.");
    }
}

winrt::hstring qgl::content::CONTENT_METADATA_BUFFER::guid_str() const
{
    OLECHAR* guidStr = nullptr;
    auto hr = StringFromCLSID(m_guid, &guidStr);
    if (SUCCEEDED(hr))
    {
        winrt::hstring ret{ guidStr };
        CoTaskMemFree(guidStr);
        return ret;
    }
    else
    {
        CoTaskMemFree(guidStr);
        winrt::throw_hresult(hr);
    }
}

void qgl::content::CONTENT_METADATA_BUFFER::guid_str(const winrt::hstring& g)
{
    winrt::check_hresult(CLSIDFromString(g.c_str(), &m_guid));
}