#pragma once
#include "include/qgl_math_includes.h"
#include <type_traits>

namespace qgl::math
{
   #pragma pack(push, 1)
   template<typename NumberT>
   struct rational
   {
      static_assert(std::is_integral<NumberT>::value,
                    "NumberT must be integral.");

      static_assert(std::is_signed<NumberT>::value,
                    "NumberT must be signed.");

      public:
      /*
       Rational number evaluates to zero.
       */
      constexpr rational() :
         m_den(1),
         m_num(0)
      {

      }

      /*
       Throws std::invalid_argument if denominator is 0.
       */
      constexpr rational(NumberT num, NumberT den) :
         m_den(den),
         m_num(num)
      {
         if (den == 0)
         {
            throw std::invalid_argument("The denominator cannot be zero.");
         }

         simplify();
      }

      /*
       Copy constructor
       */
      rational(const rational&) = default;

      /*
       Move constructor.
       */
      rational(rational&&) = default;

      /*
       Destructor
       */
      ~rational() noexcept = default;

      NumberT numerator() const noexcept
      {
         return m_num;
      }

      NumberT denominator() const noexcept
      {
         return m_den;
      }

      /*
       Swaps the content of l and r.
       */
      friend void swap(rational& l, rational& r) noexcept
      {
         using std::swap;
         swap(l.m_num, r.m_num);
         swap(l.m_den, r.m_den);
      }

      /*
       Assignment operator.
       */
      rational& operator=(rational r) noexcept
      {
         swap(*this, r);
         return *this;
      }

      /*
       Move assign operator.
       */
      //rational& operator=(rational&& r) = default;

      /*
       Returns a floating point representation of the rational number.
       */
      explicit inline operator float() const noexcept
      {
         return static_cast<float>(m_num) / static_cast<float>(m_den);
      }

      explicit inline operator double() const noexcept
      {
         return static_cast<double>(m_num) / static_cast<double>(m_den);
      }

      /*
       Adds two rational numbers.
       */
      //inline friend rational operator+(const rational& l, const rational& r);

      /*
       Subtracts two rational numbers.
       */
      //inline friend rational operator-(const rational& l, const rational& r);

      /*
       Multiplies two rational numbers.
       */
      //inline friend rational operator*(const rational& l, const rational& r);

      /*
       Returns l / r.
       */
      //inline friend rational operator/(const rational& l, const rational& r);

      /*
       Returns the rational number * -1.
       */
      //inline rational& operator-() noexcept;

      /*
       Returns true l and r evaluate to the same value.
       */
      inline friend bool operator==(const rational& l,
                                    const rational& r) noexcept
      {
         /*
          A/B == C/D if (AD - BC == 0)
         */
         return rational::msub(l, r) == NumberT(0);
      }

      /*
       Returns false if l and r evaluate to the same value.
       */
      inline friend bool operator!=(const rational& l,
                                    const rational& r) noexcept
      {
         return !(l == r);
      }

      /*
       Returns true if the value of l is less than the value of r.
       */
      inline friend bool operator<(const rational& l, rational& r) noexcept
      {
         return rational::msub(l, r) < NumberT(0);
      }

      /*
       Returns true if the value of l is greater than the value of r.
       */
      inline friend bool operator>(const rational& l, rational& r) noexcept
      {
         return rational::msub(l, r) > NumberT(0);
      }

      /*
       Returns true if the value of l is less than or equal to the value of r.
       */
      inline friend bool operator<=(const rational& l, rational& r) noexcept
      {
         return !(l > r);
      }

      /*
       Returns true if the value of l is greater than or equal to the value
       of r.
       */
      inline friend bool operator>=(const rational& l, rational& r) noexcept
      {
         return  !(l < r);
      }

      private:

      /*
       Simplifies the rational number.
       */
      constexpr void simplify()
      {
         auto g = std::gcd(m_num, m_den);
         m_num = m_num / g;
         m_den = m_den / g;
      }

      /*
       Returns AD - BC
       */
      static NumberT msub(const rational& l,
                          const rational& r) noexcept
      {
         return l.numerator() * r.denominator() - 
            l.denominator() * r.numerator();
      }

      NumberT m_num;
      NumberT m_den;
   };
   #pragma pack(pop)

   /*
    Explicit specializations of rational.
    The definitions are stored in pch.cpp.
    */
   QGL_MATH_TEMPLATE template struct QGL_MATH_API rational<int8_t>;
   QGL_MATH_TEMPLATE template struct QGL_MATH_API rational<int16_t>;
   QGL_MATH_TEMPLATE template struct QGL_MATH_API rational<int32_t>;
   QGL_MATH_TEMPLATE template struct QGL_MATH_API rational<int64_t>;
}
