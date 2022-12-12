#pragma once
#include "include/qgl_model_include.h"

namespace qgl
{
   /*
    Iterates through the keys of a map.
    */
   template<class KeyT, class MapT>
   class map_key_const_iterator final
   {
      public:
      /*
       Instantiates the begin iterator.
       */
      map_key_const_iterator(const MapT& m) :
         m_it(m.cbegin())
      {

      }

      /*
       Instantiates then end iterator.
       */
      map_key_const_iterator(const MapT& m, std::nullptr_t) :
         m_it(m.cend())
      {

      }

      map_key_const_iterator(const map_key_const_iterator&) = default;

      map_key_const_iterator(map_key_const_iterator&&) noexcept = default;

      ~map_key_const_iterator() noexcept = default;

      friend void swap(map_key_const_iterator l,
                       map_key_const_iterator r) noexcept
      {
         using std::swap;
         swap(l.m_it, r.m_it);
      }

      const KeyT& operator*() const
      {
         return it().operator*().first;
      }

      const KeyT* operator->() const
      {
         return std::addressof(it().operator->()->first);
      }

      map_key_const_iterator& operator=(map_key_const_iterator r) noexcept
      {
         swap(*this, r);
         return *this;
      }

      map_key_const_iterator& operator++()
      {
         ++it();
         return *this;
      }

      map_key_const_iterator& operator++(int)
      {
         it()++;
         return *this;
      }

      bool operator==(const map_key_const_iterator& r) const noexcept
      {
         return m_it == r.m_it;
      }

      bool operator!=(const map_key_const_iterator& r) const noexcept
      {
         return !(*this == r);
      }

      private:
      auto it() const
      {
         return m_it;
      }

      using iterator = typename MapT::const_iterator;
      iterator m_it;
   };
}