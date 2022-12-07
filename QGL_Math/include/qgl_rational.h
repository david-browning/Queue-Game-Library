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
      constexpr rational(const rational&) = default;

      /*
       Move constructor.
       */
      constexpr rational(rational&&) noexcept = default;

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
      rational& operator=(rational&& r) = default;

      /*
       Returns a floating point representation of the rational number.
       */
      explicit operator float() const noexcept
      {
         return static_cast<float>(m_num) / static_cast<float>(m_den);
      }

      explicit operator double() const noexcept
      {
         return static_cast<double>(m_num) / static_cast<double>(m_den);
      }

      /*
       Adds two rational numbers.
       */
      friend rational operator+(const rational& l, const rational& r)
      {
         //(A / B) + (C / D) = (AD + BC) / BD
         auto B = l.denominator();
         auto D = r.denominator();

         return rational(l.numerator() * D + B * r.numerator(),
                         B * D).simplified();
      }

      /*
       Subtracts two rational numbers.
       */
      friend rational operator-(const rational& l, const rational& r)
      {
         //(A / B) - (C / D) = (AD - BC) / BD
         auto B = l.denominator();
         auto D = r.denominator();

         return rational(l.numerator() * D - B * r.numerator(),
                         B * D).simplified();
      }

      /*
       Multiplies two rational numbers.
       */
      friend rational operator*(const rational& l, const rational& r)
      {
         return rational(l.numerator() * r.numerator(),
                         l.denominator() * r.denominator()).simplified();
      }

      /*
       Returns l / r.
       */
      friend rational operator/(const rational& l, const rational& r)
      {
         return rational(l.numerator() * r.denominator(),
                         l.denominator() * r.numerator()).simplified();
      }

      /*
       Returns true l and r evaluate to the same value.
       */
      friend bool operator==(const rational& l,
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
      friend bool operator!=(const rational& l,
                             const rational& r) noexcept
      {
         return !(l == r);
      }

      /*
       Returns true if the value of l is less than the value of r.
       */
      friend bool operator<(const rational& l, rational& r) noexcept
      {
         return rational::msub(l, r) < NumberT(0);
      }

      /*
       Returns true if the value of l is greater than the value of r.
       */
      friend bool operator>(const rational& l, rational& r) noexcept
      {
         return rational::msub(l, r) > NumberT(0);
      }

      /*
       Returns true if the value of l is less than or equal to the value of r.
       */
      friend bool operator<=(const rational& l, rational& r) noexcept
      {
         return !(l > r);
      }

      /*
       Returns true if the value of l is greater than or equal to the value
       of r.
       */
      friend bool operator>=(const rational& l, rational& r) noexcept
      {
         return  !(l < r);
      }

      rational simplified() noexcept
      {
         simplify();
         return *this;
      }

      template<typename U>
      friend std::ostream& operator<<(std::ostream& os,
                                      const rational<U>& r);

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
       l = A/B
       r = C/D
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
   
   template<typename U>
   std::ostream& operator<<(std::ostream& os, const rational<U>& r)
   {
      os << r.numerator() << '/' << r.denominator();
      return os;
   }
}
