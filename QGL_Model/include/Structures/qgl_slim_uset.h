#pragma once
#include "include/qgl_model_include.h"
#include "include/Threads/qgl_srw_traits.h"

namespace qgl
{
   template<
      class Key,
      class SRWTraits = qgl::srw_traits,
      class Hash = std::hash<Key>,
      class KeyEqual = std::equal_to<Key>,
      class Allocator = std::allocator<Key>>
   class slim_uset
   {
      public:
      class iterator
      {

      };

      class const_iterator
      {

      };

      slim_uset(SRWTraits traits = SRWTraits());

      slim_uset(const slim_uset& r);

      slim_uset(slim_uset&& r);

      ~slim_uset();

      friend void swap(slim_uset& l, slim_uset& r);

      slim_uset& operator=(slim_uset r);

      iterator begin();

      iterator end();

      const_iterator begin() const;

      const_iterator end() const;

      const_iterator cbegin() const;

      const_iterator cend() const;

      [[nodiscard]] bool empty() const;

      [[nodiscard]] size_t size() const;

      [[nodiscard]] bool contains(const Key& k) const;

      void clear();

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
       Inserts element(s) into the container, if the container doesn't already contain an element with an equivalent key.
       Returns a pair consisting of an iterator to the inserted element (or to the element that prevented the insertion) and a bool denoting whether the insertion took place.
       */
      std::pair<iterator, bool> insert(const Key& value);

      std::pair<iterator, bool> insert(Key&& value);

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
       Acquires a shared lock and returns the average number of elements per
       bucket, that is, size() divided by bucket_count().
       */
      float load_factor() const;

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
      void max_load_factor(float ml);

      private:
   };
}