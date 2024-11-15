#pragma once
#include <libloaderapi.h>
#include "include/Loaders/qgl_iloader_metadata.h"

/*
 Signature of a function that loads file data.
 */
typedef qgl::result_t(QGL_CC* load_file_data_functor)(
   uint64_t, const void*, uint64_t*, void*) noexcept;

/*
 Signature of a function that saves file data.
 */
typedef qgl::result_t(QGL_CC* save_file_data_functor)(
   uint64_t, const void*, uint64_t*, void*) noexcept;

/*
 This is a loader metadata where the load and save functors are named static
 functions in a module. Use this class to instantiate loader metadatas from
 a module when you know the load and save function names.
 */
class dynamic_loader_metadata : public qgl::content::iloader_metadata
{
   public:
   /*
    Constructor
    g: The ID for this loader.
    m_p: Pointer to the module that contains the loader functions.
    loadFunctorName: Name of the function in the module that loads.
    saveFunctorName: Name of the function in the module that saves.
    */
   dynamic_loader_metadata(const qgl::guid& g,
                           const qgl::imodule* m_p,
                           const char* loadFunctorName,
                           const char* saveFunctorName) :
      m_guid(g)
   {
      construct(m_p, loadFunctorName, saveFunctorName);
   }

   /*
    Constructor
    g: The ID for this loader. This parameter is moved so the passed variable is
      invalid after constructing this.
    m_p: Pointer to the module that contains the loader functions.
    loadFunctorName: Name of the function in the module that loads.
    saveFunctorName: Name of the function in the module that saves.
    */
   dynamic_loader_metadata(qgl::guid&& g,
                           const qgl::imodule* m_p,
                           const char* loadFunctorName,
                           const char* saveFunctorName) :
      m_guid(std::forward<qgl::guid>(g))
   {
      construct(m_p, loadFunctorName, saveFunctorName);
   }

   /*
    Constructor
    g: The ID for this loader. This parameter is moved so the passed variable is
      invalid after constructing this.
    m_p: Pointer to the module that contains the loader functions.
    loadFunctorName: Name of the function in the module that loads.
    saveFunctorName: Name of the function in the module that saves.
    */
   dynamic_loader_metadata(qgl::guid&& g,
                        const qgl::imodule* m_p,
                        const std::string& loadFunctorName,
                        const std::string& saveFunctorName) :
      m_guid(std::forward<qgl::guid>(g))
   {
      construct(m_p, loadFunctorName.c_str(), saveFunctorName.c_str());
   }

   /*
    Copy constructor
    */
   dynamic_loader_metadata(const dynamic_loader_metadata&) = default;

   /*
    Move constructor
    */
   dynamic_loader_metadata(dynamic_loader_metadata&&) noexcept = default;

   /*
    Destructor
    */
   virtual ~dynamic_loader_metadata() noexcept = default;

   virtual void release() noexcept
   {
      delete this;
   }

   virtual iqgl* duplicate() const noexcept
   {
      return new (std::nothrow)dynamic_loader_metadata(*this);
   }

   virtual void id(qgl::guid* g) const noexcept
   {
      memcpy(g, &m_guid, sizeof(qgl::guid));
   }

   /*
    Transforms "bytes" from the fileData pointer and puts the result in
    "out_p". "out_p" must be large enough to hold "size_p" bytes.
    You can get the required size by passing nullptr to "out_p".
    */
   virtual qgl::result_t input(uint64_t bytes,
                               const void* fileData_p,
                               uint64_t* size_p,
                               void* out_p) noexcept
   {
      return m_loadFunctor(bytes, fileData_p, size_p, out_p);
   }

   /*
    Gets the data that can be serialized to a file and puts it in
    "fileData_p". "fileData_p" must be large enough to hold "size_p" bytes.
    You can get the required size by passing nullptr to "out_p".
    */
   virtual qgl::result_t output(uint64_t bytes,
                               const void* object_p,
                               uint64_t* size_p,
                               void* fileData_p) noexcept
   {
      return m_saveFunctor(bytes, object_p, size_p, fileData_p);
   }

   private:
   void construct(const qgl::imodule* m_p,
                  const char* loadFunctorName,
                  const char* saveFunctorName)
   {
      auto loadProcRes = m_p->address(
         loadFunctorName, reinterpret_cast<uint64_t*>(&m_loadFunctor));
      auto saveProcRes = m_p->address(
         saveFunctorName, reinterpret_cast<uint64_t*>(&m_saveFunctor));

      if (FAILED(loadProcRes) || FAILED(saveProcRes))
      {
         throw std::invalid_argument{ "Functors not found." };
      }
   }

   qgl::guid m_guid;
   load_file_data_functor m_loadFunctor;
   save_file_data_functor m_saveFunctor;
};