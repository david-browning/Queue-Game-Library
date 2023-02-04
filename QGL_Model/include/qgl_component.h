#pragma once
#include "qgl_model_include.h"
#include "qgl_guid.h"
#include "Timing/qgl_timer.h"

namespace qgl::components
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
      constexpr component(const qgl::guid& g,
                          const UpdateFunctor& f = UpdateFunctor()) :
         m_guid(g),
         m_functor(f)
      {

      }

      /*
       Default copy constructor.
       */
      component(const component&) = default;

      /*
       Default move constructor.
       */
      component(component&&) noexcept = default;

      /*
       Default destructor.
       */
      virtual ~component() noexcept = default;

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

   /*
    Context using by content objects.
    */
   class game_context
   {
      public:
      using TickT = typename uint64_t;
      const qgl::time_state<TickT>& timer_state() const noexcept
      {
         return m_timerState;
      }

      private:
      qgl::time_state<TickT> m_timerState;
   };

   /*
    The update functor using by content objects.
    */
   template<class GameObject>
   using game_update_functor =
      typename std::function<qgl::result_t(GameObject&, game_context&)>;

   /*
    Specialization of the component class used by content objects.
    */
   template<class GameObject>
   using game_component = typename qgl::components::component<
      GameObject,
      game_context,
      qgl::result_t,
      game_update_functor<GameObject>>;

   template<class GameObject>
   qgl::result_t success_component_functor(GameObject&, game_context&)
   {
#ifdef WIN32
      return S_OK;
#else
      return 0;
#endif
   }
}