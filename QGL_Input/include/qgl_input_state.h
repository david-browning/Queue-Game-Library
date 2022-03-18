#pragma once
#include "include/qgl_input_include.h"
#include "include/Input/qgl_gamepad_helpers.h"
#include "include/Memory/qgl_mem_helpers.h"

namespace qgl::input
{
   using input_key = typename winrt::Windows::System::VirtualKey;

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

      /*
       Button state and input type are 4 bits each.
       Squash them together and store it in the top byte of a result_type.
       */
      constexpr result_type type_state_hash(
         const argument_type& t) const noexcept
      {
         auto typeOffset = (sizeof(result_type) - sizeof(uint8_t)) * CHAR_BIT;
         auto stateOffset = typeOffset + 4;
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
               // Squash the axis down to a 4 byte float. While this loses 
               // some precision, its unlikely that input states are expected
               // to be so precise that this actually matters.
               auto value = static_cast<float>(t.axis().value());
               qgl::mem::bit_convert<float, uint32_t> x{ value };

               // OR the value and type together so the type is the upper 32
               // bits (Which is a lot of wasted storage) and the lower 32 bits
               // is the squashed value.
               return type_state_hash(t) | 
                  (static_cast<result_type>(t.axis().id()) << 
                     (sizeof(value) * CHAR_BIT)) | 
                  x.to();
            }
            case INPUT_TYPES::INPUT_TYPE_AXIS_2D:
            {
               // [63-56] [55] [54] [53-0]
               // Axis type, Y axis sign, X axis sign, Y axis, X axis

               // There are 4 constant bits per 32-bit float.
               // 1-5 are the same for values between -1 and 1 at 1/100 
               // presision. 

               // We can move the bits around so that there are 56 bits to 
               // represent the 2 axis and the top 8 represent the axis type.

               // Squash the axis into 4 byte floats.
               auto x = static_cast<float>(t.axis_2d().x());
               auto y = static_cast<float>(t.axis_2d().y());

               // Get a binary represenation of the floats.
               qgl::mem::bit_convert<float, uint32_t> xBits{ x };
               qgl::mem::bit_convert<float, uint32_t> yBits{ y };

               // Move the x and y floats to the correct position.
               auto xXForm = static_cast<result_type>(xBits.to() & fracMask);
               auto yXForm = static_cast<result_type>(yBits.to() & fracMask) <<
                  fracLen;

               // Put the signs back in:
               auto xSign = static_cast<result_type>(xBits.to() & signMask) << 
                  (2 * fracLen);
               auto ySign = static_cast<result_type>(yBits.to() & signMask) << 
                  (2 * fracLen + 1);

               // Put the type in the top 8 bits
               auto type = static_cast<result_type>(t.axis().id()) << 
                  (2 * fracLen + 2);

               return type | ySign | xSign | yXForm | xXForm;
            }
         }

         // Fall back to hashing the type and state.
         return type_state_hash(t);
      }
   };
}