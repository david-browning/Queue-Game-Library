#include "pch.h"
#include "include/Interfaces/qgl_module.h"

namespace qgl
{
   struct hmodule_traits
   {
      using type = HMODULE;

      static void close(type value) noexcept
      {
         winrt::check_bool(FreeLibrary(value));
      }

      static constexpr type invalid() noexcept
      {
         return nullptr;
      }
   };

   class module_v_1_0 : public imodule
   {
      public:
      module_v_1_0(const sys_str& path) :
         m_modPath(path)
      {
         construct();
      }

      module_v_1_0(sys_str&& path) :
         m_modPath(std::forward<sys_str>(path))
      {
         construct();
      }

      module_v_1_0(const module_v_1_0&) = default;

      module_v_1_0(module_v_1_0&&) noexcept = default;

      virtual ~module_v_1_0() noexcept = default;

      virtual void release() noexcept
      {
         //Calls the destructor.
         delete this;
      }

      virtual iqgl* duplicate() const noexcept
      {
         return new (std::nothrow)module_v_1_0(m_modPath);
      }

      virtual result_t address(
         const char* procName, uint64_t* out_p) const noexcept
      {
         auto proc = GetProcAddress(m_modHandle.get(), procName);
         if (!proc)
         {
            return E_INVALIDARG;
         }
         
         *out_p = reinterpret_cast<uint64_t>(proc);
         return S_OK;
      }

      private:
      void construct()
      {
         // https://learn.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-loadpackagedlibrary
         m_modHandle.attach(LoadPackagedLibrary(m_modPath.c_str(), 0));
         if (!m_modHandle)
         {
            winrt::throw_last_error();
         }
      }

      sys_str m_modPath;
      winrt::handle_type<hmodule_traits> m_modHandle;
   };

   result_t QGL_CC make_module(const sys_char* const path,
                               qgl_version v,
                               imodule** h_p) noexcept
   {
      if (!h_p)
      {
#ifdef DEBUG
         OutputDebugString(L"h_p cannot be nullptr.");
#endif
         return E_INVALIDARG;
      }

      imodule* ret = nullptr;
      switch (std::hash<qgl_version>{}(v))
      {
         case qgl::hashes::VERSION_0_1_HASH:
         case qgl::hashes::VERSION_0_2_HASH:
         {
            ret = new(std::nothrow) module_v_1_0(sys_str{ path });
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

      *h_p = ret;
      return S_OK;
   }
}