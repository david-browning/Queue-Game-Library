#include "pch.h"
#include "include/qgl_window.h"
#include <winrt/Windows.Graphics.Display.h>

using namespace winrt::Windows::UI;

namespace qgl::graphics
{
   /*
    Converts device-independent pixels to physical pixels.
    */
   constexpr UINT ConvertToPixels(float dip, float dpi)
   {
      return static_cast<UINT>(dip * dpi / 96.0f + 0.5f);
   }

   struct iwindow::impl
   {
      public:

      impl(std::shared_ptr<ViewManagement::ApplicationView> appView,
           std::shared_ptr<Core::CoreWindow> coreWin) :
         m_appView(appView),
         m_coreWindow(coreWin)
      {
         update_dimmensions();
         m_coreWindow->SizeChanged({ this, &impl::resize_completed });
      }

      impl(impl&) = delete;

      impl(impl&& m) = default;

      virtual ~impl() noexcept = default;

      bool full_screen() const
      {
         return m_appView->IsFullScreen();
      }

      bool toggle_full_screen()
      {
         if (full_screen())
         {
            m_appView->ExitFullScreenMode();
            return false;
         }
         else
         {
            return m_appView->TryEnterFullScreenMode();
         }
      }

      bool enter_full_screen()
      {
         return m_appView->TryEnterFullScreenMode();
      }

      bool exit_full_screen()
      {
         m_appView->ExitFullScreenMode();
         return true;
      }

      void title(const winrt::param::hstring str)
      {
         m_appView->Title(str);
      }

      winrt::hstring title() const
      {
         return m_appView->Title();
      }

      IUnknown* unknown()
      {
         return winrt::get_unknown(*m_coreWindow);
      }

      UINT m_widthPixels;
      UINT m_heightPixels;

      private:

      void update_dimmensions()
      {
          //Get the window bounds which are measured in device-independent pixels.
         const auto& bnds = m_coreWindow->Bounds();

         //Get the DPI of the screen.
         auto dpInfo = winrt::Windows::Graphics::Display::DisplayInformation::GetForCurrentView();
         float dpi = dpInfo.LogicalDpi();

         //Convert the device-independent pixels into physical pixels.
         auto h = ConvertToPixels(bnds.Height, dpi);
         auto w = ConvertToPixels(bnds.Width, dpi);
         m_widthPixels = w;
         m_heightPixels = h;

         #ifdef _DEBUG
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

      std::shared_ptr<ViewManagement::ApplicationView> m_appView;
      std::shared_ptr<Core::CoreWindow> m_coreWindow;
   };


   iwindow::iwindow(winrt::Windows::Foundation::IInspectable coreWindow,
                  winrt::Windows::Foundation::IInspectable appView)
   {
      auto win = std::make_shared<Core::CoreWindow>(
         winrt::unbox_value<Core::CoreWindow>(coreWindow));
      
      auto vew = std::make_shared<ViewManagement::ApplicationView>(
         winrt::unbox_value<ViewManagement::ApplicationView>(appView));

      m_impl_p = new impl(vew, win);
   }

   iwindow::iwindow(iwindow&& r)
   {
      m_impl_p = r.m_impl_p;
      r.m_impl_p = nullptr;
   }

   iwindow::~iwindow() noexcept
   {
      delete m_impl_p;
      m_impl_p = nullptr;
   }

   bool iwindow::full_screen() const
   {
      return m_impl_p->full_screen();
   }

   bool iwindow::toggle_full_screen()
   {
      return m_impl_p->toggle_full_screen();
   }

   bool iwindow::enter_full_screen()
   {
      return m_impl_p->enter_full_screen();
   }

   bool iwindow::exit_full_screen()
   {
      return m_impl_p->exit_full_screen();
   }

   void iwindow::title(const winrt::param::hstring str)
   {
      m_impl_p->title(winrt::to_hstring(str));
   }

   winrt::hstring iwindow::title() const
   {
      return m_impl_p->title();
   }

   UINT iwindow::width() const noexcept
   {
      return m_impl_p->m_widthPixels;
   }

   UINT iwindow::height() const noexcept
   {
      return m_impl_p->m_heightPixels;
   }

   IUnknown* iwindow::unknown()
   {
      return m_impl_p->unknown();
   }
}