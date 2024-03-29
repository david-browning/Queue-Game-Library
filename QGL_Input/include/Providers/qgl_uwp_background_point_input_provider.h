#pragma once
#include "include/qgl_input_include.h"
#include "include/qgl_input_instance.h"
#include "include/Helpers/qgl_pointer_helpers.h"

namespace qgl::input::providers
{
   class uwp_background_point_input_provider_traits
   {
      public:
      using window_ptr = std::shared_ptr<qgl::graphics::window>;
      using swapchain =
         typename winrt::Windows::UI::Xaml::Controls::SwapChainPanel;
      using swapchain_ptr = std::shared_ptr<swapchain>;

      uwp_background_point_input_provider_traits(swapchain_ptr&& sc_p,
                                                 window_ptr&& window_p) :
         m_window_p(std::forward<window_ptr>(window_p)),
         m_sc_p(std::forward<swapchain_ptr>(sc_p))
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
         m_wndSizeChangedToken = m_window_p->rt_window()->SizeChanged(
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
            m_window_p->rt_window()->SizeChanged(m_wndSizeChangedToken);

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
               button_states::pressed);
         }
      }

      void on_pointer_released(winrt::Windows::Foundation::IInspectable const&,
         winrt::Windows::UI::Core::PointerEventArgs const& args)
      {
         std::lock_guard(m_vecMutex);
         auto key = pointer_key(
            args.CurrentPoint().Properties().PointerUpdateKind());

         m_queuedInputs.emplace_back(key,
            button_states::released);
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
            x, y, input_axis_ids::mouse },
            button_states::invalid);
      }

      void window_resized(
         winrt::Windows::Foundation::IInspectable const&,
         winrt::Windows::UI::Core::WindowSizeChangedEventArgs const& args)
      {
         std::lock_guard(m_vecMutex);

         auto dims = qgl::graphics::window_dimmensions(args);
         m_wndHeight = dims.first;
         m_wndWidth = dims.second;
      }

      void set_dimmensions()
      {
         auto wndDims = qgl::graphics::window_dimmensions(
            m_window_p->rt_window());
         m_wndWidth = wndDims.first;
         m_wndHeight = wndDims.second;
      }

      window_ptr m_window_p;

      swapchain_ptr m_sc_p;

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