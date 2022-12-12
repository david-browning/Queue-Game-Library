#pragma once
#include "include/qgl_model_include.h"
#include "include/qgl_console.h"

namespace qgl
{
   /*
    Encapsulates consoles and prints messages to them.
    */
   template<typename CharT>
   class error_reporter final
   {
      public:
      /*
       Copies the console pointer to internal storage.
       ConsoleForwardIt must point to a console_ptr.
       */
      template<class ConsoleForwardIt>
      error_reporter(ConsoleForwardIt first, ConsoleForwardIt last) :
         m_consoles(first, last)
      {
         using itT = typename std::remove_reference<decltype(*first)>::type;
         static_assert(std::is_same<itT, basic_console_ptr<CharT>>::value,
                       "first must point to a console_ptr or wconsole_ptr");
      }

      /*
       Copies the list of console pointers to internal storage.
       */
      error_reporter(std::initializer_list<basic_console_ptr<CharT>> consoles) :
         m_consoles(consoles)
      {

      }

      error_reporter(const error_reporter&) = default;

      error_reporter(error_reporter&&) noexcept = default;

      ~error_reporter() noexcept = default;

      /*
       Prints the string to the consoles.
       */
      void print(const CharT* s) noexcept
      {
         std::basic_string<CharT> str{ s };
         for (auto& c : m_consoles)
         {
            c->cout(str);
         }
      }

      /*
       Prints the string to the consoles.
       */
      void print(const std::string& s) noexcept
      {
         for (auto& c : m_consoles)
         {
            c->cout(s);
         }
      }

      private:
      std::list<basic_console_ptr<CharT>> m_consoles;
   };
}