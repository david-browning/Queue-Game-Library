#pragma once
#include "qgl_graphics_include.h"
#include "qgl_ishader.h"

namespace qgl::graphics
{
   class shader_meta
   {
      public:
      template<SHADER_TYPES ShaderT>
      shader_meta(const ishader<ShaderT>& shdr);

      shader_meta(const shader_meta& r);

      shader_meta(shader_meta&& r);

      virtual ~shader_meta() noexcept;

      friend void swap(shader_meta& first, shader_meta& second) noexcept;

      shader_meta& operator=(shader_meta r);

      private:
      void p_parse_byte_code(const D3D12_SHADER_BYTECODE& code);

      uint32_t p_parse_binding();

      #pragma warning(push)
      #pragma warning(disable: 4251)
      std::vector<uint32_t> m_cBindings;
      std::vector<uint32_t> m_tBindings;
      std::vector<uint32_t> m_sBindings;
      #pragma warning(pop)
   };
}