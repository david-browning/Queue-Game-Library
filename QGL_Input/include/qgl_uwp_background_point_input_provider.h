#pragma once
#include "include/qgl_input_include.h"
#include "include/qgl_input_instance.h"
#include "include/qgl_pointer_helpers.h"

namespace qgl::input::providers
{
   class uwp_background_point_input_provider_traits
   {
      public:
      using window = typename winrt::Windows::UI::Core::CoreWindow;
      using swapchain =
         typename winrt::Windows::UI::Xaml::Controls::SwapChainPanel;
      uwp_background_point_input_provider_traits(
         const std::shared_ptr<swapchain>& sc_p,
         const std::shared_ptr<window>& window_p) :
         m_window_p(window_p),
         m_sc_p(sc_p)
      {
         set_dimmensions();
         hook();
      }

      uwp_background_point_input_provider_traits(
         const uwp_background_point_input_provider_traits&) noexcept = delete;

      uwp_background_point_input_provider_traits(
         uwp_background_point_input_provider_traits&& r) noexcept
      {
         r.unhook();
         m_window_p = std::move(r.m_window_p);
         m_sc_p = std::move(r.m_sc_p);

         m_queuedInputs = std::move(r.m_queuedInputs);
         m_currentlyPressed = std::move(r.m_currentlyPressed);

         set_dimmensions();
         hook();
      }

      ~uwp_background_point_input_provider_traits() noexcept
      {
         std::lock_guard(m_vecMutex);
         unhook();

         m_backgroundInput.Cancel();
         m_backgroundInput.Close();
      }

      input_instance instance() const noexcept
      {
         std::lock_guard{ m_vecMutex };
         return input_instance{
            m_queuedInputs.cbegin(),
            m_queuedInputs.cend()
         };

         m_queuedInputs.clear();
         m_currentlyPressed.clear();
      }

      private:
      void hook()
      {
         using namespace winrt::Windows::System::Threading;
         m_wndSizeChangedToken = m_window_p->SizeChanged(
            { this, window_resized });
         m_backgroundInput = ThreadPool::RunAsync(background_process,
            WorkItemPriority::High, WorkItemOptions::TimeSliced);
         hooked = true;
      }

      void unhook()
      {
         std::lock_guard(m_vecMutex);
         if (hooked)
         {
            m_inputSource->PointerPressed(m_pointerPressedToken);
            m_inputSource->PointerReleased(m_pointerReleasedToken);
            m_inputSource->PointerMoved(m_pointerMovedToken);
            m_window_p->SizeChanged(m_wndSizeChangedToken);

            m_inputSource->Dispatcher().StopProcessEvents();
         }

         hooked = false;
      }

      winrt::Windows::Foundation::IAsyncAction background_process()
      {
         using namespace winrt::Windows::UI::Core;
         m_inputSource = std::make_unique<CoreIndependentInputSource>(
            m_sc_p->CreateCoreIndependentInputSource(
               CoreInputDeviceTypes::Mouse |
               CoreInputDeviceTypes::Pen |
               CoreInputDeviceTypes::Touch));

         m_inputSource->PointerPressed({ this, on_pointer_pressed });
         m_inputSource->PointerReleased({ this, on_pointer_released });
         m_inputSource->PointerMoved({ this, on_pointer_moved });

         m_inputSource->Dispatcher().ProcessEvents(
            CoreProcessEventsOption::ProcessUntilQuit);
      }

      void on_pointer_pressed(winrt::Windows::Foundation::IInspectable const&,
         winrt::Windows::UI::Core::PointerEventArgs const& args)
      {
         std::lock_guard(m_vecMutex);
         auto key = pointer_key(
            args.CurrentPoint().Properties().PointerUpdateKind());

         // Only queue the input if the key was not already pressed.
         if (key != input_key::None && m_currentlyPressed.count(key) == 0)
         {
            m_currentlyPressed.insert(key);
            m_queuedInputs.emplace_back(key,
               BUTTON_STATES::BUTTON_STATE_PRESSED);
         }
      }

      void on_pointer_released(winrt::Windows::Foundation::IInspectable const&,
         winrt::Windows::UI::Core::PointerEventArgs const& args)
      {
         std::lock_guard(m_vecMutex);
         auto key = pointer_key(
            args.CurrentPoint().Properties().PointerUpdateKind());

         m_queuedInputs.emplace_back(key,
            BUTTON_STATES::BUTTON_STATE_RELEASED);
         m_currentlyPressed.erase(key);
      }

      void on_pointer_moved(winrt::Windows::Foundation::IInspectable const&,
         winrt::Windows::UI::Core::PointerEventArgs const& args)
      {
         std::lock_guard(m_vecMutex);

         // Scale the X and Y position relative to the window's width and 
         // height.
         auto pos = args.CurrentPoint().Position();
         auto x = pos.X / m_wndWidth;
         auto y = pos.Y / m_wndHeight;

         m_queuedInputs.emplace_back(input_axis2d{
            x, y, INPUT_AXIS_IDS::INPUT_AXIS_ID_MOUSE },
            BUTTON_STATES::BUTTON_STATE_INVALID);
      }

      void window_resized(
         winrt::Windows::Foundation::IInspectable const&,
         winrt::Windows::UI::Core::WindowSizeChangedEventArgs const& args)
      {
         std::lock_guard(m_vecMutex);

         auto dims = window_dimmensions(args);
         m_wndHeight = dims.first;
         m_wndWidth = dims.second;
      }

      void set_dimmensions()
      {
         auto wndDims = window_dimmensions(m_window_p.get());
         m_wndWidth = wndDims.first;
         m_wndHeight = wndDims.second;
      }

      std::shared_ptr<window> m_window_p;

      std::shared_ptr<swapchain> m_sc_p;

      /*
       Inputs that will be dispatched when "instance()" is called.
       */
      mutable std::vector<input_state> m_queuedInputs;

      /*
       Keep track of which buttons are pressed so we don't raise multiple
       events between game looop updates.
       */
      mutable std::unordered_set<input_key> m_currentlyPressed;

      /*
       Prevent multiple event handlers frrom modifying the state at the same
       time.
       */
      std::mutex m_vecMutex;

      /*
       True if the event handlers have been hooked up.
       */
      bool hooked;

      /*
       Cache the window's width and height so we don't need to look it up
       every time the mouse moves.
       An event handler updates these.
       */
      float m_wndWidth;
      float m_wndHeight;

      std::unique_ptr<winrt::Windows::UI::Core::CoreIndependentInputSource>
         m_inputSource;

      winrt::Windows::Foundation::IAsyncAction m_backgroundInput;

      winrt::event_token m_pointerPressedToken;
      winrt::event_token m_pointerReleasedToken;
      winrt::event_token m_pointerMovedToken;
      winrt::event_token m_wndSizeChangedToken;
   };
}