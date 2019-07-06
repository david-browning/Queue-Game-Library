#pragma once
#include "include/qgl_model_include.h"
#include "include/Memory/qgl_static_ptr_ref.h"

namespace qgl
{
   template<class T, class Deleter = std::default_delete<T>>
   class static_ptr
   {
      public:
      static_ptr() :
         m_ptr(nullptr)
      {

      }

      static_ptr(std::unique_ptr<T, Deleter>&& ptr) :
         m_ptr(ptr)
      {

      }

      static_ptr(static_ptr&&) = default;

      ~static_ptr() noexcept = default;
      
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