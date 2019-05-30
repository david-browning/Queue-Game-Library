#pragma once
#include "include/qgl_content_include.h"

namespace qgl::content
{
   /*
    Abstraction of a file handle. This is meant to abstract file access across
    any OS and be safe crossing API boundaries.
    */
   class QGL_CONTENT_API file_handle
   {
      public:
      file_handle() noexcept;

      /*
       Takes ownership of the platform handle. Do not close the value passed to
       this. When this is destructed, it will close the handle.
       */
      file_handle(file_handle_t platformHandle) noexcept;

      /*
       Do not allow copying file handles.
       */
      file_handle(const file_handle&) = delete;

      /*
       Moves managing a handle.
       */
      file_handle(file_handle&&);

      /*
       Closes the file handle.
       */
      virtual ~file_handle() noexcept;

      /*
       Returns true if the file handle references a valid file.
       */
      bool valid() const;

      /*
       Closes the file handle.
       */
      void close() noexcept;

      /*
       Returns the platform-dependent underlying handle.
       */
      file_handle_t get() const noexcept;

      void attach(file_handle_t hndl) noexcept;

      file_handle_t detach() noexcept;

      file_handle_t* put() noexcept;

      explicit operator bool() const noexcept
      {
         return valid();
      }

      file_handle& operator=(file_handle&& other) noexcept
      {
         if (this != &other)
         {
            attach(other.detach());
         }

         return *this;
      }

      friend void swap(file_handle& first, file_handle& second) noexcept
      {
         using std::swap;
         swap(first.m_handle, second.m_handle);
      }

      private:
      file_handle_t m_handle;

      #ifdef WIN32
      static constexpr file_handle_t INVALID_FILE_HANDLE = INVALID_HANDLE_VALUE;
      #else
      //'nix OSes use -1 as invalid file handle.
      static constexpr file_handle_t INVALID_FILE_HANDLE = -1;
      #endif
   };
}