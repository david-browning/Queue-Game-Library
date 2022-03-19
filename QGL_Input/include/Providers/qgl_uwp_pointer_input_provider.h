#pragma once
#include "include/qgl_input_include.h"
#include "include/qgl_input_instance.h"
#include "include/Helpers/qgl_pointer_helpers.h"

namespace qgl::input::providers
{
   class uwp_point_input_provider_traits
   {
      public:
      using window_ptr = std::shared_ptr<qgl::graphics::window>;

      uwp_point_input_provider_traits(window_ptr&& window_p) :
         m_window_p(std::forward<window_ptr>(window_p))
      {
         set_dimmensions();
         hook();
      }

      uwp_point_input_provider_traits(
         const uwp_point_input_provider_traits&) noexcept = delete;

      uwp_point_input_provider_traits(
         uwp_point_input_provider_traits&& r) noexcept
      {
         // Unhook r's event handlers.
         r.unhook();

         m_window_p = std::move(r.m_window_p);

         // Move r's queued inputs. No need to lock since r is now unhooked.
         m_queuedInputs = std::move(r.m_queuedInputs);
         m_currentlyPressed = std::move(r.m_currentlyPressed);

         set_dimmensions();
         hook();
      }

      ~uwp_point_input_provider_traits() noexcept
      {
         std::lock_guard(m_vecMutex);
         unhook();
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
         m_wndSizeChangedToken = m_window_p->rt_window()->SizeChanged(
            { this, window_resized });
         m_pointerPressedToken = m_window_p->rt_window()->PointerPressed(
            { this, on_pointer_pressed });
         m_pointerReleasedToken = m_window_p->rt_window()->PointerReleased(
            { this, on_pointer_released });
         m_pointerMovedToken = m_window_p->rt_window()->PointerMoved(
            { this, on_pointer_moved });

         hooked = true;
      }

      void unhook()
      {
         // Lock mutex so nothing can be added to the inputs.
         std::lock_guard(m_vecMutex);
         if (hooked)
         {
            m_window_p->rt_window()->PointerPressed(m_pointerPressedToken);
            m_window_p->rt_window()->PointerReleased(m_pointerReleasedToken);
            m_window_p->rt_window()->PointerMoved(m_pointerMovedToken);
            m_window_p->rt_window()->SizeChanged(m_wndSizeChangedToken);
         }

         hooked = false;
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

      /*
       Handle to the window that raises the key pressed events.
       */
      window_ptr m_window_p;

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

      winrt::event_token m_pointerPressedToken;
      winrt::event_token m_pointerReleasedToken;
      winrt::event_token m_pointerMovedToken;
      winrt::event_token m_wndSizeChangedToken;
   };
}