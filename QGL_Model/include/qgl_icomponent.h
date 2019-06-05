#pragma once
#include "qgl_model_include.h"

namespace qgl
{
   /*
    A component is a tool used to decouple behavior from data. Instead of a
    monolithic class that responds to input, rendering, and physics, those
    domains can be removed and encompassed in separate components.
    Data classes will likely want to make icomponent a friend class to avoid
    publicly exposing setters for things like position, velocity, or other
    states that should not be public.
    Overload icomponent::update to implement this.
    UpdateContextT: Update Context Type. This is context used to update the
     object. The context could be the elapsed time since the last render, the
     current input state, or a graphics context to render the object.
    UpdateT: Type of object to update. Usually, this is the object being
     updated like a sprite or model.
    Components use a GUID so engine components can locate them at run time.
    Content references components using their GUID.
    */
   template<typename UpdateContextT, typename UpdateT>
   class icomponent
   {
      public:

      using uc_t = UpdateContextT;

      using u_t = UpdateT;

      /*
       Default constructor.
       */
      icomponent(const GUID* g) :
         m_guid(*g)
      {

      }

      /*
       Default copy constructor.
       */
      icomponent(const icomponent& r) = default;

      /*
       Default move constructor.
       */
      icomponent(icomponent&& r) = default;

      /*
       Default destructor.
       */
      virtual ~icomponent() noexcept = default;

      /*
       Updates obj using context.
       */
      virtual void update(const uc_t* context, u_t* obj) = 0;

      const GUID* guid() const noexcept
      {
         return &m_guid;
      }

      private:
      GUID m_guid;
   };
}