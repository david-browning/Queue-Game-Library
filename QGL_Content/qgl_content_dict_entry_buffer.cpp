#include "pch.h"
#include "include/qgl_content_dict_entry_buffer.h"

qgl::content::CONTENT_DICTIONARY_ENTRY_BUFFER::CONTENT_DICTIONARY_ENTRY_BUFFER() :
   m_offset(0),
   m_flags(0),
   m_size(0),
   m_info()
{
}

qgl::content::CONTENT_DICTIONARY_ENTRY_BUFFER::CONTENT_DICTIONARY_ENTRY_BUFFER(size_t entrySize, 
                                                                               CONTENT_INFO_BUFFER& entryInfo, 
                                                                               size_t entryOffset) :
   m_offset(entryOffset),
   m_flags(0),
   m_size(entrySize),
   m_info(entryInfo)
{

}


qgl::content::CONTENT_DICTIONARY_ENTRY_BUFFER::CONTENT_DICTIONARY_ENTRY_BUFFER(const CONTENT_DICTIONARY_ENTRY_BUFFER& r) :
   m_offset(r.m_offset),
   m_flags(r.m_flags),
   m_size(r.m_size),
   m_info(r.m_info)
{
}

qgl::content::CONTENT_DICTIONARY_ENTRY_BUFFER::CONTENT_DICTIONARY_ENTRY_BUFFER(CONTENT_DICTIONARY_ENTRY_BUFFER && r) :
   m_offset(r.m_offset),
   m_flags(r.m_flags),
   m_size(r.m_size),
   m_info(std::move(r.m_info))
{
}
