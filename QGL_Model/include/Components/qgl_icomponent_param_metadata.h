#pragma once
#include "include/qgl_model_include.h"
#include "include/qgl_version.h"
#include "include/Interfaces/qgl_interface.h"
#include "include/Components/qgl_component_params.h"

namespace qgl::components
{
   /*
    Metadata about a component parameter. This is stable at ABI boundaries.
    Do not directly instantiate this class. Pointers to instances of this will
    be provided by an icomponent_metadata.
    */
   QGL_INTERFACE icomponent_param_metadata : public iqgl
   {
      public:
      /*
       Writes the parameters's name to "strz" and returns the number of
       characters written (including the null terminator). Pass null and this
       function will return the necessary buffer size to hold the name,
       including the null terminator.
       */
      virtual uint32_t name(char* strz) const noexcept = 0;

      /*
       Writes the parameters's description to "strz" and returns the number of
       characters written (including the null terminator). Pass null and this
       function will return the necessary buffer size to hold the description,
       including the null terminator.
       */
      virtual uint32_t description(char* strz) const noexcept = 0;

      /*
       Returns the data type of the parameter.
       */
      virtual content_param_types type() const noexcept = 0;

      /*
       Returns the number of elements in the data type. If this is more than 1,
       then the parameter is an array of "type()".
       */
      virtual uint8_t size() const noexcept = 0;

      /*
       Places the "idx'th" metadata into "out_p". Deallocate it by calling
       "release()".
       */
      virtual result_t parameter(
         uint32_t idx, icomponent_param_metadata** out_p) const noexcept = 0;
   };
}