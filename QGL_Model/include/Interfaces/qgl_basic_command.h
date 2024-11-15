#pragma once
#include "include/qgl_model_include.h"
#include "include/Interfaces/qgl_icommand.h"

namespace qgl
{
   /*
    A basic command is a wrapper for some state where the state is updated
    using "execute" and can be restored using "undo".
    */
   template<class State>
   class basic_command : public icommand<State>
   {
      public:
      static_assert(std::is_default_constructible_v<State>,
                    "State must be default constructible");

      basic_command(const qgl::guid& g, State initialState = State()) :
         icommand(g),
         m_curState(initialState)
      {

      }

      basic_command(qgl::guid&& g, State initialState = State()) :
         icommand(std::forward<qgl::guid>(g)),
         m_curState(initialState)
      {

      }

      basic_command(const basic_command&) = default;

      basic_command(basic_command&&) = default;

      virtual ~basic_command() noexcept = default;

      virtual void execute(State& args)
      {
         m_lastState = m_curState;
         m_curState = args;
      }

      virtual void undo()
      {
         m_curState = m_lastState;
      }

      State& state() noexcept
      {
         return m_curState;
      }

      const State& state() const noexcept
      {
         return m_curState;
      }

      private:
      State m_curState;
      State m_lastState;
   };
}