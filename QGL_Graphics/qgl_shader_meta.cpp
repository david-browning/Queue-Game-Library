#include "pch.h"
#include "include/Metrics/qgl_shader_meta.h"

namespace qgl::graphics::metrics
{
   struct shader_meta::impl
   {
      std::vector<uint32_t> m_cBindings;
      std::vector<uint32_t> m_tBindings;
      std::vector<uint32_t> m_sBindings;
   };
}