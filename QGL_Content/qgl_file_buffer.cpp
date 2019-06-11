#include "pch.h"
#include "include/qgl_file_buffer.h"
#include "include/qgl_file_handle.h"
#include "include/qgl_file_helpers.h"

namespace qgl::content
{
   struct file_buffer::impl
   {
      impl(const file_handle* h)
      {
         size_t sz = 0;
         auto hr = file_size(h, &sz);
         winrt::check_hresult(hr);

         Data.resize(sz);
         hr = read_file_sync(h, sz, 0, Data.data());
         winrt::check_hresult(hr);
      }

      impl(const impl&) = default;

      impl(impl&&) = default;

      ~impl() noexcept = default;

      std::vector<uint8_t> Data;
   };

   file_buffer::file_buffer(const file_handle* h)
   {
      m_impl_p = new impl(h);
   }

   file_buffer::file_buffer(file_buffer&& r)
   {
      delete m_impl_p;
      m_impl_p = r.m_impl_p;
      r.m_impl_p = nullptr;
   }

   file_buffer::~file_buffer() noexcept
   {
      delete m_impl_p;
      #ifdef DEBUG
      m_impl_p = nullptr;
      #endif;
   }

   const void* file_buffer::data() const noexcept
   {
      return m_impl_p->Data.data();
   }

   size_t file_buffer::size() const noexcept
   {
      return m_impl_p->Data.size();
   }
}