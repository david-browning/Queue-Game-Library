#pragma once
#include "include/qgl_input_include.h"
#include <Xinput.h>
#include <winrt/Windows.Gaming.Input.h>
#include <QGLMath.h>

namespace qgl::input
{
   enum class input_axis_ids : uint8_t
   {
      none = 0,
      mouse = 1,
      right_trigger = 2,
      left_trigger = 4,
      right_stick = 8,
      left_stick = 16,
   };

   /*
    Hash functions guarentee this to be accurate to 3 decimal places!
    */
   using axis_magnitude = typename double;

   struct dead_zone_config
   {
      axis_magnitude lstick;
      axis_magnitude rstick;
      axis_magnitude ltrigger;
      axis_magnitude rtrigger;
   };

   /*
    Use 12 bits to store an axis value between -1 and -1.
    This gives us a precision of 0.0005.
    */
   static constexpr size_t AXIS_DENOMINATOR = 2047;

   struct input_axis
   {
      public:
      constexpr input_axis(axis_magnitude val, input_axis_ids axis,
         axis_magnitude tol = FLT_EPSILON) :
         m_value(val), m_id(axis), m_tolerance(tol)
      {

      }

      input_axis(const input_axis&) = default;

      input_axis(input_axis&&) = default;

      constexpr bool approx_zero() const noexcept
      {
         return math::approx_equal(0, value(), m_tolerance);
      }

      constexpr axis_magnitude value() const noexcept
      {
         return m_value;
      }

      constexpr input_axis_ids id() const noexcept
      {
         return m_id;
      }

      private:
      axis_magnitude m_value;
      axis_magnitude m_tolerance;
      input_axis_ids m_id;
   };

   struct input_axis2d
   {
      public:
      constexpr input_axis2d(axis_magnitude xValue, axis_magnitude yValue,
         input_axis_ids axis, axis_magnitude tol = FLT_EPSILON) :
         m_x(xValue), m_y(yValue), m_id(axis), m_tolerance(tol)
      {

      }

      constexpr input_axis2d(const input_axis2d&) noexcept = default;

      constexpr input_axis2d(input_axis2d&&) noexcept = default;

      constexpr bool approx_zero() const noexcept
      {
         return
            math::approx_equal(0, x(), m_tolerance) &&
            math::approx_equal(0, y(), m_tolerance);
      }

      constexpr axis_magnitude x() const noexcept
      {
         return m_x;
      }

      constexpr axis_magnitude y() const noexcept
      {
         return m_y;
      }

      constexpr input_axis_ids id() const noexcept
      {
         return m_id;
      }

      private:
      axis_magnitude m_x;
      axis_magnitude m_y;
      axis_magnitude m_tolerance;
      input_axis_ids m_id;
   };

   template<typename ButtonT>
   constexpr bool pressed(ButtonT input, ButtonT button)
   {
      return (input & button) == button;
   }

   /*
    Normalizes "value" to the given range and subtracts the deadzone.
    Returns 0 if "value" is within the "deadZone".
    */
   template<typename ReadingT>
   constexpr input_axis normalize_axis(
      ReadingT value,
      ReadingT minValue,
      ReadingT maxValue,
      ReadingT deadZone,
      input_axis_ids id,
      axis_magnitude tolerance = FLT_EPSILON)
   {
      if (value > deadZone)
      {
         // Clamp the value to the max and adjust it relative to the end of the
         // dead zone.
         auto clamped = std::clamp(value, minValue, maxValue) - deadZone;

         // Normalize the value to the 0-1 range.
         auto value = static_cast<axis_magnitude>(clamped) /
            static_cast<axis_magnitude>(maxValue - deadZone);

         return input_axis{ value, id, tolerance };
      }

      // Inside the deadzone.
      return input_axis{ static_cast<axis_magnitude>(0), id, tolerance };
   }

   /*
    Normalizes thumbstick input and discards the input if it is within the
    dead zone.
    Based off https://docs.microsoft.com/en-us/windows/win32/xinput/getting-started-with-xinput
    */
   template<typename ReadingT>
   constexpr input_axis2d normalize_axis(
      ReadingT valueX,
      ReadingT valueY,
      ReadingT minValue,
      ReadingT maxValue,
      ReadingT deadZone,
      input_axis_ids id,
      axis_magnitude tolerance = FLT_EPSILON)
   {
      auto mag = std::sqrt(static_cast<axis_magnitude>(
         valueX * valueX + valueY * valueY));

      if (static_cast<ReadingT>(mag) > deadZone)
      {
         // Clamp the values to the max and adjust it relative to the end of 
         // the dead zone.
         auto clampedX = std::clamp(valueX, minValue, maxValue) - deadZone;
         auto clampedY = std::clamp(valueY, minValue, maxValue) - deadZone;

         // Normalize the axis to -1-1 range.
         auto normX = static_cast<axis_magnitude>(clampedX) /
            static_cast<axis_magnitude>(maxValue - deadZone);

         auto normY = static_cast<axis_magnitude>(clampedY) /
            static_cast<axis_magnitude>(maxValue - deadZone);

         return input_axis2d{ normX, normY, id, tolerance };
      }

      // Inside the deadzone.
      return input_axis2d{ 0, 0, id, tolerance };
   }

#pragma region XInput Types
   using xinput_button = typename WORD;

   static constexpr std::array<xinput_button, 14> XINPUT_BUTTONS =
   {
      XINPUT_GAMEPAD_DPAD_UP,
      XINPUT_GAMEPAD_DPAD_DOWN,
      XINPUT_GAMEPAD_DPAD_LEFT,
      XINPUT_GAMEPAD_DPAD_RIGHT,
      XINPUT_GAMEPAD_START,
      XINPUT_GAMEPAD_BACK,
      XINPUT_GAMEPAD_LEFT_THUMB,
      XINPUT_GAMEPAD_RIGHT_THUMB,
      XINPUT_GAMEPAD_LEFT_SHOULDER,
      XINPUT_GAMEPAD_RIGHT_SHOULDER,
      XINPUT_GAMEPAD_A,
      XINPUT_GAMEPAD_B,
      XINPUT_GAMEPAD_X,
      XINPUT_GAMEPAD_Y,
   };

   using xinput_trigger = typename decltype(XINPUT_GAMEPAD::bLeftTrigger);
   using xinput_stick = typename decltype(XINPUT_GAMEPAD::sThumbLX);

   static constexpr auto XINPUT_STICK_AXIS_MIN = SHRT_MIN;
   static constexpr auto XINPUT_STICK_AXIS_MAX = SHRT_MAX;
   static constexpr auto XINPUT_TRIGGER_AXIS_MIN = 0;
   static constexpr auto XINPUT_TRIGGER_AXIS_MAX = UCHAR_MAX;
#pragma endregion

#pragma region UWP Gamepad Types
   using gamepad_button = typename winrt::Windows::Gaming::Input::GamepadButtons;

   static constexpr std::array<gamepad_button, 18> GAMEPAD_BUTTONS =
   {
      // Exclude the "No Button"
      gamepad_button::A,
      gamepad_button::B,
      gamepad_button::DPadDown,
      gamepad_button::DPadLeft,
      gamepad_button::DPadRight,
      gamepad_button::DPadUp,
      gamepad_button::LeftShoulder,
      gamepad_button::LeftThumbstick,
      gamepad_button::Menu,
      gamepad_button::Paddle1,
      gamepad_button::Paddle2,
      gamepad_button::Paddle3,
      gamepad_button::Paddle4,
      gamepad_button::RightShoulder,
      gamepad_button::RightThumbstick,
      gamepad_button::View,
      gamepad_button::X,
      gamepad_button::Y,
   };

   using gamepad_trigger = typename decltype(
      winrt::Windows::Gaming::Input::GamepadReading::LeftTrigger);
   using gamepad_stick = typename decltype(
      winrt::Windows::Gaming::Input::GamepadReading::RightThumbstickX);

   static constexpr auto GAMEPAD_TRIGGER_MIN =
      static_cast<gamepad_trigger>(0);
   static constexpr auto GAMEPAD_TRIGGER_MAX =
      static_cast<gamepad_trigger>(1);

   static constexpr auto GAMEPAD_STICK_MIN =
      static_cast<gamepad_trigger>(-1);
   static constexpr auto GAMEPAD_STICK_MAX =
      static_cast<gamepad_trigger>(1);

#pragma endregion
}