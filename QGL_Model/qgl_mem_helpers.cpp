#include "pch.h"
#include "include/Memory/qgl_mem_helpers.h"
#include "include/Debug/qgl_debug.h"
#include "include/String/qgl_strings.h"

namespace qgl::mem
{
   static const sys_char* ALLOC_MESSAGE_FMT = TEXT("Alloc at %x\n");
   static const sys_char* DEALLOC_MESSAGE_FMT = TEXT("Free at %x\n");
   static const sys_char* LEAK_MESSAGE_FMT = TEXT("Leak at %x\n");

   static constexpr size_t MEM_ALERT_STR_LEN = 24;

   void print_mem_leak(uintptr_t address)
   {
      sys_char outStr[24] = { 0 };
      swprintf(outStr, 24, LEAK_MESSAGE_FMT, address);
      qgl::debug_message(outStr);
   }

   void print_mem_dealloc(uintptr_t address)
   {
      sys_char outStr[24] = { 0 };
      swprintf(outStr, 24, DEALLOC_MESSAGE_FMT, address);
      qgl::debug_message(outStr);
   }

   void print_mem_alloc(uintptr_t address)
   {
      sys_char outStr[24] = { 0 };
      swprintf(outStr, 24, ALLOC_MESSAGE_FMT, address);
      qgl::debug_message(outStr);
   }
}