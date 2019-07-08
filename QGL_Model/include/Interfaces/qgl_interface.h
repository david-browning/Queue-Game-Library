#pragma once
#include "include/qgl_model_include.h"

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
      virtual void release() = 0;

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

   };

   /*
    Wraps an iqgl pointer in a unique pointer so that when the unique pointer 
    goes out of scope, the pointer is properly freed. 
    p: Pointer to the iqgl interface to wrap. Do not release the p pointer after 
    wrapping it with a unique pointer. The smart pointer takes care of 
    deallocation.
    */
   template<class T>
   inline std::unique_ptr<T, std::function<void(T*)>>make_unique(T* p)
   {
      return std::unique_ptr<T, std::function<void(T*)>>(p,
                                                         [](T* ptr)
      {
         ptr->release();
      });
   }
}