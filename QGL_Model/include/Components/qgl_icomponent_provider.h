#pragma once
#include "include/qgl_model_include.h"
#include "include/Interfaces/qgl_module.h"
#include "include/qgl_version.h"
#include "include/Interfaces/qgl_interface.h"
#include "include/Components/qgl_icomponent_metadata.h"

namespace qgl::components
{
   /*
    Provided information about a module's available components.
    */
   QGL_INTERFACE icomponent_provider : public iqgl
   {
      public:
      /*
       Returns the number of components in the provider.
       */
      virtual uint32_t size() const noexcept = 0;

      /*
       Place's the "idx"th metadata into "out_p". Deallocate out_p by calling
       "release()".
       */
      virtual result_t metadata(uint32_t idx, 
                                icomponent_metadata** out_p) const noexcept = 0;
   };

   /*
    The entry point which will return the module's available components.
    Its signature is "get_component_provider_functor"
    */
   static constexpr auto QGL_COMPONENT_PROVIDER_ENTRY_POINT = 
      "qgl_get_components";

   /*
    Signature of a function that returns a module's available components.
    */
   typedef qgl::result_t(QGL_CC *get_component_provider_functor)(
      qgl_version, icomponent_provider**) noexcept;

   /*
    Gets the address of the component provider function and puts the result
    in "out_p".
    */
   extern "C" QGL_MODEL_API result_t QGL_CC component_prodiver_proc(
      const imodule* h_p, get_component_provider_functor* out_p) noexcept;
}