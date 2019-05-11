#pragma once
#include <vector>
#include <exception>

namespace std
{
   template<typename T>
   class ring
   {
      public:
      ring(size_t capacity) :
         m_ring(capacity),
         m_current(0)
      {

      }

      ring(std::initializer_list<T>& fill) :
         m_ring(fill.begin(), fill.end()),
         m_current(0)
      {

      }

      template<class InputIterator>
      ring(InputIterator first, InputIterator last) :
         m_ring(first, last),
         m_current(0)
      {

      }

      ring(const ring& r) :
         m_ring(r.m_ring),
         m_current(r.m_current)
      {

      }

      ring(ring&& r) :
         m_ring(std::move(r.m_ring)),
         m_current(r.m_current)
      {
         r.m_current = static_cast<size_t>(-1);
      }

      virtual ~ring()
      {
         m_current = static_cast<size_t>(-1);
      }

      /*
       Advances the font pointer to the next item. (Clockwise)
       */
      size_t advance()
      {
         return m_current = (m_current + 1) % size();
      }

      void advance_to(size_t idx)
      {
         if (idx >= 0 && idx < size())
         {
            m_current = idx;
         }
         else
         {
            throw std::out_of_range("Tried to advance outside the ring.");
         }
      }

      /*
       Retreats the front pointer to the previous item. (Counter-Clockwise)
       */
      void retreat()
      {
         if (m_current != 0)
         {
            m_current--;
         }
         else
         {
            m_current = size() - 1;
         }
      }

      /*
       Returns a const reference to the item where the ring is pointing.
       */
      inline const T& front() const
      {
         return m_ring[m_current];
      }

      /*
       Returns a reference to the item where the ring is pointing.
       */
      inline T& front()
      {
         return m_ring[m_current];
      }

      /*
       Returns a const reference to the idx'th item.
       */
      inline const T& at(size_t idx) const
      {
         //vector::at does bounds checking. Do not do it again.
         return m_ring.at(idx);
      }

      /*
       Returns a reference to the idx'th item.
       */
      inline T& at(size_t idx)
      {
         //vector::at does bounds checking. Do not do it again.
         return m_ring.at(idx);
      }

      inline const T& operator[](size_t idx) const
      {
         return m_ring[idx];
      }

      inline T& operator[](size_t idx)
      {
         return m_ring[idx];
      }

      inline size_t size() const
      {
         return m_ring.size();
      }

      auto begin()
      {
         return m_ring.begin();
      }

      auto cbegin() const
      {
         return m_ring.cbegin();
      }

      auto end()
      {
         return m_ring.end();
      }

      auto cend() const
      {
         return m_ring.cend();
      }

      private:
      std::vector<T> m_ring;
      size_t m_current;
   };
}