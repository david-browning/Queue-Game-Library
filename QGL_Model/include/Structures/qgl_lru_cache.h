#pragma once
#include "include/qgl_model_include.h"
#include "include/qgl_not_cached_ex.h"
#include "include/Structures/qgl_slim_list.h"
#include "include/Structures/qgl_slim_umap.h"
#include "QGLTraits.h"

namespace qgl
{
   /*
    A least recently used cache. Items in the cache can be accessed via a key.
    The "front" item was the most recently used.

    Iterators are provided but they are not thread safe.
    */
   template<
      class Key,
      class T,
      class Size = qgl::get_size<T>,
      class Hash = std::hash<Key>,
      class KeyEqual = std::equal_to<Key>>
   class lru_cache final
   {
      public:

      using lru_list = typename std::list<Key>;
      using lru_map = typename std::unordered_map<
         Key,
         T,
         Hash,
         KeyEqual>;

      template<bool IsConst>
      class basic_iterator
      {
         public:
         using list_iterator = typename std::conditional<
            IsConst,
            typename lru_list::const_iterator,
            typename lru_list::iterator>::type;

         using map_type = typename std::conditional<
            IsConst,
            const lru_map,
            lru_map>::type;

         using mapped_reference = typename std::conditional<
            IsConst,
            const T&,
            T&>::type;

         using value_type = std::pair<const Key&, mapped_reference>;
         using reference = value_type;
         using pointer = void; // optional; not used in range-based for

         // Constructor
         basic_iterator(list_iterator listIt, map_type* map_p)
            : m_map_p(map_p), m_curPos(listIt)
         {
         }

         // Copy/move
         basic_iterator(const basic_iterator&) = default;
         basic_iterator(basic_iterator&&) = default;
         ~basic_iterator() = default;

         basic_iterator& operator=(basic_iterator r) noexcept
         {
            swap(*this, r);
            return *this;
         }

         friend void swap(basic_iterator& l, basic_iterator& r) noexcept
         {
            using std::swap;
            swap(l.m_map_p, r.m_map_p);
            swap(l.m_curPos, r.m_curPos);
         }

         // Dereference
         reference operator*() const
         {
            auto& key = *m_curPos;
            auto& value = m_map_p->at(key);
            return reference{ key, value };
         }

         // Iteration
         basic_iterator& operator++()
         {
            ++m_curPos;
            return *this;
         }

         basic_iterator& operator--()
         {
            --m_curPos;
            return *this;
         }

         bool operator==(const basic_iterator& r) const
         {
            return m_curPos == r.m_curPos;
         }

         bool operator!=(const basic_iterator& r) const
         {
            return !(*this == r);
         }

         private:
         map_type* m_map_p;
         list_iterator m_curPos;
      };

      using iterator = typename basic_iterator<false>;
      using const_iterator = typename basic_iterator<true>;

      lru_cache(size_t maxSize,
                Size szFunctor = Size()) :
         m_capacity(maxSize),
         m_size(0),
         m_sizeFunctor(szFunctor)
      {

      }

      /*
       Copy constructor.
       */
      lru_cache(const lru_cache& r) = default;

      /*
       Move constructor.
       */
      lru_cache(lru_cache&& r) = default;

      /*
       Destructor
       */
      ~lru_cache() noexcept = default;

      friend void swap(lru_cache& l, lru_cache& r) noexcept
      {
         using std::swap;
         swap(l.m_sizeFunctor, r.m_sizeFunctor);
         swap(l.m_lru, r.m_lru);
         swap(l.m_cache, r.m_cache);
         swap(l.m_capacity, r.m_capacity);
         swap(l.m_size, r.m_size);
      }

      /*
       Copy assign operator
       */
      lru_cache& operator=(lru_cache r) noexcept
      {
         swap(*this, r);
         return *this;
      }

      /*
       Returns the maximum number of bytes the cache can hold.
       */
      [[nodiscard]] size_t capacity() const noexcept
      {
         return m_capacity;
      }

      /*
       Returns the number of bytes currently in use.
       */
      [[nodiscard]] size_t size() const noexcept
      {
         return m_size;
      }

      /*
       Returns true if the cache is full.
       */
      [[nodiscard]] bool full() const noexcept
      {
         return size() >= capacity();
      }

      /*
       Returns true if there is an object with the given key in the cache.
       */
      bool cached(const Key& k) const noexcept
      {
         return m_cache.count(k) > 0;
      }

      /*
       Gets a reference to the cached object and moves it to the front of the 
       cache.
       Throws qgl::not_cached if the object is not cached.
       */
      [[nodiscard]] const T& get(const Key& k) const
      {
         if (m_cache.count(k) == 0)
         {
            throw qgl::not_cached<Key>{k};
         }

         m_lru.remove(k);
         m_lru.push_front(k);
         return m_cache.at(k);
      }

      /*
       Puts an item in the front of the cache and evicts the last item if there
       is no more room.
       */
      void put(const Key& k, const T& val)
      {
         auto objectSize = m_sizeFunctor(val);
         make_space(objectSize);
         m_lru.push_front(k);
         m_cache[k] = val;
         m_size += objectSize;
      }

      /*
       Evicts the least recently used item from the cache.
       */
      void evict_back() noexcept
      {
         auto key = m_lru.back();
         m_size -= m_sizeFunctor(m_cache[key]);
         m_cache.erase(key);
         m_lru.pop_back();
      }

      /*
       Returns a reference to the most recently cached item.
       */
      const T& front() const
      {
         auto key = m_lru.front();
         return m_cache.at(key);
      }

      /*
       Returns a reference to the least recently cached item.
       */
      const T& back() const
      {
         auto key = m_lru.back();
         return m_cache.at(key);
      }

      iterator begin()
      {
         return basic_iterator<false>(m_lru.begin(), &m_cache);
      }

      iterator end()
      {
         return basic_iterator<false>(m_lru.end(), &m_cache);
      }

      const_iterator begin() const
      {
         return basic_iterator<true>(m_lru.begin(), &m_cache);
      }

      const_iterator end() const
      {
         return basic_iterator<true>(m_lru.end(), &m_cache);
      }

      const_iterator cbegin() const
      {
         return basic_iterator<true>(m_lru.begin(), &m_cache);
      }

      const_iterator cend() const
      {
         return basic_iterator<true>(m_lru.end(), &m_cache);
      }

      private:
      /*
       Evicts items until there is "space" amount of free space in the cache.
       */
      void make_space(size_t space)
      {
         while (m_size + space > m_capacity)
         {
            // Evict  the back item until there is enough space.
            evict_back();
         }
      }

      /*
       Calculates the size of objects that are stored in the cache.
       */
      Size m_sizeFunctor;

      /*
       List of keys that were recently used. The closer to the front of the list,
       the more recently the key was referenced.
       Needs to be mutable so const operations can update the list order.
       */
      mutable lru_list m_lru;

      /*
       Maps a key to the cached item.
       */
      lru_map m_cache;

      /*
       Maximum amount of storage in the cache.
       */
      size_t m_capacity;

      /*
       Currently used space.
       */
      size_t m_size;
   };
}