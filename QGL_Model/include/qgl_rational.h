#pragma once
#include "include/qgl_model_include.h"

namespace qgl::math
{
   template<typename NumberT>
   constexpr NumberT gcd(NumberT a, NumberT b)
   {
      return a == 0 ? a : gcd(b, a % b);
   }

   struct QGL_MODEL_API rational
   {
      public:
      using number_t = int32_t;

      /*
       Rational number evaluates to zero.
       */
      constexpr rational();

      /*
       Throws std::invalid_argument if denominator is 0.
       */
      constexpr rational(number_t num, number_t den);

      rational(const rational&) = default;

      rational(rational&&) = default;

      ~rational() noexcept = default;

      friend void swap(rational& l, rational& r) noexcept;
      rational& operator=(rational r) noexcept;
      rational& operator=(rational&& r);

      friend bool operator==(const rational& l, const rational& r) noexcept;
      friend bool operator!=(const rational& l, const rational& r) noexcept;

      float operator()() const noexcept;

      //friend rational operator+(const rational& l, const rational& r);
      //friend rational operator-(const rational& l, const rational& r);
      //friend rational operator*(const rational& l, const rational& r);
      //friend rational operator/(const rational& l, const rational& r);

      //rational& operator-() noexcept;

      //friend bool operator<(const rational& l, rational& r) noexcept;
      //friend bool operator>(const rational& l, rational& r) noexcept;
      //friend bool operator<=(const rational& l, rational& r) noexcept;
      //friend bool operator>=(const rational& l, rational& r) noexcept;

      private:

      constexpr void simplify();
      
      number_t m_num;
      number_t m_den;
   };
}
