#pragma once
#include "include/qgl_model_include.h"
#include "include/qgl_version.h"
#include "include/Interfaces/qgl_icontent_type.h"

namespace qgl
{
   enum class content_control_types
   {
      text_box = 1,
      spinner,
      uspinner,
      file_reference,
      toggle,
      list,
      multi_list,
   };

   struct content_map_data
   {
      size_t controls;
      content_control_types control_type;
   };

   QGL_INTERFACE icontent_map : public iqgl
   {
      public:
      virtual bool contains(content_param_types) const noexcept = 0;

      virtual void insert(content_param_types, const content_map_data*) = 0;

      /*
       Returns the number of controls that are needed to represent the input
       for a icontent_type_param.
       */
      virtual size_t control_count(content_param_types) const = 0;

      /*
       Returns the type of control that is used to represent data for an
       icontent_type_param.
       */
      virtual content_control_types control_type(content_param_types) const = 0;
   };

   extern "C" QGL_MODEL_API result_t QGL_CC make_content_map(
      qgl_version_t, icontent_map**) noexcept;
}