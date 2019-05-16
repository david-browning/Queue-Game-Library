#include "pch.h"
#include "include/Content-Dictionary/qgl_content_dictionary.h"

qgl::content::content_dictionary::content_dictionary()
{
}

qgl::content::content_dictionary::content_dictionary(
   const CONTENT_DICTIONARY_METADATA_BUFFER& buf) :
   m_entries(buf.count())
{
}

void qgl::content::content_dictionary::push_back(
   const CONTENT_DICTIONARY_ENTRY_BUFFER & entry)
{
   m_entries.push_back(entry);
}

void qgl::content::content_dictionary::push_back(
   CONTENT_DICTIONARY_ENTRY_BUFFER && entry)
{
   m_entries.push_back(entry);
}
