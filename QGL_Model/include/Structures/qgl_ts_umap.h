#pragma once
#include "include/qgl_model_include.h"

namespace qgl
{
   template<class KeyT, class ValueT>
   class ts_umap final
   {
      public:
      using data_t = typename std::unordered_map<KeyT, ValueT>;
      using iterator = typename data_t::iterator;
      using const_iterator = typename data_t::const_iterator;

      ts_umap() :
         m_locked(false)
      {

      }

      ts_umap(const ts_umap& r)
      {
         // Prevent r from changing.
         std::scoped_lock l{ r.m_mutex, m_mutex };

         // Copy r
         m_data = r.m_data;
      }

      ts_umap(ts_umap&& r) noexcept
      {
         // Prevent r from changing.
         std::scoped_lock l{ r.m_mutex, m_mutex };

         // Move r's data
         m_data = std::move(r.m_data);
      }

      ~ts_umap() noexcept = default;

      /*
       Locks l and r from changing and exchanges their data.
       */
      friend void swap(ts_umap& l, ts_umap& r) noexcept
      {
         // Wait for l and r to be free.
         std::scoped_lock lk{ l.m_mutex, r.m_mutex };
         using std::swap;
         swap(l.m_data, r.m_data);
      }

      ts_umap& operator=(ts_umap r)
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
       Returns an iterator that points to the beginning of the map. The
       iterator can be invalidated by any modification operations. Prevent
       modifications from running by calling "lock" before calling this
       function. You must call "unlock" once done with the iterator.
       */
      iterator begin() noexcept
      {
         return m_data.begin();
      }

      /*
       Returns an iterator that points to the beginning of the map. The
       iterator can be invalidated by any modification operations. Prevent
       modifications from running by calling "lock" before calling this
       function. You must call "unlock" once done with the iterator.
       */
      const_iterator begin() const noexcept
      {
         return m_data.begin();
      }

      /*
       Returns an iterator that points to the beginning of the map. The
       iterator can be invalidated by any modification operations. Prevent
       modifications from running by calling "lock" before calling this
       function. You must call "unlock" once done with the iterator.
       */
      const_iterator cbegin() const noexcept
      {
         return m_data.cbegin();
      }

      /*
       Returns an iterator that points to the end of the map. The
       iterator can be invalidated by any modification operations.
       Call "lock"/"unlock" to prevent the iterator from being invalidated.
       */
      iterator end() noexcept
      {
         return m_data.end();
      }

      /*
       Returns an iterator that points to the end of the map. The
       iterator can be invalidated by any modification operations.
       Call "lock"/"unlock" to prevent the iterator from being invalidated.
       */
      const_iterator end() const noexcept
      {
         return m_data.end();
      }

      /*
       Returns an iterator that points to the end of the map. The
       iterator can be invalidated by any modification operations.
       Call "lock"/"unlock" to prevent the iterator from being invalidated.
       */
      const_iterator cend() const noexcept
      {
         return m_data.cend();
      }

      /*
       Prevents modifications and returns the number of times "value" is
       present in the map. Since a map can only contain 1 of an element, this
       returns 1 if the element is present. Otherwise it returns 0.
       */
      size_t count(const KeyT& k) const
      {
         // If locked
         if (m_locked.load())
         {
            return m_data.count(k);
         }
         else
         {
            // Acquire a lock
            std::scoped_lock l{ m_mutex };
            return m_data.count(k);
         }
      }

      /*
       Searches the map for "value" and returns an iterator that points to it.
       If the map does not contain "value", this returns "end()".
       This is NOT thread safe. You should call "lock" prior to "find" to keep
       other threads from invalidating the returned iterator.
       */
      iterator find(const KeyT& k)
      {
         // If locked
         if (m_locked.load())
         {
            return m_data.find(k);
         }
         else
         {
            // Acquire a lock
            std::scoped_lock l{ m_mutex };
            return m_data.find(k);
         }
      }

      /*
       Returns true if the map is empty the instant this is called with
       respect to thread accesses.
       */
      [[nodiscard]] bool empty() const
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
       Returns the number of elements in the map the instant this is called
       with respect to thread access.
       */
      size_t size() const
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
       Atomically clears the map. Depending on thread races, the map may not
       be empty once inspected after calling this.
       */
      void clear()
      {
         std::scoped_lock l{ m_mutex };
         m_data.clear();
      }

      /*
       Prevents simultaneous modifications and returns a reference to the value
       that is mapped to a key equivalent to key, performing an insertion if
       such key does not already exist.
       */
      ValueT& operator[](const KeyT& k)
      {
         if (m_locked.load())
         {
            return m_data[k];
         }
         else
         {
            // Acquire a lock
            std::scoped_lock l{ m_mutex };
            return m_data[k];
         }
      }

      /*
       Prevents simultaneous modifications and returns an element at "pos" with
       respect to thread accesses.
       If no such element exists, an exception of type std::out_of_range is 
       thrown.
       */
      ValueT& at(const KeyT& k)
      {
         if (m_locked.load())
         {
            return m_data.at(k);
         }
         else
         {
            // Acquire a lock
            std::scoped_lock l{ m_mutex };
            return m_data.at(k);
         }
      }

      const ValueT& at(const KeyT& k) const
      {
         if (m_locked.load())
         {
            return m_data.at(k);
         }
         else
         {
            // Acquire a lock
            std::scoped_lock l{ m_mutex };
            return m_data.at(k);
         }
      }

      /*
       Prevents simultaneous modifications and inserts the range of values into
       the map.
       */
      std::pair<iterator, bool> insert(const ValueT& value)
      {
         std::scoped_lock l{ m_mutex };
         return m_data.insert(value);
      }

      /*
       Prevents simultaneous modifications and inserts the range of values into
       the map.
       */
      std::pair<iterator, bool> insert(ValueT&& value)
      {
         std::scoped_lock l{ m_mutex };
         return m_data.insert(std::forward<ValueT>(value));
      }

      /*
       Prevents simultaneous modifications and inserts the range of values into
       the map.
       */
      void insert(std::initializer_list<ValueT> init)
      {
         std::scoped_lock l{ m_mutex };
         m_data.insert(init);
      }

      /*
       Prevents simultaneous modifications and inserts "value" into the map.
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
       Removes the element at "pos" from the map.
       This is NOT thread safe. Lock the map and get the correct iterator before
       calling this.
       */
      iterator erase(const_iterator pos)
      {
         return m_data.erase(pos);
      }

      /*
       Removes the range from the map.
       This is NOT thread safe. Lock the map and get the correct iterator before
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
      size_t erase(const KeyT& k)
      {
         std::scoped_lock l{ m_mutex };
         return m_data.erase(k);
      }

      private:
      mutable std::mutex m_mutex;
      data_t m_data;
      std::atomic_bool m_locked;
   };
}