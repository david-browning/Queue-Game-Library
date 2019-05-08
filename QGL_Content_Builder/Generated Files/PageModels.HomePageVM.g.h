﻿// WARNING: Please don't edit this file. It was generated by C++/WinRT v1.0.180821.2

#pragma once

#include "winrt/QGL_Content_Builder.Extensions.h"
#include "winrt/QGL_Projection.h"
#include "winrt/Windows.UI.Xaml.Data.h"
#include "winrt/QGL_Content_Builder.PageModels.h"

namespace winrt::QGL_Content_Builder::PageModels::implementation {

template <typename D, typename... I>
struct WINRT_EBO HomePageVM_base : implements<D, QGL_Content_Builder::PageModels::IHomePageVM, Windows::UI::Xaml::Data::INotifyPropertyChanged, I...>
{
    using base_type = HomePageVM_base;
    using class_type = QGL_Content_Builder::PageModels::HomePageVM;
    using implements_type = typename HomePageVM_base::implements_type;
    using implements_type::implements_type;
    
#if _MSC_VER < 1914
    operator class_type() const noexcept
    {
        static_assert(std::is_same_v<typename impl::implements_default_interface<D>::type, default_interface<class_type>>);
        class_type result{ nullptr };
        attach_abi(result, detach_abi(static_cast<default_interface<class_type>>(*this)));
        return result;
    }
#else
    operator impl::producer_ref<class_type> const() const noexcept
    {
        return { to_abi<default_interface<class_type>>(this) };
    }
#endif

    hstring GetRuntimeClassName() const
    {
        return L"QGL_Content_Builder.PageModels.HomePageVM";
    }
};

}

namespace winrt::QGL_Content_Builder::PageModels::factory_implementation {

template <typename D, typename T, typename... I>
struct WINRT_EBO HomePageVMT : implements<D, Windows::Foundation::IActivationFactory, QGL_Content_Builder::PageModels::IHomePageVMFactory, I...>
{
    using instance_type = QGL_Content_Builder::PageModels::HomePageVM;

    hstring GetRuntimeClassName() const
    {
        return L"QGL_Content_Builder.PageModels.HomePageVM";
    }

    Windows::Foundation::IInspectable ActivateInstance() const
    {
        throw hresult_not_implemented();
    }

    QGL_Content_Builder::PageModels::HomePageVM CreateInstance(QGL_Content_Builder::Extensions::ExtensionProvider const& extensionProvider)
    {
        return make<T>(extensionProvider);
    }
};

}

#if defined(WINRT_FORCE_INCLUDE_HOMEPAGEVM_XAML_G_H) || __has_include("PageModels.HomePageVM.xaml.g.h")

#include "PageModels.HomePageVM.xaml.g.h"

#else

namespace winrt::QGL_Content_Builder::PageModels::implementation
{
    template <typename D, typename... I>
    using HomePageVMT = HomePageVM_base<D, I...>;
}

#endif
