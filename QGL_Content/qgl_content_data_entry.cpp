#include "pch.h"
#include "include\Content-Files\qgl_content_data_entry.h"

using namespace qgl::content;

content_data_entry::content_data_entry(CONTENT_DATA_USE_TYPES useType) :
   m_useType(useType),
   m_loaded(false)
{

}

content_data_entry::content_data_entry(const content_data_buffer_t& b) :
   m_buffer(b),
   m_useType(CONTENT_DATA_USE_TYPES::CONTENT_DATA_USE_TYPE_BUFFER),
   m_loaded(true)
{

}

content_data_entry::content_data_entry(const shared_content_data_buffer_t& b) :
   m_sharedBuffer(b),
   m_useType(CONTENT_DATA_USE_TYPES::CONTENT_DATA_USE_TYPE_SHARED),
   m_loaded(true)
{

}