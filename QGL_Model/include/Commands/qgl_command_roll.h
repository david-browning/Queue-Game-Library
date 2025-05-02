#pragma once
#include "include/qgl_model_include.h"
#include "include/qgl_guid.h"
#include "include/Commands/qgl_icommand.h"
#include "include/Commands/qgl_command_frame.h"

namespace qgl
{
   /*
    Stored commands in the order they are recorded so its entirely possible that
    commands could be played back in a non-chronological order. The underlying
    data structure can always be sorted by timestamp by calling "sort()"
    */
   template<typename TickT>
   class command_roll
   {
      public:
      using container = typename std::vector<command_frame<TickT>>;
      using iterator = typename container::iterator;
      using const_iterator = typename container::const_iterator;
      using riterator = typename container::reverse_iterator;
      using const_riterator = typename container::const_reverse_iterator;

      command_roll()
      {

      }

      command_roll(command_roll& r) = default;

      command_roll(command_roll&&) = default;

      ~command_roll() noexcept = default;

      friend void swap(command_roll& r, command_roll& l) noexcept
      {
         using std::swap;
         swap(l.m_frames, l.m_frames);
      }

      command_roll& operator=(command_roll r) noexcept
      {
         swap(*this, r);
         return *this;
      }

      /*
       Adds a command to the queue.
       */
      void record(const guid& id, TickT timestamp)
      {
         m_frames.emplace_back(id, timestamp);
      }

      /*
       Returns the number of commands that have been queued.
       */
      size_t size() const noexcept
      {
         return m_frames.size();
      }

      /*
       Clears the internal storage.
       */
      void clear()
      {
         m_frames.clear();
      }

      /*
       Sorts all the queued commands so they are in order by time stamp.
       This means the newest command will be at the back.
       */
      void sort()
      {
         std::sort(
            m_frames.begin(), m_frames.end(), [](const auto& a, const auto& b)
         {
            return a.timestamp < b.timestamp;
         });
      }

      /*
       Removes the last command that was added to the roll.
       */
      void pop_back()
      {
         m_frames.pop_back();
      }

      iterator begin()
      {
         return m_frames.begin();
      }

      iterator end()
      {
         return m_frames.end();
      }

      riterator rbegin()
      {
         return m_frames.rbegin();
      }

      riterator rend()
      {
         return m_frames.rend();
      }

      const_iterator begin() const
      {
         return m_frames.begin();
      }

      const_iterator end() const
      {
         return m_frames.end();
      }

      const_riterator rbegin() const
      {
         return m_frames.rbegin();
      }

      const_riterator rend() const
      {
         return m_frames.rend();
      }

      const_iterator cbegin() const
      {
         return m_frames.cbegin();
      }

      const_iterator cend() const
      {
         return m_frames.cend();
      }

      const_riterator crbegin() const
      {
         return m_frames.crbegin();
      }

      const_riterator crend() const
      {
         return m_frames.crend();
      }

      private:
      container m_frames;
   };
}