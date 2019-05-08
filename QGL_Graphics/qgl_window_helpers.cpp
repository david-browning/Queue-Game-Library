#include "pch.h"
#include "include/qgl_window_helpers.h"

void qgl::graphics::window_helpers::window_title(winrt::Windows::UI::ViewManagement::ApplicationView& appView,
                                                const winrt::param::hstring& title)
{
   appView.Title(title);
}

void qgl::graphics::window_helpers::window_title(winrt::Windows::UI::ViewManagement::ApplicationView& appView,
                                                const std::wstring& title)
{
   appView.Title(title);
}

void qgl::graphics::window_helpers::enter_full_screen(winrt::Windows::UI::ViewManagement::ApplicationView& appView)
{
   appView.TryEnterFullScreenMode();
}

void qgl::graphics::window_helpers::exit_full_screen(winrt::Windows::UI::ViewManagement::ApplicationView& appView)
{
   appView.ExitFullScreenMode();
}

void qgl::graphics::window_helpers::toggle_full_screen(winrt::Windows::UI::ViewManagement::ApplicationView& appView)
{
   if (appView.IsFullScreenMode())
   {
      exit_full_screen(appView);
   }
   else
   {
      enter_full_screen(appView);
   }
}
