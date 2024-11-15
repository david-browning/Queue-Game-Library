#pragma once
#include "include/qgl_model_include.h"
#include "include/Threads/qgl_srw_traits.h"
#include <memory>
#include <utility>

namespace qgl
{
   template<
      class Key,
      class T,
      class SRWTraits = qgl::srw_traits,
      class Hash = std::hash<Key>,
      class KeyEqual = std::equal_to<Key>,
      class Allocator = std::allocator<std::pair<const Key, T>>>
   class slim_umap final
   {
      public:
      using insert_type = typename std::pair<const Key, T>;
      using value_type = typename std::pair<T&, SRWTraits>;
      using const_value_type = typename std::pair<const T&, SRWTraits>;

      class bucket_t;

      class iterator;
      class const_iterator;

      slim_umap();

      slim_umap(const slim_umap& r);

      slim_umap(slim_umap&& r);

      ~slim_umap();

      friend void swap(slim_umap& l, slim_umap& r);

      slim_umap& operator=(slim_umap r);

      iterator begin();

      iterator end();

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

      [[nodiscard]] bool empty() const;

      [[nodiscard]] size_t size() const;

      /*
       Acquires an exclusive lock and clears all elements from the map.
       std::unordered_map's clear function is linear complexity.
       */
      void clear();

      /*
       Acquires a shared lock and returns a reference to the mapped value of
       the element with key equivalent to key. If no such element exists,
       an exception of type std::out_of_range is thrown.
       */
      value_type at(const Key& k);

      /*
       Acquires a shared lock and returns a reference to the mapped value of
       the element with key equivalent to key. If no such element exists,
       an exception of type std::out_of_range is thrown.
       */
      const_value_type at(const Key& k) const;

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
       Returns the number of elements with key that compares equal to the specified argument key, which is either 1 or 0 since this container does not allow duplicates.
       */
      size_t count(const Key& k) const;

      /*
       Finds an element with key equivalent to key.
       */
      iterator find(const Key& k);

      /*
       Finds an element with key equivalent to key.
       */
      const_iterator find(const Key& k) const;

      /*
       Inserts a new element into the container constructed in-place with the given args if there is no element with the key in the container.
       Returns a pair consisting of an iterator to the inserted element, or the already-existing element if no insertion happened, and a bool denoting whether the insertion took place
       */
      template<class... Args>
      std::pair<iterator, bool> emplace(Args&&... args);

      /*
       Returns an iterator following the last removed element.
       */
      iterator erase(iterator pos);

      /*
       Returns an iterator following the last removed element.
       */
      iterator erase(const_iterator pos);

      /*
       Returns an iterator following the last removed element.
       */
      iterator erase(const_iterator first, const_iterator last);

      /*
       Returns the number of elements removed (0 or 1)
       */
      size_t erase(const Key& key);

      /*
       Inserts element(s) into the container, if the container doesn't already contain an element with an equivalent key.
       Returns a pair consisting of an iterator to the inserted element (or to the element that prevented the insertion) and a bool denoting whether the insertion took place.
       */
      std::pair<iterator, bool> insert(const insert_type& value);

      std::pair<iterator, bool> insert(insert_type&& value);

      /*
       Acquires a shared lock and returns the average number of elements per
       bucket, that is, size() divided by bucket_count().
       */
      float load_factor() const
      {
         m_traits.share_lock();
         auto ret = static_cast<float>(m_size.load()) /
            static_cast<float>(m_buckets.size());
         m_traits.share_release();
         return ret;
      }

      /*
       Acquires a shared lock and returns current maximum load factor.
       */
      float max_load_factor() const
      {
         return m_maxLoadFactor.load();
      }

      /*
       Acquires am exclusive lock and sets the maximum load factor.
       If the passed load factor is less than the current load factor, this will
       trigger a rehash.
       */
      void max_load_factor(float ml)
      {
         m_traits.excl_lock();
         if (ml < m_maxLoadFactor.load())
         {
            auto newSize = static_cast<size_t>(ml * m_buckets.size());
            rehash(newSize);
         }

         m_maxLoadFactor.store(ml);
         m_traits.excl_release();
      }

      /*
       Acquires an exclusive lock and sets the number of buckets to count and
       rehashes the container.

       If the new number of buckets makes load factor more than maximum load
       factor (count < size() / max_load_factor()), then the new number of
       buckets is at least size() / max_load_factor().
       */
      void rehash(size_t count);

      private:
      std::atomic<size_t> m_size;
      std::atomic<float> m_maxLoadFactor;
      std::vector<bucket_t> m_buckets;
      mutable SRWTraits m_traits;
   };
}