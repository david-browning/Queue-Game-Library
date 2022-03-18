#pragma once
#include "include/qgl_math_includes.h"
#include "include/qgl_math_funcs.h"

namespace qgl::math
{
#pragma pack(push, 1)
   template<size_t DecimalPlaces>
   struct decimal
   {
      private:

      struct bit_denom
      {
         static constexpr auto value = power<size_t>(10, DecimalPlaces - 1);
      };

      template<size_t Denominator>
      struct bit_round
      {
         static constexpr auto value = round_up(
            qgl::mem::msb(Denominator), static_cast<size_t>(8));
      };

      template<size_t Bits>
      struct make_type
      {
         typedef int64_t type;
      };

      template<>
      struct make_type<8>
      {
         typedef int8_t type;
      };

      template<>
      struct make_type<16>
      {
         typedef int16_t type;
      };

      template<>
      struct make_type<24>
      {
         typedef int32_t type;
      };

      template<>
      struct make_type<32>
      {
         typedef int32_t type;
      };

      public:
      using numerator_type =
         typename make_type<bit_round<bit_denom::value>::value>::type;

      /*
       Stores the number 0.
       */
      constexpr decimal() :
         m_numerator(0)
      {}

      constexpr decimal(numerator_type num) :
         m_numerator(num)
      {}

      constexpr decimal(float x) :
         m_numerator(denominator() * x)
      {}

      constexpr decimal(double x) :
         m_numerator(denominator() * x)
      {}

      decimal(const decimal&) = default;

      decimal(decimal&&) = default;

      ~decimal() noexcept = default;

      constexpr auto numerator() const noexcept
      {
         return m_numerator;
      }

      constexpr auto denominator() const noexcept
      {
         return bit_denom::value;
      }

      template<size_t DecPlaces>
      friend void swap(decimal<DecPlaces>& l, decimal<DecPlaces>& r) noexcept
      {
         using std::swap;
         swap(l.m_numerator, r.m_numerator);
      }

      decimal& operator=(decimal r) noexcept
      {
         swap(*this, r);
         return *this;
      }

      decimal& operator=(decimal&&) = default;

      explicit inline operator float() const noexcept
      {
         return static_cast<float>(numerator()) /
            static_cast<float>(denominator());
      }

      explicit inline operator double() const noexcept
      {
         return static_cast<double>(numerator()) /
            static_cast<double>(denominator());
      }

      private:
      numerator_type m_numerator = 0;
   };
#pragma pack(pop)
}