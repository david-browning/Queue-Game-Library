#include "pch.h"
#include "include/qgl_misc_helpers.h"

namespace qgl
{
   hw_types hw_type()
   {
#ifdef WIN32
      GAMING_DEVICE_MODEL_INFORMATION modelInformation;
      auto result = GetGamingDeviceModelInformation(&modelInformation);
      winrt::check_hresult(result);

      return static_cast<hw_types>(modelInformation.deviceId);
#else
      return hw_types::hw_type_pc;
#endif
   }
}