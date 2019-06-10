#include "pch.h"
#include "qgl.graphics.window_rt.h"


namespace winrt::qgl::graphics::implementation
{
   window_rt::window_rt(Core::CoreWindow const& wnd,
                        ViewManagement::ApplicationView const& appView) :
      m_wnd(wnd),
      m_appView(appView)
   {
   }

   Core::CoreWindow* window_rt::core_ptr()
   {
      return &m_wnd;
   }

   ViewManagement::ApplicationView* window_rt::view_prt()
   {
      return &m_appView;
   }
}
