#pragma once
#include "include/qgl_model_include.h"

namespace qgl
{
   struct iqgl
   {
      virtual void release() = 0;

      template<class T>
      T* as()
      {
         return dynamic_cast<T*>(this);
      }
   };

   typedef std::shared_ptr<iqgl> iqgl_sptr;

   inline iqgl_sptr make_shared(iqgl* p)
   {
      return iqgl_sptr(p, std::mem_fn(&iqgl::release));
   }
}