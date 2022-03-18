#pragma once
#include "include/qgl_graphics_include.h"

namespace qgl::graphics
{
   QGL_INTERFACE iwindow : public iqgl
   {
      public:
      virtual UINT width() const noexcept = 0;
      virtual UINT height() const noexcept = 0;
      virtual float ratio() const noexcept = 0;
      virtual bool full_screen() const = 0;
      virtual bool enter_full_screen() = 0;
      virtual bool exit_full_screen() = 0;
      virtual IUnknown* unknown() const noexcept = 0;
   };

   extern "C" QGL_GRAPHICS_API HRESULT make_window(
      winrt::Windows::UI::Core::CoreWindow* const,
      winrt::Windows::UI::ViewManagement::ApplicationView* const,
      iwindow**);
}