#pragma once
#include "include/qgl_model_include.h"
#include "include/Commands/qgl_command_roll.h"
#include "include/Commands/qgl_command_locator.h"

namespace qgl
{
   enum class replay_controller_state
   {
      stopped,
      playing,
      paused
   };

   enum class replay_controller_direction
   {
      forward,
      reverse
   };

   /*
    Controls playback of recorded commands stored in a command_roll.
    This class steps through the roll and re-executes commands
    in timestamp order using the provided command_locator.
    */
   template<typename TickT, typename ExecuteArgs>
   class replay_controller final
   {
      public:
      using roll_type = command_roll<TickT>;
      using frame_type = command_frame<TickT>;
      using locator_type = command_locator<ExecuteArgs>;
      using fwd_iter = typename roll_type::const_iterator;
      using rev_iter = typename roll_type::const_riterator;

      replay_controller(roll_type* const roll_p,
                        locator_type* const locator_p) :
         m_roll_p(roll_p),
         m_locator_p(locator_p),
         m_replayClock(0),
         m_state(replay_controller_state::stopped),
         m_dir(replay_controller_direction::forward),
         m_fwdIt(roll_p->cend()),
         m_revIt(roll_p->crend())
      {}

      replay_controller(const replay_controller&) = default;
      replay_controller(replay_controller&&) = default;
      ~replay_controller() noexcept = default;

      void start(replay_controller_direction dir, TickT startAt = 0)
      {
         m_state = replay_controller_state::playing;
         m_replayClock = startAt;
         m_dir = dir;

         if (m_dir == replay_controller_direction::forward)
         {
            m_fwdIt = m_roll_p->cbegin();
         }
         else
         {
            m_revIt = m_roll_p->crbegin();
         }
      }

      void stop()
      {
         m_state = replay_controller_state::stopped;
         m_fwdIt = m_roll_p->cend();
         m_revIt = m_roll_p->crend();
      }

      void pause()
      {
         if (m_state == replay_controller_state::playing)
         {
            m_state = replay_controller_state::paused;
         }
      }

      void update(TickT elapsedTime, ExecuteArgs& args)
      {
         if (m_state != replay_controller_state::playing)
         {
            return;
         }

         m_replayClock = elapsedTime;

         if (m_dir == replay_controller_direction::forward)
         {
            while (m_fwdIt != m_roll_p->cend() &&
                   m_fwdIt->timestamp <= m_replayClock)
            {
               auto cmd = m_locator_p->get(m_fwdIt->id);
               cmd->execute(args);
               ++m_fwdIt;
            }
         }
         else // reverse
         {
            while (m_revIt != m_roll_p->crend() &&
                   m_revIt->timestamp >= m_replayClock)
            {
               auto cmd = m_locator_p->get(m_revIt->id);
               cmd->execute(args);
               ++m_revIt;
            }
         }
      }

      replay_controller_state state() const noexcept
      {
         return m_state;
      }

      replay_controller_direction direction() const noexcept
      {
         return m_dir;
      }

      TickT time() const noexcept
      {
         return m_replayClock;
      }

      private:
      roll_type* m_roll_p = nullptr;
      locator_type* m_locator_p = nullptr;
      TickT m_replayClock;
      replay_controller_state m_state;
      replay_controller_direction m_dir;
      fwd_iter m_fwdIt;
      rev_iter m_revIt;
   };
}