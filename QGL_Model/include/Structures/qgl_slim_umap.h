#pragma once
#include "include/qgl_model_include.h"
#include "include/Threads/qgl_srw_traits.h"
#include <memory>
#include <utility>
#include <tuple>

namespace qgl
{
   /*
    A slim umap uses a slim reader writer lock to control access to the
    unordered map. Write operations, such as those that insert or remove 
    elements from the map will set an exclusive lock.

    While in exclusive lock mode, no other thread can read or write to the
    map. While in shared lock mode, any thread can read, but in order to
    modify the map, all read operations must finish.

    Attempting to acquire an exclusive lock will block the current thread until
    all shared and exclusive operations are finished.

    Attempting to acquire a shared lock will block the current thread until
    all exclusive operations are finished.

    Accessing elements will return an std::pair that includes a reference to
    the element and a copy of the SRW lock. A copy of the lock is returned so
    that once it goes out of scope, the shared lock is decremented automatically.
    The caller should not modify the returned lock and let the destructor
    handle cleaning up access to the resource.
    */
   template<
      class Key,
      class T,
      class SRWTraits = qgl::srw_traits,
      class Hash = std::hash<Key>,
      class KeyEqual = std::equal_to<Key>>
   class slim_umap final
   {
      public:
      using map_type = typename std::unordered_map<Key, T, Hash, KeyEqual>;
      using insert_type = typename std::pair<const Key, T>;
      using value_type = typename std::pair<T&, SRWTraits>;
      using const_value_type = typename std::pair<const T&, SRWTraits>;

      template<bool IsConst>
      class basic_iterator final
      {
         public:
         using value_type = typename std::tuple<Key, T, SRWTraits>;
         using reference = typename std::conditional<IsConst, const value_type&, value_type&>::type;
         using pointer = typename std::conditional<IsConst, const value_type*, value_type*>::type;

         basic_iterator(const SRWTraits& traits);

         basic_iterator(const basic_iterator& r);

         basic_iterator(basic_iterator&& r);

         ~basic_iterator() noexcept;

         friend void swap(basic_iterator& l, basic_iterator& r);

         basic_iterator& operator=(basic_iterator r);

         reference operator*() const;

         pointer operator->() const;

         basic_iterator& operator++();

         bool operator==(const basic_iterator& r) const;

         bool operator!=(const basic_iterator& r) const;

         private:
      };

      using iterator = typename basic_iterator<false>;
      using const_iterator = typename basic_iterator<true>;

      slim_umap(SRWTraits traits = SRWTraits()) :
         m_traits(traits)
      {

      }

      slim_umap(const slim_umap& r)
      {
         r.m_traits.share_lock();
         m_map = r.m_map;
         r.m_traits.share_release();
      }

      slim_umap(slim_umap&& r)
      {
         r.m_traits.excl_lock();
         m_map = r.m_map;
         r.m_traits.excl_release();
      }

      ~slim_umap()
      {
         m_traits.excl_lock();
      }

      friend void swap(slim_umap& l, slim_umap& r)
      {
         l.m_traits.excl_lock();
         r.m_traits.excl_lock();

         using std::swap;
         swap(l.m_map, r.m_map);
         swap(l.m_traits, r.m_traits);

         l.m_traits.excl_release();
         r.m_traits.excl_release();
      }

      slim_umap& operator=(slim_umap r)
      {
         swap(*this, r);
         return *this;
      }

      const_iterator begin() const
      {
         return cbegin();
      }

      const_iterator end() const
      {
         return cend();
      }

      const_iterator cbegin() const;

      const_iterator cend() const;

      [[nodiscard]] bool empty() const
      {
         m_traits.share_lock();
         auto ret = m_map.empty();
         m_traits.share_release();

         return ret;
      }

      [[nodiscard]] size_t size() const
      {
         m_traits.share_lock();
         auto ret = m_map.size();
         m_traits.share_release();

         return ret;
      }

      /*
       Acquires an exclusive lock and clears all elements from the map.
       std::unordered_map's clear function is linear complexity.
       */
      void clear()
      {
         m_traits.excl_lock();
         m_map.clear();
         m_traits.excl_release();
      }

      /*
       Acquires a shared lock and returns a reference to the mapped value of
       the element with key equivalent to key. If no such element exists,
       an exception of type std::out_of_range is thrown.
       */
      value_type at(const Key& k)
      {
         SRWTraits sharedLock{ m_traits };
         sharedLock.share_lock();
         if (m_map.count(k) == 0)
         {
            throw std::out_of_range{ "Key is not in the map." };
         }

         return std::pair<T&, SRWTraits>(m_map[k], sharedLock);
      }

      /*
       Acquires a shared lock and returns a reference to the mapped value of
       the element with key equivalent to key. If no such element exists,
       an exception of type std::out_of_range is thrown.
       */
      const_value_type at(const Key& k) const
      {
         SRWTraits sharedLock{ m_traits };
         sharedLock.share_lock();
         if (m_map.count(k) == 0)
         {
            throw std::out_of_range{ "Key is not in the map." };
         }

         return std::pair<const T&, SRWTraits>{m_map[k], sharedLock};
      }

      /*
       Acquires an exclusive lock and returns a reference to the value that is
       mapped to a key equivalent to key, performing an insertion if such key
       does not already exist.
       */
      value_type operator[](const Key& k);

      /*
       Acquires an exclusive lock and returns a reference to the value that is
       mapped to a key equivalent to key, performing an insertion if such key
       does not already exist.
       */
      value_type operator[](Key&& k);

      /*
       Returns the number of elements with key that compares equal to the
       specified argument key, which is either 1 or 0 since this container does
       not allow duplicates.
       */
      size_t count(const Key& k) const
      {
         m_traits.share_lock();
         auto ret = m_map.count(k);
         m_traits.share_release();
         return ret;
      }

      /*
       Finds an element with key equivalent to key.
       */
      const_iterator find(const Key& k) const;

      /*
       Returns the number of elements removed (0 or 1)
       */
      size_t erase(const Key& key)
      {
         m_traits.excl_lock();
         auto ret = m_map.erase(key);
         m_traits.excl_release();
         return ret;
      }

      /*
       Acquires a shared lock and returns the average number of elements per
       bucket, that is, size() divided by bucket_count().
       */
      float load_factor() const
      {
         m_traits.share_lock();
         auto ret = m_map.load_factor();
         m_traits.share_release();
         return ret;
      }

      /*
       Acquires a shared lock and returns current maximum load factor.
       */
      float max_load_factor() const
      {
         return m_map.max_load_factor();
      }

      /*
       Acquires am exclusive lock and sets the maximum load factor.
       If the passed load factor is less than the current load factor, this will
       trigger a rehash.
       */
      void max_load_factor(float ml)
      {
         m_traits.excl_lock();
         m_map.max_load_factor(ml);
         m_traits.excl_release();
      }

      /*
       Acquires an exclusive lock and sets the number of buckets to count and
       rehashes the container.

       If the new number of buckets makes load factor more than maximum load
       factor (count < size() / max_load_factor()), then the new number of
       buckets is at least size() / max_load_factor().
       */
      void rehash(size_t count)
      {
         m_traits.excl_lock();
         m_map.rehash(count);
         m_traits.excl_release();
      }

      private:
      map_type m_map;
      mutable SRWTraits m_traits;
   };
}