#include "pch.h"
#include "include/Debug/qgl_debug.h"

namespace qgl
{
   void debug_message(const sys_char* const msg) noexcept
   {
      OutputDebugString(msg);
   }
}