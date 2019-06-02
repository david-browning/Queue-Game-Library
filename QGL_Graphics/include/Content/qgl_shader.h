#pragma once
#include "include/qgl_graphics_include.h"

namespace qgl::content
{
   class QGL_GRAPHICS_API shader : public content_item
   {
      public:
      /*
       Creates a shader from the give shader byte code.
       */
      shader(const void* data,
             size_t dataSizeBytes,
             const wchar_t* name,
             content_id id);

      /*
       csoFile: Absolute path to a compiled shader object file. This is not a 
        QGL content file, it is the raw byte data from a D3D shader compile.
       */
      shader(const wchar_t* csoFile,
             const wchar_t* name,
             content_id id);

      /*
       Copy constructor.
       */
      shader(const shader&);

      /*
       Move constructor.
       */
      shader(shader&&);

      /*
       Destructor.
       */
      virtual ~shader() noexcept;

      /*
       Returns a const pointer to the D3D shader byte code.
       */
      const D3D12_SHADER_BYTECODE* byte_code() const noexcept;

      private:
      D3D12_SHADER_BYTECODE m_byteCode;
      uint8_t* m_data = nullptr;
   };
}