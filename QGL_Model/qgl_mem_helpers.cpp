#include "pch.h"
#include "include/Memory/qgl_mem_helpers.h"
#include "include/Debug/qgl_debug.h"
#include "include/String/qgl_strings.h"

namespace qgl::mem
{
   static const sys_char* ALLOC_MESSAGE_FMT = TEXT("Alloc at %\n");
   static const sys_char* DEALLOC_MESSAGE_FMT = TEXT("Free at %\n");
   static const sys_char* LEAK_MESSAGE_FMT = TEXT("Leak at %\n");

   static constexpr size_t MEM_ALERT_STR_LEN = 24;

   void print_mem_leak(uintptr_t address)
   {
      qgl::debug_message(qgl::tsprintf(LEAK_MESSAGE_FMT, address).c_str());
   }

   void print_mem_dealloc(uintptr_t address)
   {
      qgl::debug_message(qgl::tsprintf(DEALLOC_MESSAGE_FMT, address).c_str());
   }

   void print_mem_alloc(uintptr_t address)
   {
      qgl::debug_message(qgl::tsprintf(ALLOC_MESSAGE_FMT, address).c_str());
   }
}