#pragma once
#include "include/qgl_input_include.h"
#include "include/Input/qgl_input_state.h"

namespace qgl::input
{
   template<typename TickT>
   class input_trigger final
   {
      public:
      using key_array = typename std::vector<input_key>;
      using key_iterator = typename key_array::iterator;
      using const_key_iterator = typename key_array::const_iterator;

      /*
       Trigger for when the given key or mouse button is in the given state for
       the given amount of time.
       */
      input_trigger(input_key k, BUTTON_STATES btnState, TickT wait) :
         m_type(INPUT_TYPES::INPUT_TYPE_KEY), m_state(btnState), m_wait(wait),
         m_button(std::move(std::vector<input_key>{k}))
      {}

      template<class InputKeyIterator>
      input_trigger(InputKeyIterator first, InputKeyIterator last,
         BUTTON_STATES btnState, TickT wait) :
         m_type(INPUT_TYPES::INPUT_TYPE_KEY), m_state(btnState), m_wait(wait),
         m_button(std::move(std::vector<input_key>{first, last}))
      {}

      /*
       Trigger for when the given buttons are in the given state for the given
       amount of time.
       Bitwise OR buttons together so the trigger is raised only if all
       buttons are pressed.
       */
      input_trigger(gamepad_button btns, BUTTON_STATES btnState, TickT wait) :
         m_type(INPUT_TYPES::INPUT_TYPE_GAMEPAD), m_state(btnState),
         m_wait(wait), m_button(btns)
      {}

      /*
       Trigger for when the given buttons are in the given state for the given
       amount of time.
       Bitwise OR buttons together so the trigger is raised only if all
       buttons are pressed.
       */
      input_trigger(xinput_button btns, BUTTON_STATES btnState, TickT wait) :
         m_type(INPUT_TYPES::INPUT_TYPE_XINPUT), m_state(btnState),
         m_wait(wait), m_button(btns)
      {}

      /*
       Trigger for when the given axis is held at least "mag" amount for "wait"
       amount of time.
       */
      input_trigger(INPUT_AXIS_IDS axis, axis_magnitude mag, TickT wait) :
         m_type(INPUT_TYPES::INPUT_TYPE_AXIS),
         m_state(BUTTON_STATES::BUTTON_STATE_PRESSED), m_wait(wait),
         m_button(std::move(input_axis{ mag, axis }))
      {}

      input_trigger(const input_trigger&) = default;

      input_trigger(input_trigger&&) = default;

      ~input_trigger() noexcept = default;

      constexpr TickT hold() const noexcept
      {
         return m_wait;
      }

      constexpr INPUT_TYPES type() const noexcept
      {
         return m_type;
      }

      constexpr BUTTON_STATES state() const noexcept
      {
         return m_state;
      }

      constexpr gamepad_button gp_button() const
      {
         return std::get<gamepad_button>(m_button);
      }

      constexpr xinput_button xi_button() const
      {
         return std::get<xinput_button>(m_button);
      }

      constexpr input_axis axis() const
      {
         return std::get<input_axis>(m_button);
      }

      constexpr input_axis2d axis_2d() const
      {
         return std::get<input_axis2d>(m_button);
      }

      constexpr size_t key_count() const
      {
         return std::get<key_array>(m_button).size();
      }

      const const_key_iterator cbegin() const
      {
         return std::get<key_array>(m_button).cbegin();
      }

      const const_key_iterator cend() const
      {
         return std::get<key_array>(m_button).cend();
      }

      friend void swap(input_trigger& l, input_trigger& r) noexcept
      {
         using std::swap;
         swap(l.m_type, r.m_type);
         swap(l.m_state, r.m_type);
         swap(l.m_wait, r.m_wait);
         swap(l.m_button, r.m_button);
      }

      input_trigger& operator=(input_trigger& r) noexcept
      {
         swap(*this, r);
         return *this;
      }

      input_trigger& operator=(input_trigger&& r) = default;

      friend bool operator==(
         const input_trigger& l, const input_trigger& r) noexcept
      {
         if (l.type() == r.type() &&
            l.hold() == r.hold() &&
            l.state() == r.state())
         {
            auto t = l.type();
            switch (t)
            {
               case INPUT_TYPES::INPUT_TYPE_KEY:
               {
                  return std::get<key_array>(l.m_button) ==
                     std::get<key_array>(r.m_button)
               }
               case INPUT_TYPES::INPUT_TYPE_XINPUT:
               {
                  return l.xi_button() == r.xi_button();
               }
               case INPUT_TYPES::INPUT_TYPE_GAMEPAD:
               {
                  return l.gp_button() == r.gp_button();
               }
               case INPUT_TYPES::INPUT_TYPE_AXIS:
               {
                  return l.axis() == r.axis();
               }
               case INPUT_TYPES::INPUT_TYPE_AXIS_2D:
               {
                  return l.axis_2d() == r.axis_2d();
               }
            }
         }

         return false;
      }

      friend bool operator!=(const input_trigger& l, const input_trigger& r)
      {
         return !(l == r);
      }

      private:
      INPUT_TYPES m_type;
      BUTTON_STATES m_state;
      TickT m_wait;

      std::variant<
         key_array, xinput_button, gamepad_button,
         input_axis, input_axis2d> m_button;
   };
}

namespace std
{
   template<typename TickT>
   struct hash<qgl::input::input_trigger<TickT>>
   {
      constexpr hash()
      {

      }

      typedef qgl::input::input_trigger<TickT> argument_type;
      typedef std::size_t result_type;
      constexpr result_type operator()(const argument_type& t) const noexcept
      {
         // First byte is the input type & button state.
         auto itbs = (static_cast<result_type>(t.state()) << 4) |
            static_cast<result_type>(t.type());

         // While TickT is probably much bigger than 20 bits, its unlikely that
         // a button will be held for more than a few seconds (1,000,000 ticks).
         auto hold = static_cast<result_type>(t.hold()) & 0x000F'FFFF;

         // There are 36 bits left:
         result_type ret = 0;
         using namespace qgl::input;
         switch (t.type())
         {
            case INPUT_TYPES::INPUT_TYPE_KEY:
            {
               break;
            }
            case INPUT_TYPES::INPUT_TYPE_GAMEPAD:
            {
               break;
            }
            case INPUT_TYPES::INPUT_TYPE_XINPUT:
            {
               break;
            }
            case INPUT_TYPES::INPUT_TYPE_AXIS:
            {
               break;
            }
            case INPUT_TYPES::INPUT_TYPE_AXIS_2D:
            {
               break;
            }
         }

         return (ret << 28) | itbs | hold;
      }
   };
}