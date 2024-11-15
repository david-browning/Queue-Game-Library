#pragma once
#include "include/qgl_model_include.h"
#include "include/qgl_console.h"
#include "include/Structures/qgl_slim_list.h"

namespace qgl
{
   /*
    Encapsulates consoles and prints messages to them.
    This only holds references to consoles. Do not allow any console passed to
    this class to be released before this class is destroyed.
    */
   template<typename CharT, class SRWTraits>
   class error_reporter final
   {
      public:
      using console_t = typename basic_console<CharT, SRWTraits>;
      using console_ptr = typename console_t*;
      using console_ref = typename std::reference_wrapper<console_t>;
      /*
       Copies the console pointer to internal storage.
       ConsoleForwardIt must point to a console_ptr.
       */
      template<class ConsoleForwardIt>
      error_reporter(ConsoleForwardIt first, ConsoleForwardIt last) :
         m_consoles(first, last)
      {
         using itT = typename std::remove_const<
            std::remove_reference<decltype(*first)>::type>::type;
         static_assert(std::is_same<itT, console_t>::value,
                       "first must point to a console_ptr or wconsole_ptr");
      }

      /*
       Copies the list of console pointers to internal storage.
       */
      error_reporter(std::initializer_list<console_ptr> consoles) :
         m_consoles(consoles)
      {

      }

      /*
       Copies the list of console pointers to internal storage.
       */
      error_reporter(std::initializer_list<console_ref> consoles)
      {
         for (auto c : consoles)
         {
            m_consoles.push_back(std::addressof(c.get()));
         }
      }

      error_reporter(const error_reporter&) = default;

      error_reporter(error_reporter&&) noexcept = default;

      ~error_reporter() noexcept = default;

      void insert(console_t& c)
      {
         m_consoles.push_back(std::addressof(c));
      }

      void insert(console_ptr p)
      {
         m_consoles.push_back(p);
      }

      /*
       This is linear complexity.
       */
      void erase(console_ptr c)
      {
         std::remove_if(m_consoles.begin(),
                        m_consoles.end(),
                        console_finder{ c });
      }


      /*
       This is linear complexity.
       */
      void erase(console_t& c)
      {
         std::remove_if(m_consoles.begin(),
                        m_consoles.end(),
                        console_finder{ std::addressof(c) });
      }

      /*
       Prints the string to the consoles.
       This is linear complexity.
       */
      void print(const CharT* s) noexcept
      {
         m_consoles.lock();
         std::basic_string<CharT> str{ s };
         for (auto& c : m_consoles)
         {
            c->cout(str);
         }
         m_consoles.unlock();
      }

      /*
       Prints the string to the consoles.
       This is linear complexity.
       */
      void print(const std::string& s) noexcept
      {
         m_consoles.lock();
         for (auto& c : m_consoles)
         {
            c->cout(s);
         }
         m_consoles.unlock();
      }

      private:
      struct console_finder
      {
         public:
         console_finder(console_ptr toFind) :
            m_toFind(toFind)
         {

         }

         bool operator()(console_ptr r) const noexcept
         {
            return r == m_toFind;
         }

         private:
         console_ptr m_toFind;
      };

      slim_list<console_ptr, SRWTraits> m_consoles;
   };
}