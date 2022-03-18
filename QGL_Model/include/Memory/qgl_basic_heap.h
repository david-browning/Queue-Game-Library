#pragma once
#include "include/qgl_model_include.h"
#include "include/Memory/qgl_heap_traits.h"

namespace qgl::mem
{
   template<DWORD Flags,
      class Traits = qgl::mem::basic_heap_traits<Flags>>
   class basic_heap
   {
      public:
      using heap_handle = typename Traits::heap_handle;
      using size_type = typename Traits::size_type;

      basic_heap(size_type initialSizeBytes) :
         m_heapHandle(INVALID_HANDLE)
      {
         m_heapHandle = Traits::make(initialSizeBytes);
         if (m_heapHandle == nullptr)
         {
            throw_last_error();
         }
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
         if (m_heapHandle != INVALID_HANDLE<handle_t>)
         {
            Traits::destroy(m_heapHandle);
         }
      }

      /*
       Allocates enough memory to hold count instances of T so that they are
       all properly aligned. This does not construct an object of type T.
       The caller must construct T after the address is returned.
       Returns nullptr if count is 0.
       Throws std::bad_alloc if the heap cannot allocate memory.
       */
      template<class T>
      T* allocate(size_type count)
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
         ptr->~T();
         Traits::deallocate(m_heapHandle, ptr);
      }

      private:
      heap_handle m_heapHandle;
   };
}