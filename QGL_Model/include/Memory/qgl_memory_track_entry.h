#pragma once

namespace qgl::mem::internal
{
   /*
    This is an entry in a memory tracker.
    Overloads operators so this can be compared with an address.
    */
   struct memory_entry final
   {
      public:
      memory_entry(uintptr_t p, size_t sz) :
         m_p(p),
         m_size(sz)
      {

      }

      memory_entry(const memory_entry&) = default;

      memory_entry(memory_entry&&) = default;

      ~memory_entry() noexcept = default;

      inline uintptr_t ptr() const noexcept
      {
         return m_p;
      }

      inline size_t size() const noexcept
      {
         return m_size;
      }

      private:
      uintptr_t m_p;
      size_t m_size;
   };

   inline bool operator<(const memory_entry& l,
                         const uintptr_t& r) noexcept
   {
      return l.ptr() < r;
   }

   inline bool operator<(const uintptr_t& l,
                         const memory_entry& r) noexcept
   {
      return l < r.ptr();
   }

   inline bool operator<(const memory_entry& l,
                         const memory_entry& r) noexcept
   {
      return l.ptr() < r.ptr();
   }

   inline bool operator==(const memory_entry& l,
                          const memory_entry& r) noexcept
   {
      return l.ptr() == r.ptr();
   }
}