#pragma once
#include "include/qgl_graphics_include.h"

namespace qgl::graphics::gpu
{
   /*
    Holds information about a GPU allocator.
    */
   struct allocator_stats final
   {
      constexpr allocator_stats()
      {

      }

      constexpr allocator_stats(const allocator_stats&) = default;

      constexpr allocator_stats(allocator_stats&&) noexcept = default;

      ~allocator_stats() noexcept = default;

      friend void swap(allocator_stats& l, allocator_stats& r) noexcept
      {
         using std::swap;
         swap(l.alignment, r.alignment);
         swap(l.total, r.total);
         swap(l.budgeted, r.budgeted);
         swap(l.committed, r.committed);
         swap(l.allocations, r.allocations);
         swap(l.commits, r.commits);
      }

      allocator_stats& operator=(allocator_stats r) noexcept
      {
         swap(*this, r);
         return *this;
      }

      /*
       GPU memory alignment.
       */
      size_t alignment = 0;

      /*
       The total amount of memory available on the GPU.
       */
      size_t total = 0;

      /*
       How many bytes are budgeted for the allocator. 0 indicates that the 
       allocator has an unlimited budget.
       */
      size_t budgeted = 0;

      /*
       How many bytes of memory have been committed to the GPU.
       */
      size_t committed = 0;

      /*
       How many current allocations there are.
       */
      size_t allocations = 0;

      /*
       How many allocations are currently committed to the GPU.
       */
      size_t commits = 0;
   };
}