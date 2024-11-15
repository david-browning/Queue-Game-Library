#include "pch.h"
#include "include/Loaders/qgl_content_loader_provider.h"
#include "src/Loaders/qgl_struct_loader_metadata.h"
#include "src/Loaders/qgl_string_loader_metadata.h"
#include "src/Loaders/qgl_wstring_loader_metadata.h"

using namespace qgl;

/*
 Version 1.0 loader provider. Provides the loaders for the QGL_Content module.
 */
class loader_provider_1_0 : public content::iloader_provider
{
   public:
   loader_provider_1_0() :
      m_metadatas(3)
   {
      m_metadatas[0] = qgl::make_unique<content::iloader_metadata>(
         new content::struct_loader_1_0());
      m_metadatas[1] = qgl::make_unique<content::iloader_metadata>(
         new content::string_loader_1_0());
      m_metadatas[2] = qgl::make_unique<content::iloader_metadata>(
         new content::wstring_loader_1_0());

      // TODO: Add more loaders
   }

   virtual ~loader_provider_1_0() noexcept = default;

   virtual void release() noexcept
   {
      delete this;
   }

   virtual iqgl* duplicate() const noexcept
   {
      return new (std::nothrow)loader_provider_1_0();
   }

   virtual uint32_t size() const noexcept
   {
      return static_cast<uint32_t>(m_metadatas.size());
   }

   virtual result_t metadata(uint32_t idx,
                             content::iloader_metadata** out_p) const noexcept
   {
      if (!out_p || idx > size() - 1)
      {
         return E_INVALIDARG;
      }

      *out_p = m_metadatas[idx].get();
      return S_OK;
   }

   private:
   std::vector<qgl_unique_ptr<content::iloader_metadata>> m_metadatas;
};

result_t QGL_CC qgl::qgl_get_loaders(qgl_version v,
                                     content::iloader_provider** out_p) noexcept
{
   if (!out_p)
   {
      return E_INVALIDARG;
   }

   content::iloader_provider* ret = nullptr;
   switch (std::hash<qgl_version>{}(v))
   {
      case qgl::hashes::VERSION_0_1_HASH:
      case qgl::hashes::VERSION_0_2_HASH:
      {
         ret = new(std::nothrow)loader_provider_1_0();
         if (ret == nullptr)
         {
#ifdef DEBUG
            OutputDebugString(L"Out of memory!");
#endif
            return E_OUTOFMEMORY;
         }
         break;
      }
      default:
      {
#ifdef DEBUG
         OutputDebugString(L"This QGL version is not supported.");
#endif
         return E_NOINTERFACE;
      }
   }

   *out_p = ret;
   return S_OK;
}