#include "pch.h"
//#include "include/qgl_content_project_compiler.h"
//#include "include/qgl_file_helpers.h"
//#include "include/qgl_content_dict_entry_buffer.h"
//#include "include/qgl_content_dictionary_buffer.h"
//
//void qgl::content::compile_project(content_project& proj,
//                                   const winrt::hstring& absPath)
//{
//   CREATEFILE2_EXTENDED_PARAMETERS openParameters = { 0 };
//   openParameters.dwSize = sizeof(CREATEFILE2_EXTENDED_PARAMETERS);
//   openParameters.dwFileAttributes = FILE_ATTRIBUTE_NORMAL;
//   openParameters.dwFileFlags = FILE_FLAG_OVERLAPPED | FILE_FLAG_SEQUENTIAL_SCAN;
//   openParameters.dwSecurityQosFlags = SECURITY_ANONYMOUS;
//   openParameters.hTemplateFile = nullptr;
//   SECURITY_ATTRIBUTES sa;
//   sa.bInheritHandle = TRUE;
//   sa.nLength = sizeof(sa);
//   sa.lpSecurityDescriptor = nullptr;
//   openParameters.lpSecurityAttributes = &sa;
//
//   auto hndl = open_file_write(absPath, openParameters);
//
//   size_t offset = 0;
//   auto& header = proj.header();
//   const auto headerSize = sizeof(header);
//
//   //Write the header.
//   write_file_sync(hndl, headerSize, offset, &header);
//   offset += headerSize;
//
//   //Write the dictionary header
//   static constexpr auto dictEntrySize = sizeof(CONTENT_DICTIONARY_ENTRY_BUFFER);
//   static constexpr auto dictHeaderSize = sizeof(CONTENT_DICTIONARY_METADATA_BUFFER);
//   CONTENT_DICTIONARY_METADATA_BUFFER dictHeader(proj.count(), dictEntrySize, 0);
//   write_file_sync(hndl, dictHeaderSize, offset, &dictHeaderSize);
//   offset += dictHeaderSize;
//
//   auto dictEntryOffset = offset;
//   auto contentOffset = dictEntryOffset + proj.count() * dictEntrySize;
//
//   //Write each entry.
//   for (auto& entry : proj)
//   {
//      //Get the entry that will be written in the dictionary.
//      auto& dictEntry = entry.first;
//
//      //Update the entry's data offset.
//      dictEntry.offset(contentOffset);
//
//      //Now that the dictionary entry is up to date, write it to the dictionary.
//      write_file_sync(hndl, dictEntrySize, offset, &entry.first);
//      dictEntryOffset += dictEntrySize;
//
//      //Open the buffer file and read it into memory.
//      auto buffHndl = open_file_read(entry.second, openParameters);
//      auto dataBuffer = file_data(buffHndl);
//      buffHndl.close();
//
//      //Write the content the dictionary entry points to.
//      write_file_sync(hndl, dictEntry.size(), contentOffset, dataBuffer.data());
//      contentOffset += dictEntry.size();
//   }
//
//   hndl.close();
//}
