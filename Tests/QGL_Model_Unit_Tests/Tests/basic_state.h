#pragma once

namespace QGL_Model_Unit_Tests
{
   template<class StateT>
   class basic_state
   {
      public:

      basic_state()
      {

      }

      basic_state(const basic_state& c) = default;

      basic_state(basic_state&& m) = default;

      virtual ~basic_state() noexcept = default;

      StateT& state() noexcept
      {
         return m_state;
      }

      const StateT& state() const noexcept
      {
         return m_state;
      }

      private:
      StateT m_state;
   };
}