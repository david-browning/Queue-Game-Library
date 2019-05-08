#pragma once
#include "qgl_model_include.h"

namespace qgl
{
   /*
    
    */
   template<typename UpdateContextT, typename UpdateT>
   class icomponent
   {
      public:
      
      /*
       Update Context Type. This is context used to update the object.
       The context could be the elapsed time since the last frame, or a graphics
       context to render the object.
       */
      using uc_t = UpdateContextT;
      
      /*
       Update Type. Usually, this is the object being updated.
       */
      using u_t = UpdateT;

      /*
       Default constructor
       */
      icomponent() = default;

      /*
       Default copy constructor
       */
      icomponent(const icomponent& r) = default;

      icomponent(icomponent&& r) = default;

      virtual ~icomponent() noexcept = default;

      virtual void update(const uc_t& context, u_t& obj) = 0;
   };
}