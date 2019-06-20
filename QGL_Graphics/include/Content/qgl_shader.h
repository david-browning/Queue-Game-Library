#pragma once
#include "include/qgl_graphics_include.h"
#include "include/Content/Content-Buffers/qgl_shader_header_buffer.h"

namespace qgl::content
{
   class QGL_GRAPHICS_API shader : public content_item
   {
      public:
      /*
       Creates a shader from the give shader byte code. The byte code must be 
       compiled. It cannot be source code.
       */
      shader(const buffers::SHADER_HEADER_BUFFER* hdr_p,
             const void* data,
             size_t dataSizeBytes,
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

      /*
       Returns the type of shader.
       */
      buffers::SHADER_TYPES type() const noexcept;

      /*
       Returns the shader vendor.
       */
      buffers::SHADER_VENDORS vendor() const noexcept;

      private:
      buffers::SHADER_HEADER_BUFFER m_header;
      D3D12_SHADER_BYTECODE m_byteCode;
      uint8_t* m_data = nullptr;
   };
}