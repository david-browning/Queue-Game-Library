#include <pch.h>
#include "include\Content-Files\qgl_content_file_helpers.h"

using namespace qgl::content;

CONTENT_FILE_HEADER_BUFFER qgl::content::load_header(
   const winrt::file_handle& hndl)
{
}

content_dictionary qgl::content::load_dictionary(
   const winrt::file_handle& hndl,
   size_t dictionaryOffset)
{
}

std::vector<uint8_t> qgl::content::load_content_data(
   const winrt::file_handle& hndl,
   const CONTENT_DICTIONARY_ENTRY_BUFFER& entry)
{
}

void qgl::content::write_dictionary(
   const winrt::file_handle& hndl,
   size_t dictionaryOffset,
   const content_dictionary& dict)
{
}

void qgl::content::write_dictionary_entry_data(
   const winrt::file_handle& hndl,
   CONTENT_DICTIONARY_ENTRY_BUFFER& entry,
   const void* contentData)
{
}

size_t qgl::content::dictionary_data_offset(
   const CONTENT_FILE_HEADER_BUFFER& fileHeader,
   const CONTENT_DICTIONARY_METADATA_BUFFER& dictMeta)
{
}
