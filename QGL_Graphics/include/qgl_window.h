#pragma once
#include "qgl_graphics_include.h"
#include "qgl_window_helpers.h"
#include <winrt/Windows.Foundation.h>

namespace qgl::graphics
{
   //Notify: width, height changed.
   //Notify: full screen changed.
   class LIB_EXPORT window
   {
      public:
      using appview_pt = std::shared_ptr<winrt::Windows::UI::ViewManagement::ApplicationView>;
      using corewin_pt = std::shared_ptr<winrt::Windows::UI::Core::CoreWindow>;

      window(const appview_pt& appView_p,
             const corewin_pt& coreWindow_p);

      /*
       Do not allow copies of a window.
       */
      window(const window& r) = delete;

      window(window&& r);

      virtual ~window() = default;

      bool full_screen() const;

      void toggle_full_screen();

      void enter_full_screen();

      void exit_full_screen();

      void title(const winrt::param::hstring str);

      winrt::hstring title() const;

      IUnknown* unknown();

      inline auto width() const
      {
         return m_width;
      }

      inline auto height() const
      {
         return m_height;
      }

      private:
      void p_update_dimmensions();

      inline void p_resize_completed(winrt::Windows::Foundation::IInspectable const&,
                                     winrt::Windows::UI::Core::WindowSizeChangedEventArgs const&)
      {
         p_update_dimmensions();
      }

      #pragma warning(push)
      #pragma warning(disable: 4251)
      appview_pt m_appView_p;
      corewin_pt m_coreWin_p;
      #pragma warning(pop)

      UINT m_width;
      UINT m_height;
   };
}