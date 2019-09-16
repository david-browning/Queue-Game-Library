#pragma once
#include "include/qgl_model_include.h"
#include "include/Memory/qgl_memory_track_entry.h"

namespace qgl::mem
{
   /*
    Tracks memory allocations and deallocations only if compiled with DEBUG
    defined. This class does nothing if DEBUG is not defined.
    Add an address using track().
    Remove an address using free().
    Remove all addresses using free_all().
    Despite changing the state, track() and free() are const so they can be
    called by allocators who's allocation and deallocation methods are const.
    */
   class mem_tracker
   {
      private:
      using alloc_list = 
         typename std::set<internal::memory_entry<uintptr_t>, std::less<>>;

      public:
      using iterator = typename alloc_list::iterator;
      using const_iterator = typename alloc_list::const_iterator;

      /*
       Constructor.
       */
      mem_tracker()
      {

      }

      /*
       Copy constructor.
       */
      mem_tracker(const mem_tracker&) = default;

      /*
       Move constructor.
       */
      mem_tracker(mem_tracker&&) = default;

      /*
       Destructor.
       */
      ~mem_tracker() noexcept = default;

      /*
       Adds an address to the tracker.
       This only does something if DEBUG is defined.
       */
      inline void track(uintptr_t p, size_t sz = 0) const
      {
         #ifdef DEBUG
         m_allocations.insert(internal::memory_entry(p, sz));
         #endif
      }

      /*
       Removes an address from the tracker. This does not deallocate the memory.
       This only does something if DEBUG is defined.
       */
      inline void free(uintptr_t p) const
      {
         #ifdef DEBUG
         //See the link for example on finding with differing keys.
         //https://en.cppreference.com/w/cpp/container/set/find
         auto it = m_allocations.find(p);
         if (it != end())
         {
            m_allocations.erase(it);
         }
         #endif
      }

      /*
       Removes all address from the tracker. This does not deallocate the
       memory.
       This only does something if DEBUG is defined.
       */
      inline void free_all()
      {
         #ifdef DEBUG
         m_allocations.clear();
         #endif
      }

      /*
       Returns true if this memory tracker is tracking the given pointer.
       */
      bool contains(uintptr_t p) const
      {
         auto it = m_allocations.find(p);
         return it != end();
      }

      #pragma region Iterators
      iterator begin() noexcept
      {
         return m_allocations.begin();
      }

      iterator end() noexcept
      {
         return m_allocations.end();
      }

      const_iterator begin() const noexcept
      {
         return m_allocations.cbegin();
      }

      const_iterator end() const noexcept
      {
         return m_allocations.cend();
      }

      const_iterator cbegin() const noexcept
      {
         return m_allocations.cbegin();
      }

      const_iterator cend() const noexcept
      {
         return m_allocations.cend();
      }
      #pragma endregion

      /*
       If not compiled with DEBUG, this does nothing because memory allocations
       are only tracked in DEBUG mode.
       */
      inline friend void swap(mem_tracker& l, mem_tracker& r) noexcept
      {
         #ifdef DEBUG
         using std::swap;
         swap(l.m_allocations, r.m_allocations);
         #endif
      }

      /*
       If not compiled with DEBUG, this returns this because memory allocations
       are only tracked in DEBUG mode.
       */
      inline mem_tracker& operator=(mem_tracker& r) noexcept
      {
         #ifdef DEBUG
         swap(*this, r);
         #endif
         return *this;
      }

      /*
       Returns true if l and r are tracking the same addresses.
       */
      inline friend bool operator==(const mem_tracker& l,
                                    const mem_tracker& r) noexcept
      {
         return l.m_allocations == r.m_allocations;
      }

      /*
       Returns false if l and r are tracking the same addresses.
       */
      inline friend bool operator!=(const mem_tracker& l,
                                    const mem_tracker& r) noexcept
      {
         return !(l == r);
      }

      private:
      /*
       Set of addresses and how large they are.
       */
      mutable alloc_list m_allocations;
   };
}