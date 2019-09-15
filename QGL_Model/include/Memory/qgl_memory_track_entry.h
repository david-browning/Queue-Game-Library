#pragma once

namespace qgl::mem::alloc::internal
{
   /*
    This is an entry in a memory tracker.
    Overloads operators so this can be compared with an address.
    */
   template<typename Pointer>
   struct memory_entry
   {
      public:
      using pointer = typename Pointer;
      memory_entry(pointer p, size_t sz) :
         m_p(p),
         m_size(sz)
      {

      }

      memory_entry(const memory_entry&) = default;

      memory_entry(memory_entry&&) = default;

      ~memory_entry() noexcept = default;

      inline pointer ptr() const noexcept
      {
         return m_p;
      }

      inline size_t size() const noexcept
      {
         return m_size;
      }

      private:
      pointer m_p;
      size_t m_size;
   };

   template<typename PointerT>
   inline bool operator<(const memory_entry<PointerT>& l,
                         const PointerT& r) noexcept
   {
      return l.ptr() < r;
   }

   template<typename PointerT>
   inline bool operator<(const PointerT& l,
                         const memory_entry<PointerT>& r) noexcept
   {
      return l < r.ptr();
   }

   template<typename PointerT>
   inline bool operator<(const memory_entry<PointerT>& l,
                         const memory_entry<PointerT>& r) noexcept
   {
      return l.ptr() < r.ptr();
   }
}