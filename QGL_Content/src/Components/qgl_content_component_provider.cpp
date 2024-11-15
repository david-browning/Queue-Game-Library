#include "pch.h"
#include "include/Components/qgl_content_component_provider.h"

using namespace qgl;

/*
 Version 1.0 component provider. Provides the components for the QGL_Content
 module.
 */
class component_provider_1_0 : public components::icomponent_provider
{
   public:
   component_provider_1_0()
   {
      // TODO: Add component metadatas to m_metadatas.
   }

   virtual ~component_provider_1_0() noexcept = default;

   virtual void release() noexcept
   {
      delete this;
   }

   virtual iqgl* duplicate() const noexcept
   {
      return new (std::nothrow)component_provider_1_0();
   }

   virtual uint32_t size() const noexcept
   {
      return static_cast<uint32_t>(m_metadatas.size());
   }

   virtual result_t metadata(
      uint32_t idx, components::icomponent_metadata** out_p) const noexcept
   {
      if (!out_p || idx > size() - 1)
      {
         return E_INVALIDARG;
      }

      *out_p = static_cast<components::icomponent_metadata*>(m_metadatas[idx].duplicate());
      return S_OK;
   }

   private:
   std::vector<components::icomponent_metadata> m_metadatas;
};

result_t QGL_CC qgl::qgl_get_components(
   qgl_version v, qgl::components::icomponent_provider** out_p) noexcept
{
   if (!out_p)
   {
      return E_INVALIDARG;
   }

   components::icomponent_provider* ret = nullptr;
   switch (std::hash<qgl_version>{}(v))
   {
      case qgl::hashes::VERSION_0_1_HASH:
      case qgl::hashes::VERSION_0_2_HASH:
      {
         ret = new(std::nothrow)component_provider_1_0();
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