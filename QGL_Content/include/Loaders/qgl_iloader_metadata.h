#pragma once
#include "include/qgl_content_include.h"

namespace qgl::content
{
   /*
    The loader metadata is a class used to used to convert an object to and 
    from a serializable binary buffer. This is stable at ABI boundaries.
    Instantiate this by using the "make_loader_from_json" factory function.
    */
   QGL_INTERFACE iloader_metadata : public iqgl
   {
      public:
      /*
       Writes the loaders's GUID to the pointer "g".
       */
      virtual void id(qgl::guid* g) const noexcept = 0;

      /*
       Transforms "bytes" from the fileData pointer and puts the result in 
       "out_p". "out_p" must be large enough to hold "size_p" bytes.
       You can get the required size by passing nullptr to "out_p".
       */
      virtual result_t input(uint64_t bytes,
                             const void* fileData_p,
                             uint64_t* size_p,
                             void* out_p) noexcept = 0;

      /*
       Gets the data that can be serialized to a file and puts it in 
       "fileData_p". "fileData_p" must be large enough to hold "size_p" bytes.
       You can get the required size by passing nullptr to "out_p".
       */
      virtual result_t output(uint64_t bytes,
                             const void* object_p,
                             uint64_t* size_p,
                             void* fileData_p) noexcept = 0;
   };

   /*
    Creates a loader from a JSON string. Only 1 loader can be specified
    in the string.

   {
      "id" : "5B8B3826ECB84825A3508D149359A72E",
      "module" : "customloaders.dll",
      "load" : "load_int32",
      "save" : "save_int32"
   }

    Returns the following:
    S_OK: success,
    E_INVALIDARG: out_p is null.
    E_FAIL: A JSON element has the wrong type.
    HRESULT with facility "FACILITY_QGL" on JSON parse error.
    E_OUTOFMEMORY: If the metadata cannot be allocated.
    E_UNEXPECTED: On other errors.
    */
   extern "C" QGL_MODEL_API result_t QGL_CC make_loader_from_json(
      const char* str,
      const imodule* mod_p,
      iloader_metadata** out_p) noexcept;
}