#pragma once
#include "include/qgl_input_include.h"
#include "include/qgl_input_trigger.h"
#include <QGLModel.h>

namespace qgl::input
{
   /*
    On calls to "update()", this dispatches inputs to an icommand.
    This class acts as the bridge between polling inputs and event-driven
    inputs.
    Set up input mappings by using "insert()".
    */
   template<class InputProvider, typename TickT>
   class input_dispatcher 
   {
      public:
      using command = typename icommand<const input_state&>;
      using command_ptr = typename std::shared_ptr<command>;
      using trigger_type = typename input_trigger<TickT>;

      input_dispatcher(InputProvider&& provider) :
         m_provider(std::forward<InputProvider>(provider))
      {}

      /*
       Do not allow multiple dispatchers.
       */
      input_dispatcher(const input_dispatcher&) noexcept = delete;

      /*
       Default move constructor.
       */
      input_dispatcher(input_dispatcher&&) noexcept = default;

      /*
       Destructor.
       */
      ~input_dispatcher() noexcept = default;

      /*
       Dispatches inputs from the input provider.
       Only input triggers that are mapped will be considered.
       */
      template<class ExecutionPolicy>
      void update(TickT delta, ExecutionPolicy policy = std::execution::seq)
      {
         using it_type = typename std::add_lvalue_reference<
            decltype(*m_map.begin())>::type;

         std::lock_guard lock{ m_currentUpdate };
         auto instance = m_provider.instance();
         std::for_each(m_map.cbegin(), m_map.cend(), [&](it_type p)
         {
            winrt::throw_hresult(E_NOTIMPL);
         });
      }

      /*
       Inserts a mapping so that when the given trigger is raised during
       "update()", the command is executed.
       The command's shared pointer is copied so this also owns a pointer to
       the command.
       */
      void insert(trigger_type&& trigger, command_ptr&& cmd) noexcept
      {
         std::lock_guard lock{ m_currentUpdate };
         m_map[std::forward<trigger_type>(trigger)] =
            std::make_tuple<command_ptr, TickT>(
               std::forward<command_ptr>(cmd),
               static_cast<TickT>(0));
      }

      /*
       Removes the icommand that is mapped by the given trigger.
       */
      void erase(trigger_type&& trigger)
      {
         std::lock_guard lock{ m_currentUpdate };
         m_map.erase(std::forward<trigger_type>(trigger));
      }

      /*
       Removes all icommands that would be dispatched.
       */
      void clear()
      {
         std::lock_guard lock{ m_currentUpdate };
         m_map.clear();
      }

      private:

      /*
       Maps an input trigger to a command to execute.
       */
      std::unordered_map<trigger_type, std::tuple<command_ptr, TickT>> m_map;

      InputProvider m_provider;

      std::mutex m_currentUpdate;
   };
}