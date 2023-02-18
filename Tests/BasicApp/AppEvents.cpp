#include "pch.h"
#include "App.h"

using namespace winrt;
using namespace winrt::Windows;
using namespace winrt::Windows::ApplicationModel::Core;
using namespace winrt::Windows::Foundation::Numerics;
using namespace winrt::Windows::UI;
using namespace winrt::Windows::UI::Core;
using namespace winrt::Windows::UI::Composition;
using namespace winrt::Windows::UI::ViewManagement;
using namespace winrt::Windows::ApplicationModel;

winrt::fire_and_forget App::Suspending(IInspectable const&,
                                       SuspendingEventArgs const& args)
{
   auto lifetime = get_strong();

   // https://learn.microsoft.com/en-us/windows/uwp/gaming/how-to-suspend-an-app-directx-and-cpp
   auto def = args.SuspendingOperation().GetDeferral();

   co_await winrt::resume_background();
   qdev_p->suspend();
   def.Complete();
}

void App::Resuming(IInspectable const&, IInspectable const&)
{
   // https://learn.microsoft.com/en-us/windows/uwp/gaming/how-to-resume-an-app-directx-and-cpp
   return;
}

void App::OnPointerPressed(IInspectable const&, PointerEventArgs const& args)
{
   float2 const point = args.CurrentPoint().Position();
   return;
}

void App::OnPointerMoved(IInspectable const&, PointerEventArgs const&)
{

}

void App::OnWindowClosed(CoreWindow const&, CoreWindowEventArgs const&)
{
}