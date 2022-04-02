#pragma once
#include "include/qgl_model_include.h"
#include "include/Interfaces/qgl_interface.h"

namespace qgl
{
   /*
    */
   using content_param_types = int32_t;
   enum class known_param_types : content_param_types
   {
      known_int8 = 1,
      known_uint8,
      known_int16,
      known_uint16,
      known_int32,
      known_uint32,
      known_int64,
      known_uint64,
      known_int_last,

      known_rational8 = 15,
      known_urational8,
      known_rational16,
      known_urational16,
      known_rational32,
      known_urational32,
      known_rational64,
      known_urational64,
      known_rational_last,

      known_rational8x2 = 23,
      known_urational8x2,
      known_rational16x2,
      known_urational16x2,
      known_rational32x2,
      known_urational32x2,
      known_rational64x2,
      known_urational64x2,
      known_rationalx2_last,

      known_rational8x3 = 31,
      known_rational16x3,
      known_rational32x3,
      known_rational64x3,
      known_urational8x3,
      known_urational16x3,
      known_urational32x3,
      known_urational64x3,
      known_rationalx3_last,

      known_rational8x4 = 39,
      known_urational8x4,
      known_rational16x4,
      known_urational16x4,
      known_rational32x4,
      known_urational32x4,
      known_rational64x4,
      known_urational64x4,
      known_rationalx4_last,

      known_str8 = 47,
      known_str16,
      known_str_last,
   };

   QGL_INTERFACE icontent_type_param : public iqgl
   {
      public:
      /*
       Returns the name of this content param. String is null terminated.
       The class owns the pointer. Do not free the pointer returned from this 
       function.
       */
      virtual const char* name_z() const noexcept = 0;

      /* 
       Returns the type of data that the "options" array points to.
       Inspect this to determine what type to cast "options()" to.
       */
      virtual content_param_types type() const noexcept = 0;

      /*
       Returns the number of items in the array returned by "options()".
       This can return zero to indicate that there is not a fixed number of
       options.
       */
      virtual size_t count() const noexcept = 0;

      /*
       Returns an array of options for the content type.
       The class owns the array. Do not free the pointer returned from this
       function.
       This can return null to indicate that there is not a fixed number of 
       options.
       */
      virtual void* options() const noexcept = 0;
   };

   QGL_INTERFACE icontent_type : public iqgl
   {
      public:
      /*
       Returns the name of this content type. String is null terminated.
       The class owns the pointer. Do not free the pointer returned from this 
       function.
       */
      virtual const char* name_z() const noexcept = 0;

      /*
       Returns the number of icontent_type_param(s).
       */
      virtual size_t count() const noexcept = 0;
      
      /*
       Returns an array of pointers to icontent_type_param(s). The class owns
       the array of pointers. Do not free the pointer returned from this 
       function or any of the pointers in the array.
       */
      virtual const icontent_type_param** iparams() const noexcept = 0;
   };

   static constexpr auto content_type_func_name = "qgl_get_content_types";
}