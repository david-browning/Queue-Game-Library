#pragma once
#include "include/qgl_model_include.h"

namespace qgl
{
   /*
    Represents a thread-safe vector.
    Inserts, accesses, and removals prevent modifications.
    */
   template<typename T>
   class ts_vector final
   {
      public:
      using data_t = typename std::vector<T>;
      using iterator = typename data_t::iterator;
      using const_iterator = typename data_t::const_iterator;

      /*
       Empty vector.
       */
      ts_vector() :
         m_locked(false)
      {

      }

      /*
       Copies the elements in the range first-last.
       */
      template<typename InputIt>
      ts_vector(InputIt first, InputIt last) :
         m_locked(false),
         m_data(first, last)
      {

      }

      ts_vector(std::initializer_list<T> init) :
         m_locked(false),
         m_data(init)
      {

      }

      ts_vector(const ts_vector& r) 
      {
         // Prevent r from changing.
         std::scoped_lock l{ r.m_mutex, m_mutex };

         // Copy r
         m_data = r.m_data;
      }

      ts_vector(ts_vector&& r) noexcept
      {
         // Prevent r from changing.
         std::scoped_lock l{ r.m_mutex, m_mutex };

         // Move r's data
         m_data = std::move(r.m_data);
      }

      /*
       Destructor
       */
      ~ts_vector() noexcept = default;

      /*
       Locks l and r from changing and exchanges their data.
       */
      friend void swap(ts_vector& l, ts_vector& r) noexcept
      {
         // Wait for l and r to be free.
         std::scoped_lock lk{ l.m_mutex, r.m_mutex };
         using std::swap;
         swap(l.m_data, r.m_data);
      }

      /*
       Copies r into this.
       */
      ts_vector& operator=(ts_vector r) noexcept
      {
         swap(*this, r);
         return *this;
      }

      /*
       Returns a pointer to the underlying array. This is not thread safe.
       To prevent this from changing, call "lock" and "unlock".
       */
      T* data() noexcept
      {
         return m_data.data();
      }

      /*
       Returns a pointer to the underlying array. This is not thread safe.
       To prevent this from changing, call "lock" and "unlock".
       */
      const T* data() const noexcept
      {
         return m_data.data();
      }

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
       Returns an iterator that points to the beginning of the vector. The
       iterator can be invalidated by any modification operations. Prevent
       modifications from running by calling "lock" before calling this
       function. You must call "unlock" once done with the iterator.
       */
      iterator begin() noexcept
      {
         return m_data.begin();
      }

      /*
       Returns an iterator that points to the beginning of the vector. The
       iterator can be invalidated by any modification operations. Prevent
       modifications from running by calling "lock" before calling this
       function. You must call "unlock" once done with the iterator.
       */
      const_iterator begin() const noexcept
      {
         return m_data.begin();
      }

      /*
       Returns an iterator that points to the beginning of the vector. The
       iterator can be invalidated by any modification operations. Prevent
       modifications from running by calling "lock" before calling this
       function. You must call "unlock" once done with the iterator.
       */
      const_iterator cbegin() const noexcept
      {
         return m_data.cbegin();
      }

      /*
       Returns an iterator that points to the end of the vector. The
       iterator can be invalidated by any modification operations.
       Call "lock"/"unlock" to prevent the iterator from being invalidated.
       */
      iterator end() noexcept
      {
         return m_data.end();
      }

      /*
       Returns an iterator that points to the end of the vector. The
       iterator can be invalidated by any modification operations.
       Call "lock"/"unlock" to prevent the iterator from being invalidated.
       */
      const_iterator end() const noexcept
      {
         return m_data.end();
      }

      /*
       Returns an iterator that points to the end of the vector. The
       iterator can be invalidated by any modification operations.
       Call "lock"/"unlock" to prevent the iterator from being invalidated.
       */
      const_iterator cend() const noexcept
      {
         return m_data.cend();
      }

      /*
       Prevents simultaneous modifications and returns an element at "pos" with 
       respect to thread accesses.
       */
      T& at(size_t pos)
      {
         if (m_locked.load())
         {
            return m_data.at(pos);
         }
         else
         {
            // Acquire a lock
            std::scoped_lock l{ m_mutex };
            return m_data.at(pos);
         }
      }

      /*
       Prevents simultaneous modifications and returns an element at "pos" with
       respect to thread accesses.
       */
      const T& at(size_t pos) const
      {
         if (m_locked.load())
         {
            return m_data.at(pos);
         }
         else
         {
            // Acquire a lock
            std::scoped_lock l{ m_mutex };
            return m_data.at(pos);
         }
      }

      /*
       Prevents simultaneous modifications and returns an element at "pos" with
       respect to thread accesses.
       */
      T& operator[](size_t pos)
      {
         if (m_locked.load())
         {
            return m_data[pos];
         }
         else
         {
            // Acquire a lock
            std::scoped_lock l{ m_mutex };
            return m_data[pos];
         }
      }

      /*
       Prevents simultaneous modifications and returns an element at "pos" with
       respect to thread accesses.
       */
      const T& operator[](size_t pos) const
      {
         if (m_locked.load())
         {
            return m_data[pos];
         }
         else
         {
            // Acquire a lock
            std::scoped_lock l{ m_mutex };
            return m_data[pos];
         }
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
       Returns true if the vector is empty the instant this is called with
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
       Atomically clears the vector. Depending on thread races, the vector may 
       not be empty once inspected after calling this.
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
      void push_back(const T& value)
      {
         std::scoped_lock l{ m_mutex };
         m_data.push_back(value);
      }

      /*
       Prevents any other modifications and inserts "val" to the end of the
       list. This ensures the list is ordered in the exact order that threads
       call this function.
       */
      void push_back(T&& value)
      {
         std::scoped_lock l{ m_mutex };
         m_data.push_back(std::forward<T>(value));
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
       Prevents any other modifications and removes the element at position 
       "pos". This ensures the vector is ordered in the exact order that threads
       call this function.
       */
      iterator erase(const_iterator pos)
      {
         std::scoped_lock l{ m_mutex };
         return m_data.erase(pos);
      }

      /*
       Prevents any other modifications and removes the element at position
       "pos". This ensures the vector is ordered in the exact order that threads
       call this function.
       */
      iterator erase(const_iterator first, const_iterator last)
      {
         std::scoped_lock l{ m_mutex };
         return m_data.erase(first , last);
      }

      /*
       Prevents any other modifications and resizes the vector to have "count"
       elements.
       */
      void resize(size_t count)
      {
         std::scoped_lock l{ m_mutex };
         m_data.resize(count);
      }

      /*
       Prevents any other modifications and resizes the vector to have "count"
       elements.
       */
      void resize(size_t count, const T& value)
      {
         std::scoped_lock l{ m_mutex };
         m_data.resize(count, value);
      }

      private:
      mutable std::mutex m_mutex;
      data_t m_data;
      std::atomic_bool m_locked;
   };
}