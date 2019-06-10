#pragma once
#include "qgl.graphics.window_rt.g.h"
using namespace winrt::Windows::UI;

namespace winrt::qgl::graphics::implementation
{
   struct window_rt : window_rtT<window_rt>
   {
      window_rt() = default;

      window_rt(Core::CoreWindow const& wnd,
                ViewManagement::ApplicationView const& appView);


      Core::CoreWindow* core_ptr();
      ViewManagement::ApplicationView* view_prt();

      private:
      Core::CoreWindow m_wnd;
      ViewManagement::ApplicationView m_appView;
   };
}
namespace winrt::qgl::graphics::factory_implementation
{
   struct window_rt : window_rtT<window_rt, implementation::window_rt>
   {
   };
}
