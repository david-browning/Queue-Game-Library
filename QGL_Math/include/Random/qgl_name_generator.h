#pragma once
#include "include/qgl_math_includes.h"

namespace qgl::math::random
{
   template<typename CharT>
   class basic_name_generator
   {
      public:
      template<typename ForwardIt>
      basic_name_generator(ForwardIt first, ForwardIt last) :
         m_names(first, last)
      {

      }

      basic_name_generator(const basic_name_generator&) = default;

      basic_name_generator(basic_name_generator&&) = default;

      ~basic_name_generator() noexcept = default;

      const std::basic_string<CharT>& get() const
      {
         return qgl::math::random::choose(m_names);
      }

      private:
      std::vector<std::basic_string<CharT>> m_names;
   };

   using name_generator = basic_name_generator<char>;
   using wname_generator = basic_name_generator<wchar_t>;
}