#include "pch.h"
#include "include/Memory/qgl_mem_tmpl.h"

namespace qgl::mem
{
   void print_mem_leak(void* address)
   {
      wchar_t outStr[24] = { 0 };
      swprintf(outStr, 24, L"Leak at %x\n", address);
      OutputDebugString(outStr);
   }

   void print_mem_dealloc(void* address)
   {
      wchar_t outStr[24] = { 0 };
      swprintf(outStr, 24, L"Free at %x\n", address);
      OutputDebugString(outStr);
   }

   void print_mem_alloc(void* address)
   {
      wchar_t outStr[24] = { 0 };
      swprintf(outStr, 24, L"Alloc at %x\n", address);
      OutputDebugString(outStr);
   }
}