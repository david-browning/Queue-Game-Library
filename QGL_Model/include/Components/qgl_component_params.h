#pragma once
#include "include/qgl_model_include.h"
#include "include/Impl/qgl_component_params_impl.h"

namespace qgl::components
{
   using content_param_types = typename impl::content_param_types_impl;
   using known_param_types = typename impl::known_param_types_impl;
   
   /*
    Returns true if the given content parameter type is a compound type.
    */
   inline bool compound_param(int32_t t)
   {
      return t == static_cast<int32_t>(
         qgl::impl::known_param_types_impl::known_compound);
   }

   /*
    Gets the content parameter type from a string. Throws std::domain_error
    if the string does not map to a known content parameter type.
    */
   inline content_param_types str_to_param_type(const std::string& s)
   {
      if (impl::STR_COMPONENT_PARAM_MAP.count(s) == 0)
      {
         throw std::domain_error{ "String does not map to a type." };
      }
      
      return static_cast<content_param_types>(
         qgl::impl::STR_COMPONENT_PARAM_MAP.at(s));
   }

   /*
    Returns the string representation of a content parameter type.
    Throws std::domain_error if the string does not map to a known content 
    parameter type.
    */
   inline std::string param_type_to_str(content_param_types t)
   {
      if (impl::COMPONENT_PARAM_STR_MAP.count(t) == 0)
      {
         throw std::domain_error{ "Type does not map to a string." };
      }

      return qgl::impl::COMPONENT_PARAM_STR_MAP.at(t);
   }
}