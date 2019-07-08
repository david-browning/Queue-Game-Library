#include "pch.h"
#include "include\Interfaces\qgl_ifile_buffer.h"
#include "include/qgl_file_handle.h"
#include "include/qgl_file_helpers.h"

namespace qgl::content
{
   class file_buffer_1_0 : public ifile_buffer
   {
      public:
      HRESULT make(const wchar_t* absPath)
      {
         auto hr = open_file_read(absPath, &m_hndl);
         if (FAILED(hr))
         {
            return hr;
         }

         size_t sz = 0;
         hr = file_size(&m_hndl, &sz);
         if (FAILED(hr))
         {
            return hr;
         }

         m_buffer.resize(sz);
         return read_file_sync(&m_hndl, sz, 0, m_buffer.data());
      }

      virtual void release()
      {
         //Calls the destructor.
         delete this;
      }

      virtual const void* const_data() const noexcept
      {
         return m_buffer.data();
      }

      virtual size_t size() const noexcept
      {
         return m_buffer.size();
      }

      virtual const file_handle* handle() const noexcept
      {
         return &m_hndl;
      }

      private:
      file_handle m_hndl;
      std::vector<uint8_t> m_buffer;
   };


   HRESULT qgl_open_file_buffer(const wchar_t* filePath,
                                qgl::qgl_version_t v,
                                ifile_buffer** out_p) noexcept
   {
      if (out_p == nullptr)
      {
         #ifdef DEBUG
         OutputDebugString(L"out_p cannot be nullptr.");
         #endif
         return E_INVALIDARG;
      }

      ifile_buffer* ret = nullptr;
      switch (std::hash<qgl_version_t>{}(v))
      {
         case qgl::hashes::VERSION_0_1_HASH:
         case qgl::hashes::VERSION_0_2_HASH:
         {
            ret = new(std::nothrow) file_buffer_1_0();
            if (ret == nullptr)
            {
               #ifdef DEBUG
               OutputDebugString(L"Out of memory!");
               #endif
               return E_OUTOFMEMORY;
            }

            auto makeable = dynamic_cast<file_buffer_1_0*>(ret);
            auto hr = makeable->make(filePath);
            if (FAILED(hr))
            {
               return hr;
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
}