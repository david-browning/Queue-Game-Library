#pragma once
#include "include/qgl_model_include.h"
#include <type_traits>

namespace qgl::math
{
   template<typename NumberT>
   constexpr NumberT gcd(NumberT a, NumberT b)
   {
      return a == 0 ? a : gcd(b, a % b);
   }

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
      operator float() const noexcept
      {
         return static_cast<float>(m_num) / static_cast<float>(m_den);
      }

      operator double() const noexcept
      {
         return static_cast<double>(m_num) / static_cast<double>(m_den);
      }

      /*
       Adds two rational numbers.
       */
      //friend rational operator+(const rational& l, const rational& r);

      /*
       Subtracts two rational numbers.
       */
      //friend rational operator-(const rational& l, const rational& r);

      /*
       Multiplies two rational numbers.
       */
      //friend rational operator*(const rational& l, const rational& r);

      /*
       Returns l / r.
       */
      //friend rational operator/(const rational& l, const rational& r);

      /*
       Returns the rational number * -1.
       */
      //rational& operator-() noexcept;

      /*
       Returns true l and r evaluate to the same value.
       */
      //friend bool operator==(const rational& l, const rational& r) noexcept;

      /*
       Returns false if l and r evaluate to the same value.
       */
      friend bool operator!=(const rational& l, const rational& r) noexcept
      {
         return !(l == r);
      }

      /*
       Returns true if the value of l is less than the value of r.
       */
      //friend bool operator<(const rational& l, rational& r) noexcept;

      /*
       Returns true if the value of l is greater than the value of r.
       */
      //friend bool operator>(const rational& l, rational& r) noexcept;

      /*
       Returns true if the value of l is less than or equal to the value of r.
       */
      //friend bool operator<=(const rational& l, rational& r) noexcept;

      /*
       Returns true if the value of l is greater than or equal to the value
       of r.
       */
       //friend bool operator>=(const rational& l, rational& r) noexcept;

      private:

      /*
       Simplifies the rational number.
       */
      constexpr void simplify()
      {
         auto g = gcd(m_num, m_den);
         m_num = m_num / g;
         m_den = m_den / g;
      }

      NumberT m_num;
      NumberT m_den;
   };
   #pragma pack(pop)

   template<> struct QGL_MODEL_API rational<int8_t>;
   template<> struct QGL_MODEL_API rational<int16_t>;
   template<> struct QGL_MODEL_API rational<int32_t>;
   template<> struct QGL_MODEL_API rational<int64_t>;
}
