#pragma once
#include "include/qgl_graphics_include.h"

namespace qgl::graphics
{
   /*
    Converts device-independent pixels to physical pixels.
    */
   constexpr float dip_to_pixels(float dip, float dpi)
   {
      return dip * dpi / 96.0f + 0.5f;
   }

   /*
    Gets the window's dimmension in physical pixels.
    Returns a pair where the first item is the height and the second is the
    width.
    */
   static std::pair<float, float> window_dimmensions(
      const winrt::Windows::UI::Core::CoreWindow* wnd)
   {
      //Get the window bounds which are measured in device-independent 
      //pixels.
      const auto& bounds = wnd->Bounds();

      //Get the DPI of the screen.
      auto dpInfo = winrt::Windows::Graphics::Display::DisplayInformation::
         GetForCurrentView();
      auto dpi = dpInfo.LogicalDpi();

      //Convert the device-independent pixels into physical pixels.
      auto h = dip_to_pixels(bounds.Height, dpi);
      auto w = dip_to_pixels(bounds.Width, dpi);

      return std::make_pair(w, h);
   }

   /*
    Gets the window's dimmension in physical pixels.
    Returns a pair where the first item is the height and the second is the
    width.
    */
   static std::pair<float, float> window_dimmensions(
      winrt::Windows::UI::Core::WindowSizeChangedEventArgs const& args)
   {
      //Get the DPI of the screen.
      auto dpInfo = winrt::Windows::Graphics::Display::DisplayInformation::
         GetForCurrentView();
      auto dpi = dpInfo.LogicalDpi();

      //Convert the device-independent pixels into physical pixels.
      return std::make_pair(
         dip_to_pixels(args.Size().Height, dpi),
         dip_to_pixels(args.Size().Width, dpi));
   }
}