#pragma once
#include "include/qgl_input_include.h"
#include "include/qgl_input_instance.h"
#include "include/qgl_gamepad_helpers.h"

namespace qgl::input::providers
{
   /*
    Gets the input for an XInput compatible controller.
    */
   class xinput_provider_traits
   {
      public:
      xinput_provider_traits(
         uint8_t controllerIndex,
         const dead_zone_config& config) :
         m_index(controllerIndex),
         m_deadzoneConfig(config),
         m_lastPacketNumber(0),
         m_lastRTrigger(false),
         m_lastLTrigger(false),
         m_lastRStick(false),
         m_lastLStick(false)
      {

      }

      xinput_provider_traits(const xinput_provider_traits&) noexcept = delete;

      xinput_provider_traits(xinput_provider_traits&&) noexcept = default;

      ~xinput_provider_traits() noexcept = default;

      input_instance instance() const noexcept
      {
         XINPUT_STATE state = { 0 };
         auto pollResult = XInputGetState(static_cast<DWORD>(m_index), &state);
         if (pollResult != ERROR_SUCCESS ||
            state.dwPacketNumber == m_lastPacketNumber)
         {
            // Controller disconnected or there were no changes in input since
            // the last instance.
            return input_instance{};
         }

         m_lastPacketNumber = state.dwPacketNumber;
         std::vector<input_state> inputs;

         // Check which buttons are pressed.
         for (auto xInputButton : XINPUT_BUTTONS)
         {
            // If the button is pressed:
            if (pressed(state.Gamepad.wButtons, xInputButton))
            {
               inputs.emplace_back(xInputButton, 
                  BUTTON_STATES::BUTTON_STATE_PRESSED);
            }
            else
            {
               // If the button was pressed last frame:
               if (m_lastUpdateButtons.count(xInputButton) > 0)
               {
                  inputs.emplace_back(xInputButton,
                     BUTTON_STATES::BUTTON_STATE_RELEASED);
                  m_lastUpdateButtons.erase(xInputButton);
               }
            }
         }

         // Handle the triggers

         // Normalize the right trigger.
         auto rTrigger = normalize_trigger_input(
            state.Gamepad.bRightTrigger,
            m_deadzoneConfig.rtrigger, 
            INPUT_AXIS_IDS::INPUT_AXIS_ID_RTRIGGER);

         // If it is not in the dead zone:
         if (!rTrigger.approx_zero())
         {
            inputs.emplace_back(rTrigger);
            m_lastRTrigger = true;
         }
         else if (m_lastRTrigger)
         {
            // Trigger not pressed but it was pressed last update.
            inputs.emplace_back(rTrigger, BUTTON_STATES::BUTTON_STATE_RELEASED);
            m_lastRTrigger = false;
         }

         auto lTrigger = normalize_trigger_input(
            state.Gamepad.bLeftTrigger,
            m_deadzoneConfig.ltrigger,
            INPUT_AXIS_IDS::INPUT_AXIS_ID_LTRIGGER);
         if (!lTrigger.approx_zero())
         {
            inputs.emplace_back(lTrigger, BUTTON_STATES::BUTTON_STATE_PRESSED);
            m_lastLTrigger = true;
         }
         else if (m_lastLTrigger)
         {
            inputs.emplace_back(lTrigger, BUTTON_STATES::BUTTON_STATE_RELEASED);
            m_lastLTrigger = false;
         }

         // Handle the thumbsticks
         auto rStick = normalize_stick_input(state.Gamepad.sThumbRX,
            state.Gamepad.sThumbRY, m_deadzoneConfig.rstick,
            INPUT_AXIS_IDS::INPUT_AXIS_ID_RSTICK);
         if (!rStick.approx_zero())
         {
            inputs.emplace_back(rStick, BUTTON_STATES::BUTTON_STATE_PRESSED);
            m_lastRStick = true;
         }
         else if (m_lastRStick)
         {
            inputs.emplace_back(rStick, BUTTON_STATES::BUTTON_STATE_RELEASED);
            m_lastRStick = false;
         }

         auto lStick = normalize_stick_input(state.Gamepad.sThumbLX,
            state.Gamepad.sThumbLY, m_deadzoneConfig.lstick,
            INPUT_AXIS_IDS::INPUT_AXIS_ID_LSTICK);
         if (!lStick.approx_zero())
         {
            inputs.emplace_back(lStick, BUTTON_STATES::BUTTON_STATE_PRESSED);
            m_lastLStick = true;
         }
         else if (m_lastLStick)
         {
            inputs.emplace_back(lStick, BUTTON_STATES::BUTTON_STATE_RELEASED);
            m_lastLStick = false;
         }

         return input_instance{ inputs.cbegin(), inputs.cend() };
      }

      private:
      input_axis normalize_trigger_input(
         xinput_trigger triggerInput,
         axis_magnitude normalizedDeadZone,
         INPUT_AXIS_IDS id) const noexcept
      {
         auto deadZone = static_cast<xinput_trigger>(
            normalizedDeadZone * XINPUT_TRIGGER_AXIS_MAX);
         return normalize_axis(triggerInput,
            static_cast<xinput_trigger>(XINPUT_TRIGGER_AXIS_MIN),
            static_cast<xinput_trigger>(XINPUT_TRIGGER_AXIS_MAX),
            deadZone, id);
      }

      /*
       Requires math that is not constexpr.
       */
      input_axis2d normalize_stick_input(
         xinput_stick x,
         xinput_stick y,
         axis_magnitude normalizedDeadZone,
         INPUT_AXIS_IDS id) const noexcept
      {
         auto deadZone = static_cast<xinput_stick>(
            normalizedDeadZone * XINPUT_STICK_AXIS_MAX);
         return normalize_axis(x, y,
            static_cast<xinput_stick>(XINPUT_STICK_AXIS_MIN),
            static_cast<xinput_stick>(XINPUT_STICK_AXIS_MAX),
            deadZone, id);
      }

      /*
       index of the XInput device this polls.
       */
      uint8_t m_index;

      /*
       XInput devices include a packet number when they are polled. Use this to
       track if there are input differences.
       */
      mutable DWORD m_lastPacketNumber;

      /*
       Defines when to throw away inputs because they are within the
       "dead zone".
       */
      dead_zone_config m_deadzoneConfig;

      /*
       Remember which buttons were pressed last input so we can track when
       a button is released.
       */
      mutable std::set<xinput_button> m_lastUpdateButtons;

      mutable bool m_lastRTrigger;

      mutable bool m_lastLTrigger;

      mutable bool m_lastRStick;

      mutable bool m_lastLStick;
   };
}