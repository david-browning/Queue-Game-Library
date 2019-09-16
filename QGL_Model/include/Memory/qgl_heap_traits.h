#pragma once
#include "include/qgl_model_include.h"

namespace qgl::mem::heap
{
   using heap_flag = DWORD;
   using heap_handle = HANDLE;

   template<heap_flag Flags>
   struct heap_traits
   {
      using size_type = size_t;

      /*
       Creates a private heap object that can be used by the calling process.
       */
      static heap_handle make(size_type initialSizeBytes) noexcept
      {
         #ifdef WIN32
         return HeapCreate(Flags, initialSizeBytes, 0);
         #else
         throw 1;
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
         throw 1;
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
         throw 1;
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
         throw 1;
         #endif
      }
   };

   template<heap_flag Flags>
   struct fixed_heap_traits
   {
      using size_type = size_t;

      /*
       Creates a private heap object that can be used by the calling process.
       */
      static heap_handle make(size_type initialSizeBytes) noexcept
      {
         #ifdef WIN32
         return HeapCreate(Flags, initialSizeBytes, initialSizeBytes);
         #else
         throw 1;
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
         throw 1;
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
         throw 1;
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
         throw 1;
         #endif
      }
   };
}