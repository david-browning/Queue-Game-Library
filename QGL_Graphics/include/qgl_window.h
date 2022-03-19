#pragma once
#include "include/qgl_graphics_include.h"
#include "include/Helpers/qgl_window_helpers.h"
#include "include/Interfaces/qgl_iwindow.h"

namespace qgl::graphics
{
   class window
   {
      public:
      using corewnd_ptr = typename std::shared_ptr<
         winrt::Windows::UI::Core::CoreWindow>;
      using view_ptr = typename std::shared_ptr<
         winrt::Windows::UI::ViewManagement::ApplicationView>;
      
      /*
       Constructs a window using a CoreWindow and ApplicationView.
       Pointers are forwarded in case they are expected to be moved.
       */
      window(
         corewnd_ptr&& coreWnd_p,
         view_ptr&& view_p) :
         m_wnd_p(std::forward<corewnd_ptr>(coreWnd_p)),
         m_view_p(std::forward<view_ptr>(view_p))
      {
         iwindow* i = nullptr;
         winrt::check_hresult(make_window(m_wnd_p.get(), m_view_p.get(), &i));
         m_wndImpl = qgl::make_unique<iwindow>(i);
      }

      /*
       Do not allow copies of a window.
       */
      window(const window& r) = delete;

      /*
       Move constructor.
       */
      window(window&& r) = default;

      /*
       Destructor
       */
      ~window() noexcept = default;

      /*
       Returns true if the window is in full screen.
       */
      bool full_screen() const
      {
         return m_wndImpl->full_screen();
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
         return m_wndImpl->enter_full_screen();
      }

      /*
       Exits full screen.
       Returns false if this fails and the window is still in full screen mode.
       */
      bool exit_full_screen()
      {
         return m_wndImpl->exit_full_screen();
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
         return m_wndImpl->ratio();
      }

      template<typename T>
      T width() const noexcept
      {
         return static_cast<T>(m_wndImpl->width());
      }

      template<typename T>
      T height() const noexcept
      {
         return static_cast<T>(m_wndImpl->height());
      }

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

      iwindow* iwnd()
      {
         return m_wndImpl.get();
      }

      private:

      /*
       Cannot pass smart pointers across the ABI. So save them into smart 
       pointers that will be released when this is released and pass their 
       "raw" pointers to the window implementation.
       */
      corewnd_ptr m_wnd_p;
      view_ptr m_view_p;
      qgl_unique_ptr<iwindow> m_wndImpl;
   };
}