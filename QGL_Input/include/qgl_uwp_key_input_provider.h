#pragma once
#include "include/qgl_input_include.h"
#include "include/qgl_input_instance.h"

namespace qgl::input::providers
{
   class uwp_key_input_provider_traits
   {
      public:
      using window = typename winrt::Windows::UI::Core::CoreWindow;
      uwp_key_input_provider_traits(
         const std::shared_ptr<window>& window_p) :
         m_window_p(window_p)
      {
         hook();
      }

      uwp_key_input_provider_traits(
         const uwp_key_input_provider_traits&) noexcept = delete;

      uwp_key_input_provider_traits(
         uwp_key_input_provider_traits&& r) noexcept
      {
         m_window_p = std::move(r.m_window_p);

         // Unhook r's event handlers.
         r.unhook();

         // Move r's queued inputs. No need to lock since r is now unhooked.
         m_queuedInputs = std::move(r.m_queuedInputs);
         m_currentlyPressed = std::move(r.m_currentlyPressed);

         hook();
      }

      ~uwp_key_input_provider_traits() noexcept
      {
         std::lock_guard(m_vecMutex);
         unhook();
      }

      input_instance instance() const noexcept
      {
         // Lock the mutex so event handlers cannot add to it.
         std::lock_guard(m_vecMutex);
         return input_instance{
            m_queuedInputs.cbegin(),
            m_queuedInputs.cend()
         };

         m_queuedInputs.clear();
         m_currentlyPressed.clear();

         // Lock goes out of scope now.
      }

      private:
      void hook()
      {
         m_keyDownToken = m_window_p->KeyDown(
            { this, uwp_key_input_provider_traits::on_key_down });
         m_keyUpToken = m_window_p->KeyUp(
            { this, &uwp_key_input_provider_traits::on_key_up });
         hooked = true;
      }

      void unhook()
      {
         // Lock mutex so nothing can be added to the inputs.
         std::lock_guard(m_vecMutex);
         if (hooked)
         {
            m_window_p->KeyDown(m_keyDownToken);
            m_window_p->KeyUp(m_keyUpToken);
         }

         hooked = false;
      }

      void on_key_down(winrt::Windows::Foundation::IInspectable const&,
         winrt::Windows::UI::Core::KeyEventArgs const& args)
      {
         std::lock_guard(m_vecMutex);
         auto key = args.VirtualKey();

         // Only queue the input if the key was not already pressed.
         if (m_currentlyPressed.count(key) == 0)
         {
            m_currentlyPressed.insert(key);
            m_queuedInputs.emplace_back(key,
               BUTTON_STATES::BUTTON_STATE_PRESSED);
         }
      }

      void on_key_up(winrt::Windows::Foundation::IInspectable const&,
         winrt::Windows::UI::Core::KeyEventArgs const& args)
      {
         std::lock_guard(m_vecMutex);
         auto key = args.VirtualKey();

         // Queue the key up input.
         m_queuedInputs.emplace_back(key, BUTTON_STATES::BUTTON_STATE_RELEASED);

         // Remove the key from the set of pressed keys.
         m_currentlyPressed.erase(key);
      }

      /*
       Handle to the window that raises the key pressed events.
       */
      std::shared_ptr<window> m_window_p;

      /*
       Inputs that were pressed since the last game loop update.
       */
      mutable std::vector<input_state> m_queuedInputs;

      /*
       Hash set of which keys are currently pressed. This is used so the same
       key does not get queued twice between game loop updates.
       */
      mutable std::unordered_set<input_key> m_currentlyPressed;

      /*
       Blocks multiple threads from updating the queued inputs at once.
       */
      std::mutex m_vecMutex;

      bool hooked;

      winrt::event_token m_keyDownToken;
      winrt::event_token m_keyUpToken;
   };
}