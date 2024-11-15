#include "pch.h"
#include "include/Loaders/qgl_iloader_provider.h"

namespace qgl::content
{
   QGL_CONTENT_API result_t QGL_CC loader_prodiver_proc(
      const imodule* h_p,
      get_loader_provider_functor* out_p) noexcept
   {
      return h_p->address(QGL_LOADER_PROVIDER_ENTRY_POINT, 
                          reinterpret_cast<uint64_t*>(out_p));
   }
}