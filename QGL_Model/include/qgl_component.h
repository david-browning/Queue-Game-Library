#pragma once
#include "qgl_model_include.h"
#include "qgl_guid.h"

namespace qgl
{
   /*
    A component is a tool used to decouple behavior from data. Instead of a
    monolithic class that responds to input, rendering, and physics, those
    domains can be removed and encompassed in separate components.
    Data classes will likely want to make component a friend class to avoid
    publicly exposing setters for things like position, velocity, or other
    states that should not be public.
   UpdateT: Type of object to update. Usually, this is the object being
     updated like a sprite, model, or physics state.
    ContextT: Update Context Type. This is context used to update the
     object. The context could be the elapsed time since the last render, the
     current input state, or a graphics context to render the object.
    ReturnT: What the update function should return.
    UpdateFunctor: A functor that provides the component's logic. The functor
     must have this signature: (ReturnT)(UpdateT&, ContextT&)
    Components use a GUID so engine components can locate them at run time.
    Content references components using their GUID.
    */
   template<class UpdateT, class ContextT, class ReturnT, class UpdateFunctor>
   class component
   {
      public:
      constexpr component(qgl::guid&& g,
                          UpdateFunctor&& f = UpdateFunctor()) :
         m_guid(std::forward<qgl::guid>(g)),
         m_functor(std::forward<UpdateFunctor>(f))
      {

      }

      /*
       Default copy constructor.
       */
      component(const component&) noexcept = default;

      /*
       Default move constructor.
       */
      component(component&&) noexcept = default;

      /*
       Default destructor.
       */
      ~component() noexcept = default;

      /*
       Applies the update function to object using the given context.
       */
      ReturnT udpate(UpdateT& obj, ContextT& context)
      {
         return m_functor(obj, context);
      }

      /*
       Returns a reference to this component's GUID.
       */
      const qgl::guid& guid() const noexcept
      {
         return m_guid;
      }

      private:
      qgl::guid m_guid;
      UpdateFunctor m_functor;
   };
}