#pragma once
#include "include/qgl_math_includes.h"
#include "include/qgl_math_funcs.h"

namespace qgl::math
{
#pragma pack(push, 1)
   template<size_t Denominator>
   struct decimal
   {
      static_assert(Denominator > 0, "Denominator must be greater than 0.");

      private:

      template<size_t Denom>
      struct bit_round
      {
         static constexpr auto value = round_up(
            qgl::mem::msb(Denom) + 1, static_cast<size_t>(8));
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
         typename make_type<bit_round<Denominator>::value>::type;

      /*
       Stores the number 0.
       */
      constexpr decimal() :
         m_numerator(0)
      {
      }

      template<typename T>
      constexpr decimal(T num) :
         m_numerator(num)
      {
      }

      template<>
      constexpr decimal(float x) :
         m_numerator(denominator()* x)
      {
      }

      template<>
      constexpr decimal(double x) :
         m_numerator(denominator()* x)
      {
      }

      decimal(const decimal&) = default;

      decimal(decimal&&) = default;

      ~decimal() noexcept = default;

      constexpr numerator_type numerator() const noexcept
      {
         return m_numerator;
      }

      constexpr numerator_type denominator() const noexcept
      {
         return Denominator;
      }

      friend void swap(decimal& l, decimal& r) noexcept
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
      numerator_type m_numerator;
   };
#pragma pack(pop)

   template<size_t LDenom, size_t RDenom>
   inline bool operator==(const decimal<LDenom>& l,
                                 const decimal<RDenom>& r) noexcept
   {
      return (l.numerator() * r.denominator() -
         r.numerator() * l.denominator()) == 0;
   }

   template<size_t LDenom, size_t RDenom>
   inline bool operator!=(const decimal<LDenom>& l,
                                 const decimal<RDenom>& r) noexcept
   {
      return !(l == r);
   }
}