#pragma once
#include "include/qgl_model_include.h"
#include "include/qgl_not_cached_ex.h"
#include "include/Structures/qgl_slim_list.h"
#include "include/Structures/qgl_slim_umap.h"
#include "QGLTraits.h"

namespace qgl
{
   template<
      class Key,
      class T,
      class SRWTraits,
      class Size = qgl::get_size<T>,
      class Hash = std::hash<Key>,
      class KeyEqual = std::equal_to<Key>,
      class Allocator = std::allocator<std::pair<const Key, T>>>
   class lru_cache final
   {
      public:
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
      lru_cache(const lru_cache&) = default;

      /*
       Move constructor.
       */
      lru_cache(lru_cache&&) = default;

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
         return capacity() > size();
      }

      /*
       Returns true if there is an object with the given key in the cache.
       */
      bool cached(const Key& k) const noexcept
      {
         auto pos = m_cache.find(k);
         return pos != m_cache.end();
      }

      /*
       Gets a reference to the cached object.
       Throws qgl::not_cached if the object is not cached.
       */
      [[nodiscard]] const T& get(const Key& k) const
      {
         // If is cached.
         auto pos = m_cache.find(k);
         if (pos != m_cache.end())
         {
            // Move the key to the front of the LRU list
            m_lru.splice(m_lru.begin(), m_lru, pos->second.second);
            return pos->second.first;
         }

         throw not_cached{k};
      }

      void put(const Key& k, const T& val)
      {
         // How much space will inserting the object take?
         auto valSize = m_sizeFunctor(val);

         auto it = m_cache.find(k);
         if (it != m_cache.end())
         {
            // Item already cached. Update its value and move to front of the
            // cache.
            auto existingSize = m_sizeFunctor(it->second.first);

            // Check if need to evict things to make room.
            if (valSize > existingSize)
            {
               make_space();
            }

            // Update the value in the cache.
            it->second.first = val;
            m_size = m_size - existingSize + m_sizeFunctor(val);
            m_lru.splice(m_lru.begin(), m_lru, it->second.second);
         }
         else
         {
            // Insert object into the cache.
            make_space();

            // Put the item in the front of the cache.
            m_lru.push_front(k);
            m_cache[k] = { val, m_lru.begin() };
            m_size += valSize;
         }
      }

      /*
       Evicts the least recently used item from the cache.
       */
      void evict_back() noexcept
      {
         if (m_lru.size() > 0)
         {
            // Key to remove
            auto backKey = m_lru.back();

            // Cached item to remove
            auto evictPos = m_cache.find(backKey);

            // Decrease the size of the cache.
            m_size -= m_sizeFunctor(evictPos->second.first);

            // Remove key from the cache and LRU.
            m_cache.erase(evictPos);
            m_lru.pop_back();
         }
      }

      /*
       Returns a reference to the most recently cached item.
       */
      const T& front() const
      {
         return m_cache[m_lru.front()];
      }

      /*
       Returns a reference to the least recently cached item.
       */
      const T& back() const
      {
         return m_cache[m_lru.back()];
      }

      private:
      using lru_list = typename slim_list<Key, SRWTraits>;
      using lru_map = typename slim_umap<
         Key,
         std::pair<T, typename lru_list::iterator>,
         SRWTraits,
         Hash,
         KeyEqual,
         Allocator>;

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
       */
      lru_list m_lru;

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