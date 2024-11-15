#pragma once
#include "include/qgl_input_include.h"
#include "include/qgl_input_state.h"
#include "include/Helpers/qgl_gamepad_helpers.h"
#include <vector>

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
      input_trigger(input_key k, button_states btnState, TickT wait) :
         m_type(input_types::key), m_state(btnState), m_wait(wait),
         m_button(std::move(std::vector<input_key>{k}))
      {
      }

      template<class InputKeyIterator>
      input_trigger(InputKeyIterator first, InputKeyIterator last,
         button_states btnState, TickT wait) :
         m_type(input_types::key), m_state(btnState), m_wait(wait),
         m_button(std::move(std::vector<input_key>{first, last}))
      {
      }

      /*
       Trigger for when the given buttons are in the given state for the given
       amount of time.
       Bitwise OR buttons together so the trigger is raised only if all
       buttons are pressed.
       */
      input_trigger(gamepad_button btns, button_states btnState, TickT wait) :
         m_type(input_types::gamepad), m_state(btnState),
         m_wait(wait), m_button(btns)
      {
      }

      /*
       Trigger for when the given buttons are in the given state for the given
       amount of time.
       Bitwise OR buttons together so the trigger is raised only if all
       buttons are pressed.
       */
      input_trigger(xinput_button btns, button_states btnState, TickT wait) :
         m_type(input_types::xinput), m_state(btnState),
         m_wait(wait), m_button(btns)
      {
      }

      /*
       Trigger for when the given axis is held at least "mag" amount for "wait"
       amount of time.
       */
      input_trigger(input_axis_ids axis, axis_magnitude mag, TickT wait) :
         m_type(input_types::axis),
         m_state(button_states::pressed), m_wait(wait),
         m_button(std::move(input_axis{ mag, axis }))
      {
      }

      input_trigger(const input_trigger&) = default;

      input_trigger(input_trigger&&) = default;

      ~input_trigger() noexcept = default;

      constexpr TickT hold() const noexcept
      {
         return m_wait;
      }

      constexpr input_types type() const noexcept
      {
         return m_type;
      }

      constexpr button_states state() const noexcept
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

      const input_key* key_data() const
      {
         return std::get<key_array>(m_button).data();
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
               case input_types::key:
               {
                  return std::get<key_array>(l.m_button) ==
                     std::get<key_array>(r.m_button)
               }
               case input_types::xinput:
               {
                  return l.xi_button() == r.xi_button();
               }
               case input_types::gamepad:
               {
                  return l.gp_button() == r.gp_button();
               }
               case input_types::axis:
               {
                  return l.axis() == r.axis();
               }
               case input_types::axis_2d:
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
      input_types m_type;
      button_states m_state;
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
         //[buttons: 32] [hold: 24] [state: 4] [type: 4]

         // First byte is the input type & button state.
         auto itbs = (static_cast<result_type>(t.state()) << 4) |
            static_cast<result_type>(t.type());

         // While TickT is probably much bigger than 24 bits, its unlikely that
         // a button will be held for more than a few seconds (1,000,000 ticks).
         auto hold = static_cast<result_type>(t.hold()) & 0x00FF'FFFF;

         // There are 32 bits left:
         result_type ret = 0;
         using namespace qgl::input;
         switch (t.type())
         {
            case input_types::key:
            {
               ret = qgl::fast_hash_32(t.key_data(),
                  t.key_count() * sizeof(qgl::input::input_key),
                  0x981430F1);
               break;
            }
            case input_types::gamepad:
            {
               ret = static_cast<result_type>(t.gp_button());
               break;
            }
            case input_types::xinput:
            {
               ret = t.xi_button();
               break;
            }
            case input_types::axis:
            {
               qgl::math::decimal<qgl::input::AXIS_DENOMINATOR> x{
                  t.axis().value()
               };

               ret = (static_cast<result_type>(t.axis().id()) <<
                     (sizeof(x.numerator()) * CHAR_BIT)) |
                  x.numerator();

               break;
            }
            case input_types::axis_2d:
            {
               // [ID: 8] [y: 12] [x: 12]
               qgl::math::decimal<qgl::input::AXIS_DENOMINATOR> x{
                  t.axis_2d().x()
               };

               qgl::math::decimal<qgl::input::AXIS_DENOMINATOR> y{
                  t.axis_2d().y()
               };

               auto id = static_cast<result_type>(t.axis_2d().id()) << 24;

               ret = id |
                  (static_cast<result_type>(y.numerator()) << 12) |
                  (static_cast<result_type>(x.numerator()));

               break;
            }
         }

         return (ret << 32) | (hold << 8) | itbs;
      }
   };
}