#include "pch.h"
#include "include/qgl_window.h"
#include <winrt/Windows.Graphics.Display.h>

using namespace winrt::Windows::UI;
using namespace winrt::qgl::graphics;
using namespace winrt::Windows::Graphics;

namespace qgl::graphics
{
   /*
    Converts device-independent pixels to physical pixels.
    */
   constexpr UINT ConvertToPixels(float dip, float dpi)
   {
      return static_cast<UINT>(dip * dpi / 96.0f + 0.5f);
   }

   struct window::impl
   {
      public:
      impl(const winrt::qgl::graphics::window_rt& wnd) :
         m_wnd(wnd) //Create a copy of the projected reference.
      {
         auto wndImpl = winrt::get_self<implementation::window_rt>(m_wnd);

         m_wnd_p = wndImpl->core_ptr();
         m_view_p = wndImpl->view_prt();

         update_dimmensions();
         m_wnd_p->SizeChanged({ this, &impl::resize_completed });
      }

      impl(impl&) = delete;

      impl(impl&& m) = delete;

      ~impl() noexcept = default;

      bool full_screen() const
      {
         return m_view_p->IsFullScreen();
      }

      bool toggle_full_screen()
      {
         if (full_screen())
         {
            m_view_p->ExitFullScreenMode();
            return false;
         }
         else
         {
            return m_view_p->TryEnterFullScreenMode();
         }
      }

      bool enter_full_screen()
      {
         return m_view_p->TryEnterFullScreenMode();
      }

      bool exit_full_screen()
      {
         m_view_p->ExitFullScreenMode();
         return true;
      }

      void title(const winrt::param::hstring str)
      {
         m_view_p->Title(str);
      }

      winrt::hstring title() const
      {
         return m_view_p->Title();
      }

      IUnknown* unknown()
      {
         return winrt::get_unknown(*m_wnd_p);
      }

      UINT WidthPixels;
      UINT HeightPixels;

      private:
      void update_dimmensions()
      {
          //Get the window bounds which are measured in device-independent 
         //pixels.
         const auto& bnds = m_wnd_p->Bounds();

         //Get the DPI of the screen.
         auto dpInfo = Display::DisplayInformation::GetForCurrentView();
         float dpi = dpInfo.LogicalDpi();

         //Convert the device-independent pixels into physical pixels.
         auto h = ConvertToPixels(bnds.Height, dpi);
         auto w = ConvertToPixels(bnds.Width, dpi);
         WidthPixels = w;
         HeightPixels = h;

         #ifdef DEBUG
         std::wstringstream ws;
         ws << "H: " << h << " W: " << w;
         title(ws.str());
         #endif;
      }

      void resize_completed(
         winrt::Windows::Foundation::IInspectable const&,
         winrt::Windows::UI::Core::WindowSizeChangedEventArgs const&)
      {
         update_dimmensions();
      }

      winrt::qgl::graphics::window_rt m_wnd;
      Core::CoreWindow* m_wnd_p;
      ViewManagement::ApplicationView* m_view_p;
   };


   window::window(const winrt::qgl::graphics::window_rt& wnd)
   {
      m_impl_p = new impl(wnd);
   }

   window::window(window&& r)
   {
      m_impl_p = r.m_impl_p;
      r.m_impl_p = nullptr;
   }

   window::~window() noexcept
   {
      delete m_impl_p;
      m_impl_p = nullptr;
   }

   bool window::full_screen() const
   {
      return m_impl_p->full_screen();
   }

   bool window::toggle_full_screen()
   {
      return m_impl_p->toggle_full_screen();
   }

   bool window::enter_full_screen()
   {
      return m_impl_p->enter_full_screen();
   }

   bool window::exit_full_screen()
   {
      return m_impl_p->exit_full_screen();
   }

   void window::title(const winrt::param::hstring str)
   {
      m_impl_p->title(winrt::to_hstring(str));
   }

   winrt::hstring window::title() const
   {
      return m_impl_p->title();
   }

   UINT window::width() const noexcept
   {
      return m_impl_p->WidthPixels;
   }

   UINT window::height() const noexcept
   {
      return m_impl_p->HeightPixels;
   }

   float window::ratio() const noexcept
   {
      return static_cast<float>(m_impl_p->WidthPixels) /
         static_cast<float>(m_impl_p->HeightPixels);
   }

   IUnknown* window::unknown()
   {
      return m_impl_p->unknown();
   }
}