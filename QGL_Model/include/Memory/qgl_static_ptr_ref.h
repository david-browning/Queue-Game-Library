#pragma once
#include "include/qgl_model_include.h"

namespace qgl::mem
{
   /*
    Weak reference to a static pointer. The underlying address cannot be 
    changed.
    Since this is a weak pointer, the pointer does not need to be freed. 
    */
   template<class T>
   class static_ptr_ref
   {
      public:
      /*
       Constructs a static pointer reference from the raw pointer.
       This address cannot be changed.
       */
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
      inline T* get() noexcept
      {
         return m_ptr;
      }

      /*
       Gets the underlying pointer.
       */
      inline const T* get() const noexcept
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

      inline T& operator*() noexcept
      {
         return *m_ptr;
      }

      inline const T& operator*() const noexcept
      {
         return *m_ptr;
      }

      inline T* operator->() noexcept
      {
         return m_ptr;
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