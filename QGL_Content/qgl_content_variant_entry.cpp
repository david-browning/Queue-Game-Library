#include "pch.h"
#include "include\Content-Files\qgl_content_variant_entry.h"

namespace qgl::content
{

   qgl::content::content_variant_entry::content_variant_entry() :
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

   content_variant_entry::content_variant_entry(const DATA_CONTENT_ENTRY* b) :
      m_useType(CONTENT_DATA_USE_TYPES::CONTENT_DATA_USE_TYPE_BUFFER),
      m_loaded(false)
   {
      m_buffer = new DATA_CONTENT_ENTRY(*b);
      m_sharedBuffer = nullptr;
   }

   content_variant_entry::content_variant_entry(const SHARED_CONTENT_ENTRY* b) :
      m_useType(CONTENT_DATA_USE_TYPES::CONTENT_DATA_USE_TYPE_SHARED),
      m_loaded(false)
   {
      m_sharedBuffer = new SHARED_CONTENT_ENTRY(*b);
      m_buffer = nullptr;
   }

   qgl::content::content_variant_entry::content_variant_entry(
      const content_variant_entry& r) :
      m_useType(r.m_useType),
      m_loaded(r.m_loaded)
   {
      if (m_useType == CONTENT_DATA_USE_TYPES::CONTENT_DATA_USE_TYPE_BUFFER)
      {
         m_buffer = new DATA_CONTENT_ENTRY(*r.m_buffer);
         m_sharedBuffer = nullptr;
      }
      else
      {
         m_buffer = nullptr;
         m_sharedBuffer = new SHARED_CONTENT_ENTRY(*r.m_sharedBuffer);
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