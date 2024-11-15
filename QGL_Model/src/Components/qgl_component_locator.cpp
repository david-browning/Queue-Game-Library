#include "pch.h"
#include <libloaderapi.h>
#include "include/Components/qgl_icomponent_provider.h"

namespace qgl
{
   result_t QGL_CC qgl::components::component_prodiver_proc(
      const imodule* h_p, get_component_provider_functor* out_p) noexcept
   {
      return h_p->address(QGL_COMPONENT_PROVIDER_ENTRY_POINT,
                          reinterpret_cast<uint64_t*>(out_p));
   }
}