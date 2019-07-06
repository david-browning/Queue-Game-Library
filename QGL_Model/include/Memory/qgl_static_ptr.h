#pragma once
#include "include/qgl_model_include.h"
#include "include/Memory/qgl_static_ptr_ref.h"

namespace qgl
{
   /*
    A static pointer is a unique pointer than can only be assigned once. This 
    pointer should remain valid for the lifetime of the program. static_ptr 
    does not have a way to enforce this behavior.

    References to this pointer can be passed around by calling ref().
    These references should remain valid for the lifetime of this pointer.
    */
   template<class T, class Deleter = std::default_delete<T>>
   class static_ptr
   {
      public:
      /*
       Sets the internal pointer to nullptr. The pointer can be reseated one
       time by calling reset.
       */
      static_ptr() :
         m_ptr(nullptr)
      {

      }

      /*
       Sets the internal pointer. The pointer cannot be reassigned.
       */
      static_ptr(std::unique_ptr<T, Deleter>&& ptr) :
         m_ptr(ptr)
      {

      }

      /*
       Move constructor.
       */
      static_ptr(static_ptr&&) = default;

      /*
       Destructor
       */
      ~static_ptr() noexcept = default;

      /*
       Reassigns the internal pointer, only if the internal pointer is nullptr.
       This throws std::bad_alloc if the stored pointer is not nullptr.
       */
      void reset(std::unique_ptr<T, Deleter>&& ptr)
      {
         if (m_ptr)
         {
            throw std::bad_alloc("Cannot reassign a valid static pointer.");
         }

         m_ptr = ptr;
      }
      
      /*
       Returns a reference to this static pointer.
       Static pointer references cannot be null, so this will verify that the
       internal pointer is valid. This check is only done in debug builds.
       */
      static_ptr_ref<T> ref() const noexcept
      {
         assert(m_ptr.get() != nullptr);
         return static_ptr_ref<T>(m_ptr.get());
      }

      /*
       Gets the underlying pointer.
       */
      T* get() noexcept
      {
         return m_ptr;
      }

      /*
       Gets the underlying pointer.
       */
      const T* get() const noexcept
      {
         return m_ptr;
      }

      inline T& operator*() const noexcept
      {
         return *m_ptr;
      }

      inline T* operator->() const noexcept
      {
         return m_ptr;
      }

      /*
       Returns true if the pointer does not point to nullptr.
       */
      inline explicit operator bool() const noexcept
      {
         return m_ptr != nullptr;
      }

      private:
      std::unique_ptr<T, Deleter> m_ptr;
   };
}