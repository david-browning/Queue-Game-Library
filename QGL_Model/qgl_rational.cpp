#include "pch.h"
#include "include/qgl_rational.h"

namespace qgl::math
{
   void swap(rational& l, rational& r) noexcept
   {
      using std::swap;
      swap(l.m_den, r.m_den);
      swap(l.m_num, r.m_num);
   }

   bool operator==(const rational& l, const rational & r) noexcept
   {
      auto lSimplify = l;
      auto rSimplify = r;
      return (lSimplify.m_num == rSimplify.m_num) &&
         (lSimplify.m_den == rSimplify.m_den);
   }

   bool operator!=(const rational& l, const rational& r) noexcept
   {
      return !(l == r);
   }

   constexpr rational::rational() :
      m_den(1),
      m_num(0)
   {

   }

   constexpr rational::rational(number_t num, number_t den) :
      m_den(den),
      m_num(num)
   {
      if (den == 0)
      {
         throw std::invalid_argument("The denominator cannot be zero.");
      }

      simplify();
   }

   rational& rational::operator=(rational r) noexcept
   {
      swap(*this, r);
      return *this;
   }

   rational& rational::operator=(rational&& r)
   {
      m_den = std::move(r.m_den);
      m_num = std::move(r.m_num);
      return *this;
   }

   float rational::operator()() const noexcept
   {
      return static_cast<float>(m_num) / static_cast<float>(m_den);
   }

   constexpr void rational::simplify()
   {
      auto g = gcd(m_num, m_den);
      m_num = m_num / g;
      m_den = m_den / g;
   }
}