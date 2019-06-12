#include "pch.h"
#include "include\Interfaces\qgl_ifile_buffer.h"
#include "include/qgl_file_handle.h"
#include "include/qgl_file_helpers.h"

namespace qgl::content
{
   extern "C"
   {
      struct file_buffer_1_0 : public ifile_buffer
      {
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

         virtual const void* QGL_API_CC const_data() const noexcept
         {
            return m_buffer.data();
         }

         virtual size_t QGL_API_CC size() const noexcept
         {
            return m_buffer.size();
         }

         virtual const file_handle* QGL_API_CC handle() const noexcept
         {
            return &m_hndl;
         }

         private:
         file_handle m_hndl;
         std::vector<uint8_t> m_buffer;
      };
   }

   HRESULT qgl_open_file_buffer(const wchar_t* filePath, 
                                qgl::qgl_version_t v,
                                ifile_buffer** out_p) noexcept
   {
      if (out_p == nullptr)
      {
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
            return E_NOINTERFACE;
         }
      }

      *out_p = ret;
      return S_OK;
   }
}