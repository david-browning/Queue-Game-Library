#include "pch.h"
#include "include/qgl_misc_helpers.h"
#include "include/Errors/qgl_e_checkers.h"
#include <gamingdeviceinformation.h>

namespace qgl
{
   hw_types QGL_CC hw_type() noexcept
   {
#ifdef WIN32
      GAMING_DEVICE_MODEL_INFORMATION modelInformation;
      auto result = GetGamingDeviceModelInformation(&modelInformation);
      if (FAILED(result))
      {
         return hw_types::hw_type_error;
      }

      return static_cast<hw_types>(modelInformation.deviceId);
#else
      return hw_types::hw_type_pc;
#endif
   }
}