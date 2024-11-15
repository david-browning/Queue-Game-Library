#pragma once
#include "include/Memory/qgl_bit_helpers.h"

namespace qgl
{
#pragma warning(push)
#pragma warning(disable : 4310)
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
#pragma warning(pop)

   template<class T>
   struct has_size
   {
      template<typename U>
      static auto check(int) -> decltype(std::declval<U>().size(),
                                         std::true_type());

      template<typename>
      static std::false_type check(...);

      public:
      static constexpr bool value = std::is_same<
         decltype(check<T>(0)),
         std::true_type>::value;
   };

   template<class T, bool = has_size<T>::value>
   struct get_size;

   template<class T>
   struct get_size<T, false>
   {
      size_t operator()(const T& val)
      {
         return sizeof(T);
      }
   };

   template<class T>
   struct get_size<T, true>
   {
      size_t operator()(const T& val)
      {
         return sizeof(T[0]) * val.size();
      }
   };

   /*
 Default make_type for signed type.
 */
   template<size_t Bits, bool Usgnd = false>
   struct make_type
   {
      typedef int64_t type;
   };

   template<>
   struct make_type<8, false>
   {
      typedef int8_t type;
   };

   template<>
   struct make_type<16, false>
   {
      typedef int16_t type;
   };

   template<>
   struct make_type<24, false>
   {
      typedef int32_t type;
   };

   template<>
   struct make_type<32, false>
   {
      typedef int32_t type;
   };

   /*
    Default make_type for unsigned type.
    */
   template<size_t Bits>
   struct make_type<Bits, true>
   {
      typedef uint64_t type;
   };

   template<>
   struct make_type<8, true>
   {
      typedef uint8_t type;
   };

   template<>
   struct make_type<16, true>
   {
      typedef uint16_t type;
   };

   template<>
   struct make_type<24, true>
   {
      typedef uint32_t type;
   };

   template<>
   struct make_type<32, true>
   {
      typedef uint32_t type;
   };

   static_assert(sizeof(make_type<mem::round_bytes<1>::value>::type) == 1,
              "make_type did not allocate the right size (1)");

   static_assert(sizeof(make_type<mem::round_bytes<8>::value>::type) == 1,
                 "make_type did not allocate the right size (8)");

   static_assert(sizeof(make_type<mem::round_bytes<15>::value>::type) == 2,
                 "make_type did not allocate the right size (15)");

   static_assert(sizeof(make_type<mem::round_bytes<17>::value>::type) == 4,
                 "make_type did not allocate the right size (17)");

   static_assert(sizeof(make_type<mem::round_bytes<33>::value>::type) == 8,
                 "make_type did not allocate the right size (33)");

}