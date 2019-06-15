#pragma once
#include "include/qgl_model_include.h"

namespace qgl
{
   class iqgl
   {
      public:
      virtual void release() = 0;

      template<class T>
      T* as()
      {
         return dynamic_cast<T*>(this);
      }
   };

   class iqgl_impl : public iqgl
   {

   };

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