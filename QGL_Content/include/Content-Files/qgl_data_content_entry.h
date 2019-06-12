#pragma once
#include "include/qgl_content_include.h"

namespace qgl::content::entries
{
   /*
    The content data stored in a content file is a raw byte array.
    The byte array is not interpreted by content files. Instead, it is the
    job of a content importer to derive meaning from the bytes.
    */
   class data_content_entry
   {
      public:
      constexpr data_content_entry();

      /*
       Copies bytes from b to internal storage. After construction, b can be
       freed.
       */
      data_content_entry(const void* const b,
                         size_t bytes);


      data_content_entry(const std::vector<uint8_t>& bytes);

      /*
       Copy Constructor.
       */
      data_content_entry(const data_content_entry&) = default;

      /*
       Move Constructor.
       */
      data_content_entry(data_content_entry&&) = default;

      /*
       Destructor.
       */
      ~data_content_entry() noexcept = default;

      /*
       Returns a const pointer to the buffer. The destructor frees the buffer.
       Do not try to free this pointer.
       */
      const void* data() const noexcept;

      /*
       Returns the number of bytes in the buffer.
       */
      size_t size() const noexcept;

      friend void swap(data_content_entry& first,
                       data_content_entry& second) noexcept
      {
         using std::swap;
         swap(first.m_buffer, second.m_buffer);
      }

      /*
       Assignment Operator.
       */
      data_content_entry& operator=(data_content_entry r) noexcept
      {
         swap(*this, r);
         return *this;
      }

      /*
       Equality Operator.
       */
      friend bool operator==(const data_content_entry& r,
                             const data_content_entry& l) noexcept
      {
         return r.m_buffer == l.m_buffer;
      }

      private:
      std::vector<uint8_t> m_buffer;
   };
}