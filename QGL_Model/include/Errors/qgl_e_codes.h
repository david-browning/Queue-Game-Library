#pragma once
#include "include/qgl_model_include.h"

namespace qgl
{
   static constexpr uint16_t FACILITY_QGL = 0x555;
   static constexpr uint16_t BAD_MAGIC_NUMBER = 0xBAD0;
   static constexpr HRESULT E_BADMAGIC = MAKE_HRESULT(
      1,
      FACILITY_QGL,
      BAD_MAGIC_NUMBER);

   static constexpr HRESULT S_ALREADYMAPPED = MAKE_HRESULT(
      0,
      FACILITY_QGL,
      ERROR_ALREADY_EXISTS);

   static constexpr HRESULT E_NOLOADER = MAKE_HRESULT(
      1,
      FACILITY_QGL,
      ERROR_NOT_FOUND);

   #ifdef WIN32

   using not_implemented = typename winrt::hresult_not_implemented;

   #else

   struct not_implemented
   {

   };

   #endif
}