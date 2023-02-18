#pragma once
#include "pch.h"

using namespace winrt;
using namespace winrt::Windows;
using namespace winrt::Windows::ApplicationModel::Core;
using namespace winrt::Windows::Foundation::Numerics;
using namespace winrt::Windows::UI;
using namespace winrt::Windows::UI::Core;
using namespace winrt::Windows::UI::Composition;
using namespace winrt::Windows::UI::ViewManagement;
using namespace winrt::Windows::ApplicationModel;

struct App : implements<App, IFrameworkViewSource, IFrameworkView>
{
   IFrameworkView CreateView();

   void Initialize(CoreApplicationView const& appView);

   void SetWindow(CoreWindow const& window);

   void Load(hstring const&);

   void Uninitialize();

   void Run();

   void Activated(IInspectable const&, Activation::IActivatedEventArgs const&);

   winrt::fire_and_forget Suspending(IInspectable const&,
                                     SuspendingEventArgs const& args);

   void Resuming(IInspectable const&, IInspectable const&);

   void OnPointerPressed(IInspectable const&, PointerEventArgs const& args);

   void OnPointerMoved(IInspectable const&, PointerEventArgs const&);

   void OnWindowClosed(CoreWindow const&, CoreWindowEventArgs const&);

   private:
   bool m_wndClosed = false;
   bool m_wndVisible = true;
   qgl::basic_console<char> m_console;
   std::unique_ptr<qgl::graphics::graphics_device> qdev_p;
};