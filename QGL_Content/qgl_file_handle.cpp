#include "pch.h"
#include "include\qgl_file_handle.h"

namespace qgl::content
{
   #ifdef WIN32
   static const file_handle_t INVALID_FILE_HANDLE = INVALID_FILE_HANDLE;
   #else
   //'nix OSes use -1 as invalid file handle.
   static constexpr file_handle_t INVALID_FILE_HANDLE = -1;
   #endif


   file_handle::file_handle() noexcept :
      m_handle(INVALID_FILE_HANDLE)
   {
   }

   file_handle::file_handle(file_handle_t platformHandle) noexcept :
      m_handle(platformHandle)
   {
   }

   file_handle::file_handle(file_handle&& m) :
      m_handle(m.detach())
   {
   }

   file_handle::~file_handle() noexcept
   {
      close();
   }

   file_handle_t file_handle::get() const noexcept
   {
      return m_handle;
   }
   
   file_handle_t* file_handle::put() noexcept
   {
      assert(!valid());
      return &m_handle;
   }

   void file_handle::attach(file_handle_t hndl) noexcept
   {
      close();
      *put() = hndl;
   }

   file_handle_t file_handle::detach() noexcept
   {
      auto hndl = m_handle;
      m_handle = INVALID_FILE_HANDLE;
      return hndl;
   }

   bool file_handle::valid() const noexcept
   {
      return m_handle != INVALID_FILE_HANDLE;
   }

   void file_handle::close() noexcept
   {
      #ifdef WIN32
      CloseHandle(m_handle);
      #else
      close(m_handle);
      #endif
      m_handle = INVALID_FILE_HANDLE;
   }
}