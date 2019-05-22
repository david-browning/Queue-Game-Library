#pragma once
#include "include/qgl_content_include.h"

namespace qgl::content
{
   using content_data_buffer_t = std::vector<uint8_t>;
   using shared_content_data_buffer_t = std::wstring;

   enum class CONTENT_DATA_USE_TYPES
   {
      CONTENT_DATA_USE_TYPE_BUFFER,
      CONTENT_DATA_USE_TYPE_SHARED,
   };

   class LIB_EXPORT content_data_entry
   {
      public:
      content_data_entry(CONTENT_DATA_USE_TYPES useType);

      content_data_entry(const content_data_buffer_t& b);

      content_data_entry(const shared_content_data_buffer_t& b);

      content_data_entry(const content_data_entry&) = default;

      content_data_entry(content_data_entry&&) = default;

      ~content_data_entry() noexcept = default;

      bool shared() const noexcept
      {
         return m_useType == 
            CONTENT_DATA_USE_TYPES::CONTENT_DATA_USE_TYPE_SHARED;
      }

      bool loaded() const noexcept
      {
         return m_loaded;
      }

      const auto& buffer() const noexcept
      {
         return m_buffer;
      }

      const auto& shared_buffer() const noexcept
      {
         return m_sharedBuffer;
      }

      private:
      CONTENT_DATA_USE_TYPES m_useType;
      content_data_buffer_t m_buffer;
      shared_content_data_buffer_t m_sharedBuffer;
      bool m_loaded;
   };
}