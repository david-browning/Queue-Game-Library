#pragma once
#include "include/qgl_model_include.h"
#include "include/Interfaces/qgl_icommand.h"

namespace qgl
{
   template<typename CharT>
   void default_con_out(const std::basic_string<CharT>& s)
   {
      OutputDebugStringA(s.c_str());
   }

   template<>
   void default_con_out(const std::wstring& s)
   {
      OutputDebugStringW(s.c_str());
   }

   /*
    Console operations are thread safe.
    */
   template<typename CharT>
   class console
   {
      public:
      using str_type = std::basic_string<CharT>;
      using console_cmd = typename icommand<const str_type&>;

      /*
       Signature for a function that will be raised when the console has output.
       */
      using cout_callback = std::function<void(const str_type&)>;

      using cmd_callback = std::function<void(const str_type&)>;

      console()
      {
#ifdef DEBUG
         insert_cout(default_con_out<CharT>);
#endif
      }

      console(const console&) = default;

      console(console&&) = default;

      ~console() noexcept = default;

      /*
       Registers a callback to be raised whenever the console has new output.
       */
      void insert_cout(cout_callback callback)
      {
         std::lock_guard threadLock{ m_threadLock };
         m_coutCallbacks.insert(callback);
      }

      /*
       Revokes a callback that is raised whenever the console has new output.
       */
      void erase_cout(cout_callback callback)
      {
         std::lock_guard threadLock{ m_threadLock };
         m_coutCallbacks.erase(callback);
      }

      void insert_cin(const str_type& cmd, console_cmd callback)
      {
         std::lock_guard threadLock{ m_threadLock };
         m_cinCmds.insert_or_assign(cmd, callback);
      }

      void erase_cin(const str_type& cmd)
      {
         std::lock_guard threadLock{ m_threadLock };
         m_cinCmds.erase(cmd);
      }

      void cout(const str_type& s) noexcept
      {
         std::lock_guard threadLock{ m_threadLock };
         m_lines.push_back(s);
         for (const auto& f : m_coutCallbacks)
         {
            f(s);
         }
      }

      void cin(const str_type& s) noexcept
      {
         std::lock_guard threadLock{ m_threadLock };
         auto spc = s.find_first_of(' ');
         auto cmd = s.substr(0, spc);
         if (m_cinCmds.count(cmd) > 0)
         {
            m_cinCmds.at(cmd).execute(s.substr(spc));
         }
      }

      /*
       Clears the console buffer.
       */
      void clear()
      {
         std::lock_guard threadLock{ m_threadLock };
         m_lines.clear();
      }

      /*
       Returns a dump of the console buffer.
       */
      str_type dump() const
      {
         std::lock_guard threadLock{ m_threadLock };
         std::basic_stringstream<CharT> stm;
         for (const auto& s : m_lines)
         {
            stm << s << std::endl;
         }

         return stm.str();
      }

      private:
      std::vector<str_type> m_lines;
      std::unordered_set<cout_callback> m_coutCallbacks;
      std::unordered_map<str_type, console_cmd> m_cinCmds;
      std::mutex m_threadLock;
   };
}