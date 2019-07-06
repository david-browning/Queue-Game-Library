#pragma once
#include "include/qgl_model_include.h"

namespace qgl
{
   template<class T>
   class QGL_MODEL_API static_ptr_ref final
   {
      public:
      static_ptr_ref(T* ptr) :
         m_ptr(ptr)
      {

      }

      /*
       Copy constructor
       */
      static_ptr_ref(const static_ptr_ref&) = default;

      /*
       Move constructor
       */
      static_ptr_ref(static_ptr_ref&&) = default;

      /*
       Destructor
       */
      ~static_ptr_ref() noexcept = default;

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

      /*
       Swaps the contents of l and r.
       */
      inline friend void swap(static_ptr_ref& l,
                              static_ptr_ref& r) noexcept
      {
         using std::swap;
         swap(l.m_ptr, r.m_ptr);
      }

      /*
       Copy swap assign operator.
       */
      inline static_ptr_ref& operator=(static_ptr_ref r) noexcept
      {
         swap(*this, r);
         return *this;
      }

      /*
       Move assign operator.
       */
      inline static_ptr_ref& operator=(static_ptr_ref&& r) noexcept
      {
         m_ptr = r.m_ptr;
         return *this;
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

      /*
       Returns true if l and r point to the same memory.
       */
      inline friend bool operator==(const static_ptr_ref& l,
                                    const static_ptr_ref& r) noexcept
      {
         return l.get() == r.get();
      }

      /*
       Returns true if l and r do not point to the same memory.
       */
      inline friend bool operator!=(const static_ptr_ref& l,
                                    const static_ptr_ref& r) noexcept
      {
         return !(l == r);
      }

      private:
      T* m_ptr;
   };
}