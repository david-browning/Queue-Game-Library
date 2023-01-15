#pragma once
#include "include/qgl_model_include.h"

namespace qgl
{
   template<
      class Key,
      class Hash = std::hash<Key>,
      class KeyEqual = std::equal_to<Key>,
      class Allocator = std::allocator<Key>>
   class ts_uset final
   {
      public:
      using data_t = typename std::unordered_set<Key, Hash, KeyEqual, Allocator>;
      using iterator = typename data_t::iterator;
      using const_iterator = typename data_t::const_iterator;

      ts_uset() :
         m_locked(false)
      {

      }

      template<class InputIt>
      ts_uset(InputIt first,
              InputIt last) :
         m_locked(false),
         m_data(first, last)
      {

      }

      ts_uset(std::initializer_list<Key> init) :
         m_locked(false),
         m_data(init)
      {

      }

      ts_uset(const ts_uset& r)
      {
         // Prevent r from changing.
         std::scoped_lock l{ r.m_mutex, m_mutex };

         // Copy r
         m_data = r.m_data;
      }

      ts_uset(ts_uset&& r) noexcept
      {
         // Prevent r from changing.
         std::scoped_lock l{ r.m_mutex, m_mutex };

         // Move r's data
         m_data = std::move(r.m_data);
      }

      ~ts_uset() noexcept = default;

      /*
       Locks l and r from changing and exchanges their data.
       */
      friend void swap(ts_uset& l, ts_uset& r) noexcept
      {
         // Wait for l and r to be free.
         std::scoped_lock lk{ l.m_mutex, r.m_mutex };
         using std::swap;
         swap(l.m_data, r.m_data);
      }

      ts_uset& operator=(ts_uset r)
      {
         swap(*this, r);
         return *this;
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
       Returns an iterator that points to the beginning of the set. The
       iterator can be invalidated by any modification operations. Prevent
       modifications from running by calling "lock" before calling this
       function. You must call "unlock" once done with the iterator.
       */
      iterator begin() noexcept
      {
         return m_data.begin();
      }

      /*
       Returns an iterator that points to the beginning of the set. The
       iterator can be invalidated by any modification operations. Prevent
       modifications from running by calling "lock" before calling this
       function. You must call "unlock" once done with the iterator.
       */
      const_iterator begin() const noexcept
      {
         return m_data.begin();
      }

      /*
       Returns an iterator that points to the beginning of the set. The
       iterator can be invalidated by any modification operations. Prevent
       modifications from running by calling "lock" before calling this
       function. You must call "unlock" once done with the iterator.
       */
      const_iterator cbegin() const noexcept
      {
         return m_data.cbegin();
      }

      /*
       Returns an iterator that points to the end of the set. The
       iterator can be invalidated by any modification operations.
       Call "lock"/"unlock" to prevent the iterator from being invalidated.
       */
      iterator end() noexcept
      {
         return m_data.end();
      }

      /*
       Returns an iterator that points to the end of the set. The
       iterator can be invalidated by any modification operations.
       Call "lock"/"unlock" to prevent the iterator from being invalidated.
       */
      const_iterator end() const noexcept
      {
         return m_data.end();
      }

      /*
       Returns an iterator that points to the end of the set. The
       iterator can be invalidated by any modification operations.
       Call "lock"/"unlock" to prevent the iterator from being invalidated.
       */
      const_iterator cend() const noexcept
      {
         return m_data.cend();
      }

      /*
       Returns true if the set is empty the instant this is called with
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
       Returns the number of elements in the set the instant this is called
       with respect to thread access.
       */
      size_t size() const noexcept
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
       Atomically clears the set. Depending on thread races, the set may not
       be empty once inspected after calling this.
       */
      void clear() noexcept
      {
         std::scoped_lock l{ m_mutex };
         m_data.clear();
      }

      /*
       Prevents simultaneous modifications and inserts "value" into the set.
       Depending on thread accesses, the returned iterator may be invalidated
       by the time it is inspected.
       */
      std::pair<iterator, bool> insert(const Key& value)
      {
         std::scoped_lock l{ m_mutex };
         return m_data.insert(value);
      }

      /*
       Prevents simultaneous modifications and inserts "value" into the set.
       Depending on thread accesses, the returned iterator may be invalidated
       by the time it is inspected.
       */
      std::pair<iterator, bool> insert(Key&& value)
      {
         std::scoped_lock l{ m_mutex };
         return m_data.insert(std::forward<Key>(value));
      }

      /*
       Prevents simultaneous modifications and inserts the range into the set.
       */
      template<class InputIt>
      void insert(InputIt first, InputIt last)
      {
         std::scoped_lock l{ m_mutex };
         m_data.insert(first, last);
      }

      /*
       Prevents simultaneous modifications and constructs an item into the set.
       Depending on thread accesses, the returned iterator may be invalidated
       by the time it is inspected.
       */
      template<class... Args>
      std::pair<iterator, bool> emplace(Args&&... args)
      {
         std::scoped_lock l{ m_mutex };
         return m_data.emplace(std::forward<Args>(args)...);
      }

      /*
       Removes the element at "pos" from the set.
       This is NOT thread safe. Lock the set and get the correct iterator before
       calling this.
       */
      iterator erase(const_iterator pos)
      {
         return m_data.erase(pos);
      }

      /*
       Removes the range from the set.
       This is NOT thread safe. Lock the set and get the correct iterator before
       calling this.
       */
      iterator erase(const_iterator first, const_iterator last)
      {
         return m_data.erase(first, last);
      }

      /*
       Prevents simultaneous modifications and removes any elements equal to
       "value". 
       Returns the number of elements removed, either 0 or 1.
       */
      size_t erase(const Key& value)
      {
         std::scoped_lock l{ m_mutex };
         return m_data.erase(value);
      }

      /*
       Prevents modifications and returns the number of times "value" is 
       present in the set. Since a set can only contain 1 of an element, this
       returns 1 if the element is present. Otherwise it returns 0.
       */
      size_t count(const Key& value) const
      {
         // If locked
         if (m_locked.load())
         {
            return m_data.count(value);
         }
         else
         {
            // Acquire a lock
            std::scoped_lock l{ m_mutex };
            return m_data.count(value);
         }
      }

      /*
       Searches the set for "value" and returns an iterator that points to it.
       If the set does not contain "value", this returns "end()".
       This is NOT thread safe. You should call "lock" prior to "find" to keep
       other threads from invalidating the returned iterator.
       */
      iterator find(const Key& value) 
      {
         // If locked
         if (m_locked.load())
         {
            return m_data.find(value);
         }
         else
         {
            // Acquire a lock
            std::scoped_lock l{ m_mutex };
            return m_data.find(value);
         }
      }

      /*
       Searches the set for "value" and returns an iterator that points to it.
       If the set does not contain "value", this returns "end()".
       This is NOT thread safe. You should call "lock" prior to "find" to keep
       other threads from invalidating the returned iterator.
       */
      const_iterator find(const Key& value) const
      {
         // If locked
         if (m_locked.load())
         {
            return m_data.find(value);
         }
         else
         {
            // Acquire a lock
            std::scoped_lock l{ m_mutex };
            return m_data.find(value);
         }
      }

      private:
      mutable std::mutex m_mutex;
      std::unordered_set<Key, Hash, KeyEqual, Allocator> m_data;
      std::atomic_bool m_locked;
   };
}