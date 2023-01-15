#pragma once
#include "include/qgl_model_include.h"

namespace qgl
{
   /*
    Represents a thread-safe linked list.
    Inserts, accesses, and removals prevent modifications.
    */
   template<typename T>
   class ts_list final
   {
      public:
      using iterator = typename std::list<T>::iterator;
      using const_iterator = typename std::list<T>::const_iterator;

      /*
       Empty list.
       */
      ts_list() :
         m_locked(false)
      {

      }

      template<class InputIt>
      ts_list(InputIt first, InputIt last) :
         m_locked(false),
         m_data(first, last)
      {
         using ItType = typename std::remove_const<
            std::remove_reference<
            decltype(*first)>::type>::type;
         static_assert(std::is_same<ItType, T>::value,
                       "Dereferencing InputIt does not yield a T.");
      }

      ts_list(std::initializer_list<T> init) :
         m_locked(false),
         m_data(init)
      {

      }

      /*
       Copy constructor
       */
      ts_list(const ts_list& r)
      {
         // Prevent r and this from changing.
         std::scoped_lock l{ r.m_mutex, m_mutex };

         // Copy r's data
         m_data = r.m_data;
      }

      /*
       Move constructor
       */
      ts_list(ts_list&& r) noexcept
      {
         // Prevent r and this from changing.
         std::scoped_lock l{ r.m_mutex, m_mutex };

         // Move r's data.
         m_data = std::move(r.m_data);
      }

      /*
       Destructor
       */
      ~ts_list() noexcept = default;

      /*
       Locks this so it cannot be changed until "unlock" is called.
       While locked, the only operations permitted are iterator and element
       access.
       The caller must call "unlock" to prevent a deadlock.
       */
      void lock()
      {
         m_mutex.lock();
         m_locked.store(true);
      }

      /*
       Unlocks the container so it can be modified.
       */
      void unlock()
      {
         m_mutex.unlock();
         m_locked.store(false);
      }

      /*
       Locks l and r from changing and exchanges their data.
       */
      friend void swap(ts_list& l, ts_list& r) noexcept
      {
         // Wait for l and r to be free.
         std::scoped_lock lk{ l.m_mutex, r.m_mutex };
         using std::swap;
         swap(l.m_data, r.m_data);
      }

      ts_list& operator=(ts_list r) noexcept
      {
         swap(*this, r);
         return *this;
      }

      /*
       Prevents any simultaneous insertions and returns the front element at
       this instant.
       */
      T& front()
      {
         // If locked:
         if (m_locked.load())
         {
            return m_data.front();
         }
         else
         {
            // Acquire a lock
            std::scoped_lock l{ m_mutex };
            return m_data.front();
         }
      }

      /*
       Prevents any simultaneous insertions and returns the front element at
       this instant.
       */
      const T& front() const
      {
         // If locked:
         if (m_locked.load())
         {
            return m_data.front();
         }
         else
         {
            // Acquire a lock
            std::scoped_lock l{ m_mutex };
            return m_data.front();
         }
      }

      /*
       Prevents any simultaneous insertions and returns the last element at
       this instant.
       */
      T& back()
      {
         // If locked
         if (m_locked.load())
         {
            return m_data.back();
         }
         else
         {
            // Acquire a lock
            std::scoped_lock l{ m_mutex };
            return m_data.back();
         }
      }

      /*
       Prevents any simultaneous insertions and returns the last element at
       this instant.
       */
      const T& back() const
      {
         // If locked
         if (m_locked.load())
         {
            return m_data.back();
         }
         else
         {
            // Acquire a lock
            std::scoped_lock l{ m_mutex };
            return m_data.back();
         }
      }

      /*
       Returns an iterator that points to the beginning of the list. The
       iterator can be invalidated by any modification operations. Prevent
       modifications from running by calling "lock" before calling this
       function. You must call "unlock" once done with the iterator.
       */
      iterator begin() noexcept
      {
         return m_data.begin();
      }

      /*
       Returns an iterator that points to the end of the list. The
       iterator can be invalidated by any modification operations.
       Call "lock"/"unlock" to prevent the iterator from being invalidated.
       */
      iterator end() noexcept
      {
         return m_data.end();
      }

      /*
       Returns an iterator that points to the beginning of the list. The
       iterator can be invalidated by any modification operations. Prevent
       modifications from running by calling "lock" before calling this
       function. You must call "unlock" once done with the iterator.
       */
      const_iterator begin() const noexcept
      {
         return m_data.begin();
      }

      /*
       Returns an iterator that points to the end of the list. The
       iterator can be invalidated by any modification operations.
       Call "lock"/"unlock" to prevent the iterator from being invalidated.
       */
      const_iterator end() const noexcept
      {
         return m_data.end();
      }

      /*
       Returns an iterator that points to the beginning of the list. The
       iterator can be invalidated by any modification operations. Prevent
       modifications from running by calling "lock" before calling this
       function. You must call "unlock" once done with the iterator.
       */
      const_iterator cbegin() const noexcept
      {
         return m_data.cbegin();
      }

      /*
       Returns an iterator that points to the end of the list. The
       iterator can be invalidated by any modification operations.
       Call "lock"/"unlock" to prevent the iterator from being invalidated.
       */
      const_iterator cend() const noexcept
      {
         return m_data.cend();
      }

      /*
       Returns true if the list is empty the instant this is called with
       respect to thread accesses.
       */
      [[nodiscard]] bool empty() const noexcept
      {
         // If locked
         if (m_locked.load())
         {
            return m_data.empty();
         }
         else
         {
            // Acquire a lock
            std::scoped_lock l{ m_mutex };
            return m_data.empty();
         }
      }

      /*
       Returns the number of elements in the list the instant this is called
       with respect to thread access.
       */
      [[nodiscard]] size_t size() const noexcept
      {
         // If locked
         if (m_locked.load())
         {
            return m_data.size();
         }
         else
         {
            // Acquire a lock
            std::scoped_lock l{ m_mutex };
            return m_data.size();
         }
      }

      /*
       Atomically clears the list. Depending on thread races, the list may not
       be empty once inspected after calling this.
       */
      void clear() noexcept
      {
         std::scoped_lock l{ m_mutex };
         m_data.clear();
      }

      /*
       Prevents any other modifications and inserts "val" to the end of the
       list. This ensures the list is ordered in the exact order that threads
       call this function.
       */
      void push_back(const T& val)
      {
         std::scoped_lock l{ m_mutex };
         m_data.push_back(val);
      }

      /*
       Prevents any other modifications and inserts "val" to the end of the
       list. This ensures the list is ordered in the exact order that threads
       call this function.
       */
      void push_back(T&& val)
      {
         std::scoped_lock l{ m_mutex };
         m_data.push_back(std::forward<T>(val));
      }

      /*
       Prevents any other modifications and removes the last element from the
       list. This ensures the list is ordered in the exact order that threads
       call this function.
       */
      void pop_back()
      {
         std::scoped_lock l{ m_mutex };
         m_data.pop_back();
      }

      /*
       Prevents any other modifications and inserts "val" to the front of the
       list. This ensures the list is ordered in the exact order that threads
       call this function.
       */
      void push_front(const T& val)
      {
         std::scoped_lock l{ m_mutex };
         m_data.push_front(val);
      }

      /*
       Prevents any other modifications and inserts "val" to the front of the
       list. This ensures the list is ordered in the exact order that threads
       call this function.
       */
      void push_front(T&& val)
      {
         std::scoped_lock l{ m_mutex };
         m_data.push_front(std::forward<T>(val));
      }

      /*
       Prevents any other modifications and removes the first element from the
       list. This ensures the list is ordered in the exact order that threads
       call this function.
       */
      void pop_front()
      {
         std::scoped_lock l{ m_mutex };
         m_data.pop_front();
      }

      /*
       Atomically pops the front item and returns it.
       */
      T gpop_front()
      {
         std::scoped_lock l{ m_mutex };
         auto ret = m_data.front();
         m_data.pop_front();
         return ret;
      }

      /*
       Atomically pops the back item and returns it.
       */
      T gpop_back()
      {
         std::scoped_lock l{ m_mutex };
         auto ret = m_data.back();
         m_data.pop_back();
         return ret;
      }

      template<class... Args>
      T& emplace_back(Args&&... args)
      {
         std::scoped_lock l{ m_mutex };
         return m_data.emplace_back(std::forward<Args>(args)...);
      }

      /*
       Removes the element at "pos" from the list.
       This is NOT thread safe. Lock the list and get the correct iterator before
       calling this.
       */
      iterator erase(const_iterator pos)
      {
         return m_data.erase(pos);
      }

      /*
       Removes the range from the list.
       This is NOT thread safe. Lock the list and get the correct iterator before
       calling this.
       */
      iterator erase(const_iterator first, const_iterator last)
      {
         return m_data.erase(first, last);
      }

      /*
       Prevents simultaneous modifications and removes the element equal to 
       "value" from the list.
       */
      void remove(const T& value)
      {
         std::scoped_lock l{ m_mutex };
         m_data.remove(value);
      }
      
      /*
       Prevents simultaneous modifications and removes all elements that match
       the predicate "p".
       */
      template<class UnaryPredicate>
      void remove_if(UnaryPredicate p)
      {
         std::scoped_lock l{ m_mutex };
         m_data.remove_if(p);
      }

      private:
      mutable std::mutex m_mutex;
      std::list<T> m_data;
      std::atomic_bool m_locked;
   };
}