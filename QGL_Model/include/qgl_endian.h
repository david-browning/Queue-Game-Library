#pragma once
#include <type_traits>

namespace qgl
{
   struct is_big_endian
   {
      private:
      static constexpr uint32_t m_toCheck = 0x01020304;
      static constexpr uint8_t m_endianess = (uint8_t)m_toCheck;

      public:
      static constexpr bool value = m_endianess == 0x01;
   };

   struct is_little_endian
   {
      private:
      static constexpr uint32_t m_toCheck = 0x01020304;
      static constexpr uint8_t m_endianess = (uint8_t)m_toCheck;

      public:
      static constexpr bool value = m_endianess == 0x04;
   };
   
} 