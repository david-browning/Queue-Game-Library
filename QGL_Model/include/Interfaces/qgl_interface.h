#pragma once
#include "include/qgl_model_include.h"
#include <memory>
#include <functional>

#define QGL_INTERFACE class __declspec(novtable)

namespace qgl
{
   QGL_INTERFACE iqgl
   {
      public:
      /*
       Releases all resourced owned by the interface. Usually, it is sufficient
       to call "delete this".
       */
      virtual void release() noexcept = 0;

      virtual iqgl* duplicate() const noexcept = 0;

      /*
       Casts this along the derivation chain to a pointer to a T.
       Returns nullptr if the cast is not valid.
       */
      template<class T>
      T* as()
      {
         return dynamic_cast<T*>(this);
      }
   };

   QGL_INTERFACE iqgl_impl : public iqgl
   {
      public:
      virtual void release() noexcept
      {
         delete this;
      }

      virtual iqgl* duplicate() const noexcept
      {
         return new iqgl_impl();
      }
   };

   template<class T>
   using qgl_unique_ptr = typename std::unique_ptr<T, std::function<void(T*)>>;

   /*
    Wraps an iqgl pointer in a unique pointer so that when the unique pointer
    goes out of scope, the pointer is properly freed.
    p: Pointer to the iqgl interface to wrap. Do not release the p pointer after
    wrapping it with a unique pointer. The smart pointer takes care of
    deallocation.
    */
   template<class T>
   inline qgl_unique_ptr<T> make_unique(T* p)
   {
      return std::unique_ptr<T, std::function<void(T*)>>(p, [](T* ptr)
      {
         ptr->release();
      });
   }

   template<class T>
   inline qgl_unique_ptr<T> duplicate(const qgl_unique_ptr<T>& p)
   {
      static_assert(std::is_base_of<iqgl, T>::value,
                    "p must point to an iqgl interface.");
      return qgl::make_unique<T>((T*)p->duplicate());
   }
}