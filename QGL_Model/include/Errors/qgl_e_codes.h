#pragma once
#include "include/qgl_model_include.h"

using qgl_code = HRESULT;

static constexpr uint16_t FACILITY_QGL = 0x555;
static constexpr uint16_t BAD_MAGIC_NUMBER = 0xBAD0;

#ifdef WIN32
inline constexpr qgl_code make_code(bool failed, 
                                    uint16_t facility, 
                                    uint16_t code)
{
   return MAKE_HRESULT(failed, facility, code);
}
#else
inline qgl_code make_code(bool failed, uint16_t facility, uint16_t code)
{
   return ((((unsigned long)(failed) << 31) |
            ((unsigned long)(facility) << 16) |
            ((unsigned long)(code))))
}
#endif

static constexpr qgl_code E_BADMAGIC = make_code(
   1,
   FACILITY_QGL,
   BAD_MAGIC_NUMBER);

static constexpr qgl_code S_ALREADYMAPPED = make_code(
   0,
   FACILITY_QGL,
   ERROR_ALREADY_EXISTS);

static constexpr qgl_code E_NOLOADER = make_code(
   1,
   FACILITY_QGL,
   ERROR_NOT_FOUND);