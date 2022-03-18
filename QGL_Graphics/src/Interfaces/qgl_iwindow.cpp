#include "pch.h"
#include "include/Interfaces/qgl_iwindow.h"
#include "include/Helpers/qgl_window_helpers.h"

using namespace winrt::Windows::UI;

namespace qgl::graphics
{
   struct window_0_1 : public iwindow
   {
      window_0_1(
         Core::CoreWindow* const wnd_p,
         ViewManagement::ApplicationView* const view_p) :
         m_wnd_p(wnd_p),
         m_view_p(view_p)
      {
         auto dims = window_dimmensions(m_wnd_p);
         m_hPixels = dims.first;
         m_wPixels = dims.second;

         m_wnd_p->SizeChanged({ this, &window_0_1::resize_completed });
      }

      window_0_1(const window_0_1&) = delete;

      window_0_1(window_0_1&&) = default;

      virtual void release()
      {
         delete this;
      }

      virtual ~window_0_1() = default;

      virtual UINT width() const noexcept
      {
         return static_cast<UINT>(m_wPixels);
      }

      virtual UINT height() const noexcept
      {
         return static_cast<UINT>(m_hPixels);
      }

      virtual float ratio() const noexcept
      {
         return static_cast<float>(width()) / static_cast<float>(height());
      }

      virtual bool full_screen() const
      {
         return m_view_p->IsFullScreen();
      }

      virtual bool enter_full_screen()
      {
         return m_view_p->TryEnterFullScreenMode();
      }

      virtual bool exit_full_screen()
      {
         m_view_p->ExitFullScreenMode();
         return true;
      }

      virtual IUnknown* unknown() const noexcept
      {
         return winrt::get_unknown(*m_wnd_p);
      }

      private:
      void resize_completed(
         winrt::Windows::Foundation::IInspectable const&,
         winrt::Windows::UI::Core::WindowSizeChangedEventArgs const& args)
      {
         auto dims = window_dimmensions(args);
         m_hPixels = dims.first;
         m_wPixels = dims.second;
      }

      float m_wPixels;
      float m_hPixels;
      Core::CoreWindow* m_wnd_p;
      ViewManagement::ApplicationView* m_view_p;
   };

   HRESULT make_window(
      winrt::Windows::UI::Core::CoreWindow* const wnd_p,
      winrt::Windows::UI::ViewManagement::ApplicationView* const view_p,
      iwindow** out_p)
   {
      if (out_p == nullptr)
      {
#ifdef DEBUG
         OutputDebugString(L"out_p cannot be nullptr.");
#endif
         return E_INVALIDARG;
      }

      iwindow* ret = new window_0_1(wnd_p, view_p);

      if (ret == nullptr)
      {
#ifdef DEBUG
         OutputDebugString(L"Out of memory!");
#endif
         return E_OUTOFMEMORY;
      }

      *out_p = ret;
      return S_OK;
   }
}