#pragma once
#include "include/qgl_input_include.h"
#include "include/qgl_input_key.h"

namespace qgl::input
{
   using input_mouse_button =
      typename winrt::Windows::UI::Input::PointerUpdateKind;

   constexpr std::array<winrt::Windows::UI::Input::PointerUpdateKind, 11>
      pointer_keys =
   {
      input_mouse_button::LeftButtonPressed,
      input_mouse_button::RightButtonPressed,
      input_mouse_button::MiddleButtonPressed,
      input_mouse_button::XButton1Pressed,
      input_mouse_button::XButton2Pressed,
      input_mouse_button::Other,
      input_mouse_button::LeftButtonReleased,
      input_mouse_button::RightButtonReleased,
      input_mouse_button::MiddleButtonReleased,
      input_mouse_button::XButton1Released,
      input_mouse_button::XButton2Released,
   };

   constexpr std::array<winrt::Windows::UI::Input::PointerUpdateKind, 5>
      pointer_press_keys =
   {
      input_mouse_button::LeftButtonPressed,
      input_mouse_button::RightButtonPressed,
      input_mouse_button::MiddleButtonPressed,
      input_mouse_button::XButton1Pressed,
      input_mouse_button::XButton2Pressed,
   };

   constexpr std::array<winrt::Windows::UI::Input::PointerUpdateKind, 5>
      pointer_release_keys =
   {
      input_mouse_button::LeftButtonReleased,
      input_mouse_button::RightButtonReleased,
      input_mouse_button::MiddleButtonReleased,
      input_mouse_button::XButton1Released,
      input_mouse_button::XButton2Released,
   };

   inline input_key pointer_key(input_mouse_button button) noexcept
   {
      static const std::unordered_map<input_mouse_button, input_key>
         POINTER_KIND_INPUT_KEY_MAP =
      {
         { input_mouse_button::LeftButtonPressed,input_key::LeftButton },
         { input_mouse_button::RightButtonPressed,input_key::RightButton },
         { input_mouse_button::MiddleButtonPressed,input_key::MiddleButton },
         { input_mouse_button::XButton1Pressed,input_key::XButton1 },
         { input_mouse_button::XButton2Pressed,input_key::XButton2 },
         { input_mouse_button::LeftButtonReleased,input_key::LeftButton },
         { input_mouse_button::RightButtonReleased,input_key::RightButton },
         { input_mouse_button::MiddleButtonReleased,input_key::MiddleButton },
         { input_mouse_button::XButton1Released,input_key::XButton1 },
         { input_mouse_button::XButton2Released, input_key::XButton2 },
      };

      if (POINTER_KIND_INPUT_KEY_MAP.count(button) > 0)
      {
         // The above guard will keep this from throwing. 
         // Mark the function as noexcept.
         return POINTER_KIND_INPUT_KEY_MAP.at(button);
      }

      return input_key::None;
   }
}