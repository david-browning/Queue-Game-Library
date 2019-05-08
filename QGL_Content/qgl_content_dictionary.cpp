#include "pch.h"
#include "include/qgl_content_dictionary.h"

qgl::content::content_dictionary::content_dictionary() :
   m_buffer(0, sizeof(CONTENT_DICTIONARY_ENTRY_BUFFER), 0),
   m_entries(0)
{
}

qgl::content::content_dictionary::content_dictionary(const CONTENT_DICTIONARY_BUFFER& buf) :
   m_buffer(buf),
   m_entries(buf.count())
{
}

qgl::content::content_dictionary::content_dictionary(const content_dictionary& r) :
   m_buffer(r.m_buffer),
   m_entries(r.m_entries)
{
}

qgl::content::content_dictionary::content_dictionary(content_dictionary&& r) :
   m_buffer(std::move(r.m_buffer)),
   m_entries(std::move(r.m_entries))
{
}

qgl::content::content_dictionary::~content_dictionary()
{
}
