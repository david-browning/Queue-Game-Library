#include "pch.h"
#include "include\Content-Files\qgl_content_variant_entry.h"

using namespace qgl::content;

content_variant_entry::content_variant_entry(CONTENT_DATA_USE_TYPES useType) :
   m_useType(useType)
{

}

content_variant_entry::content_variant_entry(const DATA_CONTENT_ENTRY& b) :
   m_useType(CONTENT_DATA_USE_TYPES::CONTENT_DATA_USE_TYPE_BUFFER)
{
   m_buffer = std::make_unique<DATA_CONTENT_ENTRY>(b);
   m_sharedBuffer = nullptr;
}

content_variant_entry::content_variant_entry(const SHARED_CONTENT_ENTRY& b) :
   m_useType(CONTENT_DATA_USE_TYPES::CONTENT_DATA_USE_TYPE_SHARED)
{
   m_sharedBuffer = std::make_unique<SHARED_CONTENT_ENTRY>(b);
   m_buffer = nullptr;
}