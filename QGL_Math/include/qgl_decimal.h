#pragma once
#include "include/qgl_math_includes.h"
#include "include/qgl_math_funcs.h"
#include <QGLTraits.h>

namespace qgl::math
{
   /*
    Represents a number between -1 and 1, inclusive;
    Stores a numerator. The denominator is the template constant.
    This only allocates enough memory needed to store the denominator.
    Ex: A denominator of 255 will only allocate 1 byte.
    */
#pragma pack(push, 1)
   template<size_t Denominator>
   struct decimal
   {
      static_assert(Denominator > 0, "Denominator must be greater than 0.");

      public:
      using numerator_type =
         typename make_type<mem::smallest_type<Denominator>::value>::type;

      /*
       Stores the number 0.
       */
      constexpr decimal()
      {
      }

      template<typename T>
      constexpr decimal(T num) :
         m_numerator(num)
      {
      }

      template<>
      constexpr decimal(float x) :
         m_numerator(numerator_type(x * float(denominator())))
      {
      }

      template<>
      constexpr decimal(double x) :
         m_numerator(numerator_type(x * double(denominator())))
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
      numerator_type m_numerator = 0;
   };
#pragma pack(pop)

   static_assert(sizeof(decimal<255>) == 1,
                 "Decimal size if not correct (255)");

   static_assert(sizeof(decimal<1000>) == 2,
                 "Decimal size if not correct (255)");

   static_assert(sizeof(decimal<67000>) == 4,
                 "Decimal size if not correct (6700)");

   static_assert(sizeof(decimal<1099511627776>::numerator_type) == 8,
                 "Decimal size if not correct (1,099,511,627,776)");

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