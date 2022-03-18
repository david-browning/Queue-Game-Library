#include "pch.h"
#include "include\Content-Files\qgl_content_variant_entry.h"

namespace qgl::content::entries
{

   content_variant_entry::content_variant_entry() :
      m_useType(CONTENT_DATA_USE_TYPES::CONTENT_DATA_USE_TYPE_UNKNOWN),
      m_buffer(nullptr),
      m_sharedBuffer(nullptr),
      m_loaded(false)
   {
   }

   content_variant_entry::content_variant_entry(CONTENT_DATA_USE_TYPES useType) :
      m_useType(useType),
      m_buffer(nullptr),
      m_sharedBuffer(nullptr),
      m_loaded(false)
   {

   }

   content_variant_entry::content_variant_entry(const void* buff,
                                                size_t numBytes) :
      m_useType(CONTENT_DATA_USE_TYPES::CONTENT_DATA_USE_TYPE_BUFFER),
      m_loaded(false)
   {
      m_buffer = new entries::data_content_entry(buff, numBytes);
      m_sharedBuffer = nullptr;
   }

   content_variant_entry::content_variant_entry(const wchar_t* sharedPath) :
      m_useType(CONTENT_DATA_USE_TYPES::CONTENT_DATA_USE_TYPE_SHARED),
      m_loaded(false)
   {
      m_sharedBuffer = new entries::shared_content_entry(sharedPath);
      m_buffer = nullptr;
   }

   content_variant_entry::content_variant_entry(
      const content_variant_entry& r) :
      m_useType(r.m_useType),
      m_loaded(r.m_loaded)
   {
      if (m_useType == CONTENT_DATA_USE_TYPES::CONTENT_DATA_USE_TYPE_BUFFER)
      {
         m_buffer = new entries::data_content_entry(*r.m_buffer);
         m_sharedBuffer = nullptr;
      }
      else
      {
         m_buffer = nullptr;
         m_sharedBuffer = new entries::shared_content_entry(*r.m_sharedBuffer);
      }
   }

   content_variant_entry::content_variant_entry(
      content_variant_entry&& r) :
      m_useType(std::move(r.m_useType)),
      m_loaded(std::move(r.m_loaded))
   {
      m_sharedBuffer = r.m_sharedBuffer;
      m_buffer = r.m_buffer;
      r.m_buffer = nullptr;
      r.m_sharedBuffer = nullptr;
   }

   content_variant_entry::~content_variant_entry() noexcept
   {
      delete m_buffer;
      delete m_sharedBuffer;

      m_sharedBuffer = nullptr;
      m_buffer = nullptr;
   }
}