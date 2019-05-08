#include "pch.h"
#include "include/qgl_window.h"
#include <winrt/Windows.Graphics.Display.h>

/*
 Converts device-independent pixels to physical pixels.
 */
constexpr auto ConvertToPixels(float dip, float dpi)
{
   return static_cast<int>(dip * dpi / 96.0f + 0.5f);
}

qgl::graphics::window::window(const appview_pt& appView_p,
                              const corewin_pt& coreWindow_p) :
   m_appView_p(appView_p),
   m_coreWin_p(coreWindow_p)
{
   p_update_dimmensions();
   m_coreWin_p->SizeChanged({this, &window::p_resize_completed});
}

qgl::graphics::window::window(window && r) :
   m_appView_p(std::move(r.m_appView_p)),
   m_coreWin_p(std::move(r.m_coreWin_p))
{
   p_update_dimmensions();
   m_coreWin_p->SizeChanged({ this, &window::p_resize_completed });
}

bool qgl::graphics::window::full_screen() const
{
   return m_appView_p->IsFullScreenMode();
}

void qgl::graphics::window::toggle_full_screen()
{
   window_helpers::toggle_full_screen(*m_appView_p);
}

void qgl::graphics::window::enter_full_screen()
{
   window_helpers::enter_full_screen(*m_appView_p);
}

void qgl::graphics::window::exit_full_screen()
{
   window_helpers::exit_full_screen(*m_appView_p);
}

void qgl::graphics::window::title(const winrt::param::hstring str)
{
   m_appView_p->Title(str);
}

winrt::hstring qgl::graphics::window::title() const
{
   return m_appView_p->Title();
}

IUnknown* qgl::graphics::window::unknown()
{
   return winrt::get_unknown(*m_coreWin_p);
}

void qgl::graphics::window::p_update_dimmensions()
{
   //Get the window bounds which are measured in device-independent pixels.
   const auto& bnds = m_coreWin_p->Bounds();
   
   //Get the DPI of the screen.
   auto dpInfo = winrt::Windows::Graphics::Display::DisplayInformation::GetForCurrentView();
   float dpi = dpInfo.LogicalDpi();

   //Convert the device-independent pixels into physical pixels.
   auto h = static_cast<UINT>(ConvertToPixels(bnds.Height, dpi));
   auto w = static_cast<UINT>(ConvertToPixels(bnds.Width, dpi));
   m_width = w;
   m_height = h;

   #ifdef DEBUG
   std::wstringstream ws;
   ws << "H: " << h << " W: " << w;
   title(ws.str());
   #endif;
}
