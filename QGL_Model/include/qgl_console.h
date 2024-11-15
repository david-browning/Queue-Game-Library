#pragma once
#include "include/qgl_model_include.h"
#include "include/Interfaces/qgl_icommand.h"
#include "include/Structures/qgl_slim_vector.h"
#include "include/Structures/qgl_slim_umap.h"

namespace qgl
{
   template<typename CharT>
   inline void default_con_out(const std::basic_string<CharT>& s)
   {
      OutputDebugStringA(s.c_str());
   }

   template<>
   inline void default_con_out<wchar_t>(const std::wstring& s)
   {
      OutputDebugStringW(s.c_str());
   }

   template<typename CharT>
   inline std::basic_string<CharT> default_con_err(
      const std::basic_string<CharT>& s)
   {
      auto spc = s.find_first_of(' ');
      auto cmd = s.substr(0, spc);

      std::basic_stringstream<CharT> ss;
      ss << cmd << " is not a registered console command.";

      return ss.str();
   }

   template<>
   inline std::wstring default_con_err(const std::wstring& s)
   {
      auto spc = s.find_first_of(' ');
      auto cmd = s.substr(0, spc);

      std::wstringstream ws;
      ws << cmd << L" is not a registered console command.";

      return ws.str();
   }

   /*
    Represents a virtual console with an input stream and multiple output 
    streams. 
    Console operations are thread safe. Output operations will be written 
    cleanly, in order they arrive. Input commands will be executed in order 
    they arrive.
    The input stream is not buffered. Text written to the input stream will
    execute immediately regardless if the text ends with a newline or not.

    The template parameters specifies if the console uses single or multi-byte
    characters.
    */
   template<typename CharT, class SRWTraits>
   class basic_console final
   {
      public:
      using str_type = std::basic_string<CharT>;
      using iconsole_cmd = typename icommand<const str_type&>;

      /*
       Signature for a function that will be raised when the console has output.
       */
      using cout_callback = std::function<void(const str_type&)>;

      using cmd_callback = std::function<void(const str_type&)>;

      /*
       The arguments is the line of text that triggered the error.
       Anything returned by the callback will be written to the console.
       */
      using err_callback = std::function<str_type(const str_type&)>;

      basic_console(err_callback err = default_con_err<CharT>) :
         m_errCallback(err)
      {
#ifdef DEBUG
         insert_cout(default_con_out<CharT>);
#endif
      }

      basic_console(const basic_console&) = default;

      basic_console(basic_console&&) = default;

      ~basic_console() noexcept = default;

      friend void swap(basic_console& l, basic_console& r) noexcept
      {
         using std::swap;
         swap(l.m_errCallback, r.m_errCallback);
         swap(l.m_lines, r.m_lines);
         swap(l.m_cinCmds, r.m_cinCmds);
         swap(l.m_coutCallbacks, r.m_coutCallbacks);
      }

      basic_console& operator=(basic_console r)
      {
         swap(*this, r);
         return *this;
      }

      /*
       Registers a callback to be raised whenever the console has new output.
       */
      void insert_cout(cout_callback callback)
      {
         m_coutCallbacks.push_back(callback);
      }

      /*
       Revokes a callback that is raised whenever the console has new output.
       */
      void erase_cout(cout_callback callback)
      {
         auto found = std::find(m_coutCallbacks.begin(),
                                m_coutCallbacks.end(),
                                callback);
         
         if (found != m_coutCallbacks.end())
         {
            m_coutCallbacks.erase(found);
         }
      }

      /*
       Registers a command that will raise the given callback.
       */
      void insert_cin(const str_type& cmd, iconsole_cmd& callback)
      {
         m_cinCmds[cmd] = callback;
      }

      /*
       Removes a command from the console. If the command is written to the 
       input stream, nothing will happen.
       */
      void erase_cin(const str_type& cmd)
      {
         m_cinCmds.erase(cmd);
      }

      /*
       Writes "s" to the console's output.
       */
      void cout(const str_type& s) noexcept
      {
         m_lines.push_back(s);
         m_coutCallbacks.lock();
         for (const auto& f : m_coutCallbacks)
         {
            f(s);
         }
         m_coutCallbacks.unlock();
      }

      /*
       Writes "s" to the console's input buffer.
       The input stream is not buffered. Text written to the input stream will
       execute immediately regardless if the text ends with a newline or not.
       */
      void cin(const str_type& s) noexcept
      {
         auto spc = s.find_first_of(' ');
         auto cmd = s.substr(0, spc);

         m_cinCmds.lock();
         if (m_cinCmds.count(cmd) > 0)
         {
            m_cinCmds.at(cmd).get().execute(s.substr(spc));
         }
         else
         {
            cout(m_errCallback(s));
         }
         m_cinCmds.unlock();
      }

      /*
       Clears the console buffer.
       */
      void clear()
      {
         m_lines.clear();
      }

      /*
       Returns a dump of the console buffer. 
       */
      str_type dump()
      {
         std::basic_stringstream<CharT> stm;
         m_lines.lock();
         for (const auto& s : m_lines)
         {
            stm << s << std::endl;
         }
         m_lines.unlock();
         return stm.str();
      }

      private:
      std::reference_wrapper<err_callback> m_errCallback;
      slim_vector<str_type, SRWTraits> m_lines;
      slim_vector<cout_callback, SRWTraits> m_coutCallbacks;
      slim_umap<str_type, std::reference_wrapper<iconsole_cmd>, SRWTraits> m_cinCmds;
   };
}