#pragma once
#include "include/qgl_model_include.h"
#include "include/Errors/qgl_e_checkers.h"

namespace qgl::mem
{
   template<DWORD Flags>
   struct basic_heap_traits
   {
      using size_type = typename size_t;
      using heap_handle = typename handle_t;

      /*
       Creates a private heap object that can be used by the calling process.
       Returns nullptr if the heap could not be allocated.
       */
      static heap_handle make(size_type initialSizeBytes) noexcept
      {
         #ifdef WIN32
         return HeapCreate(Flags, initialSizeBytes, 0);
         #else
         //Not supported yet.
         throw_not_implemented();
         #endif
      }

      /*
       Allocates a block of memory from a heap. The allocated memory is not
       movable.
       */
      static void* allocate(heap_handle hHandle,
                            size_type bytes) noexcept
      {
         #ifdef WIN32
         return HeapAlloc(hHandle, 0, bytes);
         #else
         //Not supported yet.
         throw_not_implemented();
         #endif
      }

      /*
       Frees a memory block allocated from a heap.
       */
      static void deallocate(heap_handle hHandle,
                             void* const ptr) noexcept
      {
         #ifdef WIN32
         HeapFree(hHandle, 0, ptr);
         #else
         //Not supported yet.
         throw_not_implemented();
         #endif
      }

      /*
       Destroys the specified heap object. It decommits and releases all the
       pages of a private heap object, and it invalidates the handle to the
       heap.
       */
      static void destroy(heap_handle hHandle) noexcept
      {
         #ifdef WIN32
         HeapDestroy(hHandle);
         #else
         //Not supported yet.
         throw_not_implemented();
         #endif
      }
   };

   template<DWORD Flags>
   struct fixed_heap_traits
   {
      using size_type = typename size_t;
      using heap_handle = typename handle_t;

      /*
       Creates a private heap object that can be used by the calling process.
       Returns nullptr if the heap could not be allocated.
       */
      static heap_handle make(size_type initialSizeBytes) noexcept
      {
         #ifdef WIN32
         return HeapCreate(Flags, initialSizeBytes, initialSizeBytes);
         #else
         //Not supported yet.
         throw_not_implemented();
         #endif
      }

      /*
       Allocates a block of memory from a heap. The allocated memory is not
       movable.
       */
      static void* allocate(heap_handle hHandle,
                            size_type bytes) noexcept
      {
         #ifdef WIN32
         return HeapAlloc(hHandle, 0, bytes);
         #else
         //Not supported yet.
         throw_not_implemented();
         #endif
      }

      /*
       Frees a memory block allocated from a heap.
       */
      static void deallocate(heap_handle hHandle,
                             void* const ptr) noexcept
      {
         #ifdef WIN32
         HeapFree(hHandle, 0, ptr);
         #else
         //Not supported yet.
         throw_not_implemented();
         #endif
      }

      /*
       Destroys the specified heap object. It decommits and releases all the
       pages of a private heap object, and it invalidates the handle to the
       heap.
       */
      static void destroy(heap_handle hHandle) noexcept
      {
         #ifdef WIN32
         HeapDestroy(hHandle);
         #else
         //Not supported yet.
         throw_not_implemented();
         #endif
      }
   };
}