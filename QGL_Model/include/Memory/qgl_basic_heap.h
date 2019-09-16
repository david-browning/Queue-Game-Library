#pragma once
#include "include/qgl_model_include.h"
#include "include/Memory/qgl_heap_traits.h"
#include "include/Memory/qgl_mem_tracker.h"
#include "include/Memory/qgl_mem_tmpl.h"

namespace qgl::mem::heap
{
   template<heap_flag Flags,
      class Traits = heap_traits<Flags>,
      class MemoryTracker = mem_tracker>
   class basic_heap
   {
      public:
      basic_heap(Traits::size_type initialSizeBytes,
                 MemoryTracker mt = MemoryTracker()) :
         m_tracker(mt)
      {
         m_heapHandle = Traits::make(initialSizeBytes);
      }

      /*
       Cannot copy a heap.
       */
      basic_heap(const basic_heap&) = delete;

      /*
       Cannot move a heap.
       */
      basic_heap(basic_heap&&) = delete;

      /*
       Frees the heap.
       The destructor is not called on any objects stored in the heap.
       It is up to the caller to be sure the destructors are called.
       The destructors are called when using deallocate().
       */
      ~basic_heap() noexcept
      {
         Traits::destroy(m_heapHandle);
      }

      /*
       Allocates enough memory to hold count instances of T so that they are
       all properly aligned. This does not construct an object of type T.
       The caller must construct T after the address is returned.
       Returns nullptr if count is 0.
       Throws std::bad_alloc if the heap cannot allocate memory.
       */
      template<class T>
      T* allocate(Traits::size_type count)
      {
         if (count > 1)
         {
            //Align the amount of memory requested.
            const auto actualSize = count * sizeof(T);

            //Get an address.
            auto address = Traits::allocate(m_heapHandle, actualSize);

            //Check the address
            if (address != nullptr)
            {
               //Track the address.
               m_tracker.track((uintptr_t)address, actualSize);

               //Return the address.
               return static_cast<T*>(address);
            }
            else
            {
               throw std::bad_alloc();
            }
         }

         return nullptr;
      }

      /*
       Calls the destructor and deallocate the memory pointed to by ptr.
       If compiled in DEBUG, this will validate that the address was allocated
       using this heap. If the address was not allocated using this heap, this
       throws std::invalid_argument.
       */
      template<class T>
      void deallocate(T* const ptr)
      {
         #ifdef DEBUG
         if (!m_tracker.contains((uintptr_t)ptr))
         {
            throw std::invalid_argument("Not tracking the pointer.");
         }
         #endif

         ptr->~T();
         m_tracker.free((uintptr_t)ptr);

         Traits::deallocate(m_heapHandle, ptr);
      }

      private:
      heap_handle m_heapHandle;
      MemoryTracker m_tracker;
   };
}