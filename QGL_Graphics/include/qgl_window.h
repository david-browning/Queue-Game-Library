#pragma once
#include "include/qgl_graphics_include.h"
#include "include/Helpers/qgl_window_helpers.h"

namespace qgl::graphics
{
   class window
   {
      public:
      using resize_callback = typename std::function<void(const window*)>;
      using dpi_changed_callback = typename resize_callback;
      using callback_handle = typename uintptr_t;

      using corewnd_ptr = typename std::shared_ptr<
         winrt::Windows::UI::Core::CoreWindow>;
      using view_ptr = typename std::shared_ptr<
         winrt::Windows::UI::ViewManagement::ApplicationView>;

      /*
       Constructs a window using a CoreWindow and ApplicationView.
       Pointers are forwarded in case they are expected to be moved.
       */
      window(corewnd_ptr&& coreWnd_p, view_ptr&& view_p) :
         m_wnd_p(std::forward<corewnd_ptr>(coreWnd_p)),
         m_view_p(std::forward<view_ptr>(view_p)),
         m_nextCallbackHandle(0)
      {
         using namespace winrt::Windows::Graphics;

         m_widthDIP = m_wnd_p->Bounds().Width;
         m_heightDIP = m_wnd_p->Bounds().Height;

         auto dpInfo = Display::DisplayInformation::GetForCurrentView();
         m_logicalDPIX = dpInfo.RawDpiX();
         m_logicalDPIY = dpInfo.RawDpiY();

         // Hook up events.
         m_resizeToken = m_wnd_p->SizeChanged(
            { this, &window::resize_completed });
         m_dpiToken = dpInfo.DpiChanged({ this, &window::dpi_changed });
      }

      /*
       Do not allow copies of a window.
       */
      window(const window& r) = delete;

      /*
       Move constructor.
       */
      window(window&& r) noexcept :
         m_resizeCallbacks(std::move(r.m_resizeCallbacks)),
         m_dpiCallbacks(std::move(r.m_dpiCallbacks)),
         m_wnd_p(std::move(r.m_wnd_p)),
         m_view_p(std::move(r.m_view_p)),
         m_logicalDPIX(std::move(r.m_logicalDPIX)),
         m_logicalDPIY(std::move(r.m_logicalDPIY)),
         m_widthDIP(std::move(r.m_widthDIP)),
         m_heightDIP(std::move(r.m_heightDIP)),
         m_nextCallbackHandle(r.m_nextCallbackHandle.load())
      {
         using namespace winrt::Windows::Graphics;
         auto dpInfo = Display::DisplayInformation::GetForCurrentView();
         // Hook up events.
         m_resizeToken = m_wnd_p->SizeChanged(
            { this, &window::resize_completed });
         m_dpiToken = dpInfo.DpiChanged({ this, &window::dpi_changed });
      }

      /*
       Destructor
       */
      ~window() noexcept
      {
         // Unhook the events.
         using namespace winrt::Windows::Graphics;
         m_wnd_p->ResizeCompleted(m_resizeToken);
         auto dpInfo = Display::DisplayInformation::GetForCurrentView();
         dpInfo.DpiChanged(m_dpiToken);
      }

      /*
       Returns true if the window is in full screen.
       */
      bool full_screen() const
      {
         return m_view_p->IsFullScreen();
      }

      /*
       Toggles the window's mode. If the window is full screen, calling this
       will set the window to windowed mode. If the window is windowed, calling
       this will set the window to full screen.
       Returns true if the window is full screen after calling this.
       Returns false if the window is windowed after calling this.
       */
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

      /*
       Enters full screen mode on a windowed app.
       Returns true if the windows is in full screen after calling this.
       */
      bool enter_full_screen()
      {
         return m_view_p->TryEnterFullScreenMode();
      }

      /*
       Exits full screen.
       Returns false if this fails and the window is still in full screen mode.
       */
      bool exit_full_screen()
      {
         m_view_p->ExitFullScreenMode();
         return true;
      }

      /*
       Sets the window's title.
       */
      void title(const winrt::param::hstring str)
      {
         m_view_p->Title(str);
      }

      /*
       Returns the window title.
       */
      winrt::hstring title() const
      {
         return m_view_p->Title();
      }

      /*
       Returns the window's aspect ratio.
       It is the width / height.
       */
      float ratio() const noexcept
      {
         return m_widthDIP / m_heightDIP;
      }

      /*
       Returns the window width, in device independent pixels.
       */
      dip_t width() const noexcept
      {
         return m_widthDIP;
      }

      /*
       Returns the window height, in device independent pixels.
       */
      dip_t height() const noexcept
      {
         return m_heightDIP;
      }

      /*
       Returns the window Y axis, in device independent pixels.
       */
      dip_t top() const noexcept
      {
         return m_wnd_p->Bounds().Y;
      }

      /*
       Returns the window X axis, in device independent pixels.
       */
      dip_t left() const noexcept
      {
         return m_wnd_p->Bounds().X;
      }

      dpi_t dpi_x() const noexcept
      {
         return m_logicalDPIX;
      }

      dpi_t dpi_y() const noexcept
      {
         return m_logicalDPIY;
      }

      ///*
      // Returns the window width, in physical pixels.
      // */
      //physp_t width_phys() const noexcept
      //{
      //   return helpers::dip_to_pixels(m_widthDIP, m_logicalDPI);
      //}

      ///*
      // Returns the window height, in physical pixels.
      // */
      //physp_t height_phys() const noexcept
      //{
      //   return helpers::dip_to_pixels(m_heightDIP, m_logicalDPI);
      //}

      ///*
      // Returns the window Y axis, in physical pixels.
      // */
      //physp_t top_phys() const noexcept
      //{
      //   return helpers::dip_to_pixels(m_wnd_p->Bounds().Y, m_logicalDPI);
      //}

      ///*
      // Returns the window X axis, in physical pixels.
      // */
      //physp_t left_phys() const noexcept
      //{
      //   return helpers::dip_to_pixels(m_wnd_p->Bounds().X, m_logicalDPI);
      //}

      auto rt_window()
      {
         return m_wnd_p.get();
      }

      /*
       Returns an IUnknown pointer to the core window.
       */
      IUnknown* unknown()
      {
         return winrt::get_unknown(*m_wnd_p);
      }

      math::window_boundary<dip_t> boundary() const noexcept
      {
         return math::window_boundary<dip_t>{
            left(), top(), left() + width(), top() + height()
         };
      }

      //math::window_boundary<physp_t> boundary_physical() const noexcept
      //{
      //   return math::window_boundary<physp_t>{
      //      left_phys(), top_phys(),
      //      left_phys() + width_phys(), top_phys() + height_phys()
      //   };
      //}

      callback_handle register_resize_callback(resize_callback clb)
      {
         auto ret = m_nextCallbackHandle.fetch_add(
               1, std::memory_order::memory_order_relaxed);
         m_resizeCallbacks[ret] = clb;
         return ret;
      }

      void unregister_resize_callback(callback_handle clb)
      {
         if (m_resizeCallbacks.count(clb) > 0)
         {
            m_resizeCallbacks.erase(clb);
         }
      }

      callback_handle register_dpi_changed_callback(dpi_changed_callback clb)
      {
         auto ret = m_nextCallbackHandle.fetch_add(
              1, std::memory_order::memory_order_relaxed);
         m_dpiCallbacks[ret] = clb;
         return ret;
      }

      void unregister_dpi_changed_callback(callback_handle clb)
      {
         if (m_resizeCallbacks.count(clb) > 0)
         {
            m_resizeCallbacks.erase(clb);
         }
      }

      private:
      void resize_completed(
        winrt::Windows::Foundation::IInspectable const&,
        winrt::Windows::UI::Core::WindowSizeChangedEventArgs const& args)
      {
         m_widthDIP = args.Size().Width;
         m_heightDIP = args.Size().Height;

         for (const auto& clb : m_resizeCallbacks)
         {
            clb.second(this);
         }
      }

      void dpi_changed(
         winrt::Windows::Graphics::Display::DisplayInformation const& args,
         winrt::Windows::Foundation::IInspectable const&
         )
      {
         m_logicalDPIX = args.RawDpiX();
         m_logicalDPIY = args.RawDpiY();
         for (const auto& clb : m_dpiCallbacks)
         {
            clb.second(this);
         }
      }

      std::atomic<callback_handle> m_nextCallbackHandle;
      std::unordered_map<callback_handle, resize_callback> m_resizeCallbacks;
      std::unordered_map<callback_handle, dpi_changed_callback> m_dpiCallbacks;

      corewnd_ptr m_wnd_p;
      view_ptr m_view_p;

      dpi_t m_logicalDPIX;
      dpi_t m_logicalDPIY;
      dip_t m_widthDIP;
      dip_t m_heightDIP;

      winrt::event_token m_resizeToken;
      winrt::event_token m_dpiToken;
   };
}