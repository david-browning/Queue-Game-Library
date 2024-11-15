#pragma once
#include "include/qgl_model_include.h"
#include "include/qgl_version.h"
#include "include/qgl_guid.h"
#include "include/Interfaces/qgl_interface.h"
#include "include/Components/qgl_icomponent_param_metadata.h"

namespace qgl::components
{
   /*
    Describes a component. This is stable at ABI boundaries. Do not directly
    instantiate this. Use the "make_component_from_json" factory function.
    */
   QGL_INTERFACE icomponent_metadata : public iqgl
   {
      public:
      /*
       Writes the component's name to "strz" and returns the number of
       characters written (including the null terminator). Pass null and this
       function will return the necessary buffer size to hold the name,
       including the null terminator.
       */
      virtual uint32_t name(char* strz) const noexcept = 0;

      /*
       Writes the component's description to "strz" and returns the number of
       characters written (including the null terminator). Pass null and this
       function will return the necessary buffer size to hold the description,
       including the null terminator.
       */
      virtual uint32_t description(char* strz) const noexcept = 0;

      /*
       Write's the component's GUID to the pointer "g". 
       */
      virtual void id(qgl::guid* g) const noexcept = 0;

      /*
       Returns the number of descriptors in this metadata.
       */
      virtual uint32_t size() const noexcept = 0;

      /*
       Places the "idx'th" metadata into "out_p". Deallocate it by calling
       "release()".
       */
      virtual result_t param(
         uint32_t idx, icomponent_param_metadata** out_p) const noexcept = 0;
   };

   /*
    Creates a component from a JSON string. Only 1 component can be specified
    in the string.

   {
       "id": "5B8B3826ECB84825A3508D149359A72E",
       "name": "rigid_body_param",
       "description": "Rigid Body Details",
       "parameters": [
           {
               "name": "mass",
               "description": "Mass in Kg",
               "type": "urational32",
               "size": 1
           },
           {
               "name": "collision_sphere",
               "description": "Bounding Sphere",
               "type": "compound",
               "parameters": [
                   {
                       "name": "radius",
                       "description": "Sphere Radius in M",
                       "type": "urational32",
                       "size": 1
                   },
                   {
                       "name": "pos",
                       "description": "Sphere center in M",
                       "type": "urational32",
                       "size": 3
                   }
               ]
           },
           {
               "name": "cor",
               "description": "Coefficient of Restitution",
               "type": "urational8",
               "size": 1
           }
       ]
   }

    Returns the following:
    S_OK: success,
    E_INVALIDARG: out_p is null.
    E_FAIL: A JSON element has the wrong type.
    HRESULT with facility "FACILITY_QGL" on JSON parse error.
    E_OUTOFMEMORY: If the metadata cannot be allocated.
    E_UNEXPECTED: On other errors.
    */
   extern "C" QGL_MODEL_API result_t QGL_CC make_component_from_json(
      const char* str, icomponent_metadata** out_p) noexcept;
}