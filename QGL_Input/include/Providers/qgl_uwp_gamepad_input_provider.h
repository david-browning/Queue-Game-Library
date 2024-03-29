#pragma once
#include "include/qgl_input_include.h"
#include "include/qgl_input_state.h"
#include "include/qgl_input_instance.h"
#include "include/Helpers/qgl_gamepad_helpers.h"

namespace qgl::input::providers
{
   class gamepad_provider_traits
   {
      public:
      using gamepad = typename winrt::Windows::Gaming::Input::Gamepad;
      using gamepad_ptr = typename std::shared_ptr<gamepad>;

      gamepad_provider_traits(gamepad_ptr&& gamepad_p,
                              dead_zone_config&& deadzone) :
         m_gamepad_p(std::forward<gamepad_ptr>(gamepad_p)),
         m_deadZone(std::forward<dead_zone_config>(deadzone)),
         m_lastRTrigger(false),
         m_lastLTrigger(false),
         m_lastRStick(false),
         m_lastLStick(false)
      {

      }

      gamepad_provider_traits(const gamepad_provider_traits&) noexcept = delete;

      gamepad_provider_traits(gamepad_provider_traits&&) noexcept = default;

      ~gamepad_provider_traits() noexcept = default;

      input_instance instance() const noexcept
      {
         auto state = m_gamepad_p->GetCurrentReading();
         std::vector<input_state> inputs;

         // Detect which buttons are pressed or released.
         for (auto button : GAMEPAD_BUTTONS)
         {
            // If the button is pressed:
            if (pressed<gamepad_button>(state.Buttons, button))
            {
               inputs.emplace_back(button, button_states::pressed);
            }
            else
            {
               // If the button was pressed last frame:
               if (m_lastUpdateButtons.count(button) > 0)
               {
                  // Add a button released event.
                  inputs.emplace_back(button,
                     button_states::released);
                  m_lastUpdateButtons.erase(button);
               }
            }
         }

         // Handle the triggers

         // Normalize the right trigger.
         auto rTrigger = normalize_axis(state.RightTrigger, GAMEPAD_TRIGGER_MIN,
            GAMEPAD_TRIGGER_MAX, m_deadZone.rtrigger,
            input_axis_ids::right_trigger);

         // If it is not in the dead zone:
         if (!rTrigger.approx_zero())
         {
            inputs.emplace_back(rTrigger, button_states::pressed);
            m_lastRTrigger = true;
         }
         else if (m_lastRTrigger)
         {
            // Trigger not pressed but it was pressed last update.
            inputs.emplace_back(rTrigger, button_states::released);
            m_lastRTrigger = false;
         }

         auto lTrigger = normalize_axis(state.LeftTrigger, GAMEPAD_TRIGGER_MIN,
            GAMEPAD_TRIGGER_MAX, m_deadZone.ltrigger,
            input_axis_ids::left_trigger);
         if (!lTrigger.approx_zero())
         {
            inputs.emplace_back(lTrigger, button_states::pressed);
            m_lastLTrigger = true;
         }
         else if (m_lastLTrigger)
         {
            inputs.emplace_back(lTrigger, button_states::released);
            m_lastLTrigger = false;
         }

         // Handle the thumbsticks
         auto rStick = normalize_axis(state.RightThumbstickX,
            state.RightThumbstickX, GAMEPAD_STICK_MIN, GAMEPAD_STICK_MAX,
            m_deadZone.rstick, input_axis_ids::right_stick);
         if (!rStick.approx_zero())
         {
            inputs.emplace_back(rStick, button_states::pressed);
            m_lastRStick = true;
         }
         else if (m_lastRStick)
         {
            inputs.emplace_back(rStick, button_states::released);
            m_lastRStick = false;
         }

         auto lStick = normalize_axis(state.LeftThumbstickX,
            state.LeftThumbstickY, GAMEPAD_STICK_MIN, GAMEPAD_STICK_MAX,
            m_deadZone.lstick, input_axis_ids::left_stick);
         if (!rStick.approx_zero())
         {
            inputs.emplace_back(lStick, button_states::pressed);
            m_lastLStick = true;
         }
         else if (m_lastLStick)
         {
            inputs.emplace_back(lStick, button_states::released);
            m_lastLStick = false;
         }

         return input_instance{ inputs.cbegin(), inputs.cend() };
      }

      private:
      /*
       Handle to the gamepad.
       */
      gamepad_ptr m_gamepad_p;

      dead_zone_config m_deadZone;

      /*
       Remember which buttons were pressed last input so we can track when
       a button is released.
       */
      mutable std::set<gamepad_button> m_lastUpdateButtons;

      mutable bool m_lastRTrigger;

      mutable bool m_lastLTrigger;

      mutable bool m_lastRStick;

      mutable bool m_lastLStick;
   };
}