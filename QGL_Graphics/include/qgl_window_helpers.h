#pragma once
#include "qgl_graphics_include.h"

namespace qgl::graphics::window_helpers
{
   extern LIB_EXPORT void window_title(
      winrt::Windows::UI::ViewManagement::ApplicationView& appView,
      const winrt::param::hstring& title);

   extern LIB_EXPORT void window_title(
      winrt::Windows::UI::ViewManagement::ApplicationView& appView,
      const std::wstring& title);

   extern LIB_EXPORT void enter_full_screen(
      winrt::Windows::UI::ViewManagement::ApplicationView& appView);

   extern LIB_EXPORT void exit_full_screen(
      winrt::Windows::UI::ViewManagement::ApplicationView& appView);

   extern LIB_EXPORT void toggle_full_screen(
      winrt::Windows::UI::ViewManagement::ApplicationView& appView);

   /*
    Add this in the future?
    */
   //void set_window_title(HWND window, const std::wstring& title);
}