#pragma once
#include "include/qgl_content_include.h"
#include "include/Loaders/qgl_iloader_metadata.h"

namespace qgl::content
{
   QGL_INTERFACE iloader_provider : public iqgl
   {
      public:
      /*
       Returns the number of loaders in the provider.
       */
      virtual uint32_t size() const noexcept = 0;

      /*
       Places the "idx'th" loader metadata into "out_p". Deallocate out_p by
       calling "release()".
       */
      virtual result_t metadata(uint32_t idx,
                                iloader_metadata** out_p) const noexcept = 0;
   };

   /*
    The entry point which will return the module's available loaders.
    It's signature is "get_loader_provider_functor".
    */
   static constexpr auto QGL_LOADER_PROVIDER_ENTRY_POINT = "qgl_get_loaders";

   /*
    Signature of a function that returns a module's available loaders.
    */
   typedef qgl::result_t(QGL_CC* get_loader_provider_functor)(
      qgl_version, iloader_provider**) noexcept;

   /*
    Gets the address of the loader provider function and puts the result
    in "out_p".
    */
   extern "C" QGL_CONTENT_API result_t QGL_CC loader_prodiver_proc(
      const imodule* h_p, get_loader_provider_functor* out_p) noexcept;
}