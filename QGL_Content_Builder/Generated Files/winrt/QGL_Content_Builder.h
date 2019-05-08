﻿// WARNING: Please don't edit this file. It was generated by C++/WinRT v1.0.180821.2

#pragma once

#include "winrt/base.h"


static_assert(winrt::check_version(CPPWINRT_VERSION, "1.0.180821.2"), "Mismatched component and base headers.");
#include "winrt/Windows.Foundation.h"
#include "winrt/Windows.Foundation.Collections.h"
#include "winrt/impl/Windows.UI.Xaml.Interop.2.h"
#include "winrt/impl/Windows.UI.Xaml.Markup.2.h"
#include "winrt/impl/Windows.UI.Composition.2.h"
#include "winrt/impl/Windows.UI.Xaml.2.h"
#include "winrt/impl/Windows.UI.Xaml.Controls.2.h"
#include "winrt/impl/QGL_Content_Builder.2.h"

namespace winrt::impl {

template <typename D> int32_t consume_QGL_Content_Builder_IMainPage<D>::MyProperty() const
{
    int32_t value{};
    check_hresult(WINRT_SHIM(QGL_Content_Builder::IMainPage)->get_MyProperty(&value));
    return value;
}

template <typename D> void consume_QGL_Content_Builder_IMainPage<D>::MyProperty(int32_t value) const
{
    check_hresult(WINRT_SHIM(QGL_Content_Builder::IMainPage)->put_MyProperty(value));
}

template <typename D>
struct produce<D, QGL_Content_Builder::IMainPage> : produce_base<D, QGL_Content_Builder::IMainPage>
{
    int32_t WINRT_CALL get_MyProperty(int32_t* value) noexcept final
    {
        try
        {
            typename D::abi_guard guard(this->shim());
            WINRT_ASSERT_DECLARATION(MyProperty, WINRT_WRAP(int32_t));
            *value = detach_from<int32_t>(this->shim().MyProperty());
            return 0;
        }
        catch (...) { return to_hresult(); }
    }

    int32_t WINRT_CALL put_MyProperty(int32_t value) noexcept final
    {
        try
        {
            typename D::abi_guard guard(this->shim());
            WINRT_ASSERT_DECLARATION(MyProperty, WINRT_WRAP(void), int32_t);
            this->shim().MyProperty(value);
            return 0;
        }
        catch (...) { return to_hresult(); }
    }
};

}

WINRT_EXPORT namespace winrt::QGL_Content_Builder {

inline MainPage::MainPage() :
    MainPage(impl::call_factory<MainPage>([](auto&& f) { return f.template ActivateInstance<MainPage>(); }))
{}

inline XamlMetaDataProvider::XamlMetaDataProvider() :
    XamlMetaDataProvider(impl::call_factory<XamlMetaDataProvider>([](auto&& f) { return f.template ActivateInstance<XamlMetaDataProvider>(); }))
{}

}

namespace winrt::impl {

struct property_QGL_Content_Builder_IMainPage
{ struct named {
    struct MyProperty
    {
        struct name { static constexpr std::wstring_view value{ L"MyProperty"sv }; };
        using property_type = int32_t;
        using target_type = winrt::QGL_Content_Builder::IMainPage;

        using is_readable = std::true_type;
        using is_writable = std::true_type;
        using is_static = std::false_type;
        struct getter
        {
            auto operator()(target_type const& target) const
            {
                return target.MyProperty();
            }
        };
        struct setter
        {
            template <typename Value>
            void operator()(target_type const& target, Value&& value) const
            {
                target.MyProperty(std::forward<Value>(value));
            }
        };
    };};
    struct list { using type = impl::typelist<named::MyProperty>; };
};

struct property_QGL_Content_Builder_MainPage
{ struct named {
    struct MyProperty
    {
        struct name { static constexpr std::wstring_view value{ L"MyProperty"sv }; };
        using property_type = int32_t;
        using target_type = winrt::QGL_Content_Builder::MainPage;

        using is_readable = std::true_type;
        using is_writable = std::true_type;
        using is_static = std::false_type;
        struct getter
        {
            auto operator()(target_type const& target) const
            {
                return target.MyProperty();
            }
        };
        struct setter
        {
            template <typename Value>
            void operator()(target_type const& target, Value&& value) const
            {
                target.MyProperty(std::forward<Value>(value));
            }
        };
    };};
    struct list { using type = impl::typelist<named::MyProperty>; };
};

}

WINRT_EXPORT namespace winrt::experimental::reflect {
template <> struct named_property<QGL_Content_Builder::IMainPage> : impl::property_QGL_Content_Builder_IMainPage::named {};
template <> struct properties<QGL_Content_Builder::IMainPage> : impl::property_QGL_Content_Builder_IMainPage::list {};
template <> struct named_property<QGL_Content_Builder::MainPage> : impl::property_QGL_Content_Builder_MainPage::named {};
template <> struct properties<QGL_Content_Builder::MainPage> : impl::property_QGL_Content_Builder_MainPage::list {};

template <>
struct base_type<QGL_Content_Builder::MainPage> { using type = Windows::UI::Xaml::Controls::Page; };
}

WINRT_EXPORT namespace std {

template<> struct hash<winrt::QGL_Content_Builder::IMainPage> : winrt::impl::hash_base<winrt::QGL_Content_Builder::IMainPage> {};
template<> struct hash<winrt::QGL_Content_Builder::MainPage> : winrt::impl::hash_base<winrt::QGL_Content_Builder::MainPage> {};
template<> struct hash<winrt::QGL_Content_Builder::XamlMetaDataProvider> : winrt::impl::hash_base<winrt::QGL_Content_Builder::XamlMetaDataProvider> {};

}
