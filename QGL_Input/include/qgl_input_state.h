#pragma once
#include "include/qgl_input_include.h"
#include "include/qgl_input_key.h"
#include "include/Helpers/qgl_gamepad_helpers.h"

namespace qgl::input
{
   enum class BUTTON_STATES : uint8_t
   {
      BUTTON_STATE_INVALID = 0,
      BUTTON_STATE_PRESSED = 1,
      BUTTON_STATE_RELEASED = 2,
      BUTTON_STATE_TOGGLED = 3,
   };

   /*
   Only the low 4 bits are set.
   */
   enum class INPUT_TYPES : uint8_t
   {
      /*
      Keyboard or mouse button pressed or released.
      */
      INPUT_TYPE_KEY = 1,

      /*
      A 1D axis such as a trigger.
      */
      INPUT_TYPE_AXIS = 2,

      /*
      A 2D axis such as a mouse or thumbstick movement.
      */
      INPUT_TYPE_AXIS_2D = 3,

      /*
      A gamepad button was pressed or released.
      Gamepads provided by the WinRT framework have more functionality than
      XInput.
      */
      INPUT_TYPE_GAMEPAD = 4,

      /*
      An XInput device button was pressed or released.
      */
      INPUT_TYPE_XINPUT = 5,
   };

   class input_state final
   {
      public:
      input_state(input_key input, BUTTON_STATES state) :
         input_state(input, state, INPUT_TYPES::INPUT_TYPE_KEY)
      {

      }

      input_state(gamepad_button input, BUTTON_STATES state) :
         input_state(input, state, INPUT_TYPES::INPUT_TYPE_GAMEPAD)
      {

      }

      input_state(xinput_button input, BUTTON_STATES state) :
         input_state(input, state, INPUT_TYPES::INPUT_TYPE_XINPUT)
      {

      }

      input_state(input_axis&& input, BUTTON_STATES state) :
         input_state(std::forward<input_axis>(input),
            state,
            INPUT_TYPES::INPUT_TYPE_AXIS)
      {

      }

      input_state(input_axis2d&& input, BUTTON_STATES state) :
         input_state(std::forward<input_axis2d>(input),
            state,
            INPUT_TYPES::INPUT_TYPE_AXIS_2D)
      {

      }

      template<typename Input>
      input_state(Input&& i,
         BUTTON_STATES buttonState,
         INPUT_TYPES inputType) :
         m_state(buttonState),
         m_type(inputType),
         m_pressed(std::forward<Input>(i))
      {

      }

      /*
       Copy Constructor
       */
      input_state(const input_state&) noexcept = default;

      /*
       Move constructor
       */
      input_state(input_state&&) noexcept = default;

      /*
       Destructor
       */
      ~input_state() noexcept = default;

      /*
       Returns the type of input.
       Use this to determine which property is valid for this.
       */
      constexpr INPUT_TYPES type() const noexcept
      {
         return m_type;
      }

      constexpr BUTTON_STATES state() const noexcept
      {
         return m_state;
      }

      constexpr input_key key() const
      {
         return std::get<input_key>(m_pressed);
      }

      constexpr gamepad_button gp_button() const
      {
         return std::get<gamepad_button>(m_pressed);
      }

      constexpr xinput_button xi_button() const
      {
         return std::get<xinput_button>(m_pressed);
      }

      /*
       Returns the percentage of how pressed the axis input is.
       Throws std::bad_variant_access if this does not hold an axis.
       */
      constexpr input_axis axis() const
      {
         return std::get<input_axis>(m_pressed);
      }

      /*
       Returns the percentage of how pressed the X and Y axis are.
       Throws std::bad_variant_access if this does not hold an axis.
       */
      constexpr input_axis2d axis_2d() const
      {
         return std::get<input_axis2d>(m_pressed);
      }

      private:
      INPUT_TYPES m_type;
      BUTTON_STATES m_state;
      std::variant<
         input_key, xinput_button, gamepad_button,
         input_axis, input_axis2d> m_pressed;
   };
}

namespace std
{
   template<>
   struct hash<qgl::input::input_state>
   {
      constexpr hash()
      {

      }

      typedef qgl::input::input_state argument_type;
      typedef std::size_t result_type;
      static_assert(sizeof(result_type) == 8,
         "Hash size must be at least 8 bytes.");

      static constexpr uint32_t fracLen = 27;
      static constexpr uint32_t fracMask = 0x07FF'FFFF;
      static constexpr uint32_t signMask = 1 << (sizeof(float) * CHAR_BIT - 1);

      static constexpr size_t typeOffset =
         (sizeof(result_type) - sizeof(uint8_t)) * CHAR_BIT;
      static constexpr size_t  stateOffset = typeOffset + 4;
      /*
       Button state and input type are 4 bits each.
       Squash them together and store it in the top byte of a result_type.
       */
      constexpr result_type type_state_hash(
         const argument_type& t) const noexcept
      {
         return
            (static_cast<result_type>(t.type()) << typeOffset) |
            (static_cast<result_type>(t.state()) << stateOffset);
      }

      constexpr result_type operator()(const argument_type& t) const noexcept
      {
         using namespace qgl::input;
         switch (t.type())
         {
            case INPUT_TYPES::INPUT_TYPE_KEY:
            {
               return type_state_hash(t) |
                  static_cast<result_type>(t.key());
            }
            case INPUT_TYPES::INPUT_TYPE_GAMEPAD:
            {
               return type_state_hash(t) |
                  static_cast<result_type>(t.gp_button());
            }
            case INPUT_TYPES::INPUT_TYPE_XINPUT:
            {
               return type_state_hash(t) |
                  static_cast<result_type>(t.xi_button());
            }
            case INPUT_TYPES::INPUT_TYPE_AXIS:
            {
               // Squash the axis down to a 2 byte numerator. While this loses 
               // some precision, its unlikely that input states are expected
               // to be so precise that this actually matters.
               qgl::math::decimal<qgl::input::AXIS_DENOMINATOR> x{
                  t.axis().value()
               };

               auto upper = type_state_hash(t) |
                  (static_cast<result_type>(t.axis().id()) <<
                     (sizeof(result_type) / 2 * CHAR_BIT));

               // OR the value and type together so the type is the upper 32
               // bits (Which is a lot of wasted storage) and the lower 32 bits
               // is the squashed value.
               return upper | x.numerator();
            }
            case INPUT_TYPES::INPUT_TYPE_AXIS_2D:
            {
               auto upper = type_state_hash(t) |
                  (static_cast<result_type>(t.axis_2d().id()) <<
                     (sizeof(result_type) / 2 * CHAR_BIT));

               qgl::math::decimal<qgl::input::AXIS_DENOMINATOR> x{
                  t.axis_2d().x()
               };

               qgl::math::decimal<qgl::input::AXIS_DENOMINATOR> y{
                  t.axis_2d().y()
               };

               return upper |
                  (static_cast<result_type>(y.numerator()) <<
                     (sizeof(x.numerator()) * CHAR_BIT)) |
                  static_cast<result_type>(x.numerator());
            }
         }

         // Fall back to hashing the type and state.
         return type_state_hash(t);
      }
   };
}