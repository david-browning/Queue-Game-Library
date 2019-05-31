#pragma once
#include "include/qgl_graphics_include.h"
#include <winrt/Windows.Foundation.h>

namespace qgl::graphics
{
   class QGL_GRAPHICS_API window
   {
      public:
      /*
       Constructs a window using a CoreWindow and ApplicationView.
       */
      window(winrt::Windows::Foundation::IInspectable coreWindow,
             winrt::Windows::Foundation::IInspectable appView);

      /*
       Do not allow copies of a window.
       */
      window(const window& r) = delete;

      /*
       Move constructor.
       */
      window(window&& r);

      /*
       Destructor
       */
      virtual ~window() noexcept;

      /*
       Returns true if the window is in full screen.
       */
      bool full_screen() const;

      /*
       Toggles the window's mode. If the window is full screen, calling this
       will set the window to windowed mode. If the window is windowed, calling
       this will set the window to full screen.
       Returns true if the window is full screen after calling this.
       Returns false if the window is windowed after calling this.
       */
      bool toggle_full_screen();

      /*
       Enters full screen mode on a windowed app.
       Returns true if the windows is in full screen after calling this.
       */
      bool enter_full_screen();

      /*
       Exits full screen.
       Returns false if this fails and the window is still in full screen mode.
       */
      bool exit_full_screen();

      /*
       Sets the window's title.
       */
      void title(const winrt::param::hstring str);

      /*
       Returns the window title.
       */
      winrt::hstring title() const;

      /*
       Returns the width of the window in pixels.
       */
      UINT width() const noexcept;

      /*
       Returns the height of the window in pixels.
       */
      UINT height() const noexcept;

      private:
      friend class graphics_device;
      /*
         Returns an IUnknown pointer to the core window.
       */
      IUnknown* unknown();

      struct impl;
      impl* m_impl_p;
   };
}