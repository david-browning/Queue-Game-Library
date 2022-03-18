#pragma once
#include "include/qgl_input_include.h"
#include "include/Input/qgl_input_state.h"
#include "include/Input/qgl_input_trigger.h"

namespace qgl::input
{
   /*
    Holds the inputs that occured.
    */
   class input_instance final
   {
      public:
      using container = typename std::vector<input_state>;
      using iterator = typename container::iterator;
      using const_iterator = typename container::const_iterator;

      /*
       No inputs.
       */
      input_instance()
      {

      }

      template<class InputIt>
      input_instance(InputIt first, InputIt last) :
         m_inputs(first, last)
      {

      }

      input_instance(const input_instance&) noexcept = default;

      input_instance(input_instance&&) noexcept = default;

      ~input_instance() noexcept = default;

      iterator begin() noexcept
      {
         return m_inputs.begin();
      }

      iterator end() noexcept
      {
         return m_inputs.end();
      }

      const_iterator cbegin() const noexcept
      {
         return m_inputs.cbegin();
      }

      const_iterator cend() const noexcept
      {
         return m_inputs.cend();
      }

      private:
      container m_inputs;
   };
}