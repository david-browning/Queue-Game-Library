#pragma once
#include "include/qgl_model_include.h"
#include "include/qgl_version.h"

namespace qgl
{
   class QGL_MODEL_API qgl_bootstrapper
   {
      public:
      /*
       Searches the directory for the bootstrapper file and loads it. 
       Throws std::invalid_argument if the bootstrap file is not found.
       */
      qgl_bootstrapper(const wchar_t* installDir);

      /*
       Copy constructor.
       */
      qgl_bootstrapper(const qgl_bootstrapper& r);

      /*
       Move constructor.
       */
      qgl_bootstrapper(qgl_bootstrapper&& r);

      /*
       Destructor.
       */
      virtual ~qgl_bootstrapper() noexcept;

      /*
       Returns an absolute directory path to where the preferences are located.
       This will end with a backslash on Windows, or a forward slash on other 
       platforms.
       */
      const wchar_t* preferences_dir() const noexcept;

      /*
       Returns an absolute directory path to where the content store is 
       located. This will end with a backslash on Windows, or a forward slash
       on other platforms.
       */
      const wchar_t* content_dir() const noexcept;

      const GUID* launch_point() const noexcept;

      /*
       Returns the version.
       */
      qgl_version_t version() const noexcept;

      private:
      struct impl;
      impl* m_impl_p;
   };
}