#pragma once
#include "qgl_content_include.h"
#include "qgl_content_file.h"
#include "qgl_content_header_buffer.h"
#include "qgl_content_store_config.h"

namespace qgl::content
{
   template<CONTENT_LOADER_IDS expected>
   inline void check_loader_id(const CONTENT_METADATA_BUFFER& info)
   {
      if (info.loader_id() != expected)
      {
         #ifdef DEBUG
         std::wstringstream errorStream;
         errorStream << "Incorrect content loader ID. Expected " << expected <<
            ". Actual: " << info.loader_id() << std::endl;
         OutputDebugString(errorStream.str().c_str());
         #endif
         throw std::runtime_error("The content loader ID is not correct.");
      }
   }

   template<RESOURCE_TYPES expected>
   inline void check_resource_type(const CONTENT_METADATA_BUFFER& info)
   {
      if (info.resource_type() != expected)
      {
         #ifdef DEBUG
         std::wstringstream errorStream;
         errorStream << "Incorrect resource type. Expected " << expected <<
            ". Actual: " << info.resource_type() << std::endl;
         OutputDebugString(errorStream.str().c_str());
         #endif
         throw std::runtime_error("The resource type is not correct.");
      }
   }

   template<RESOURCE_TYPES expectedResource, CONTENT_LOADER_IDS expectedLoader>
   inline void check_loader_and_resource(const CONTENT_METADATA_BUFFER& info)
   {
      if (info.resource_type() != expectedResource ||
          info.loader_id() != expectedLoader)
      {
         #ifdef DEBUG
         std::wstringstream errorStream;
         errorStream << "Incorrect content loader ID or resource ID.\n"
            "Expected resource: " << expectedResource << ". Actual: " << info.resource_type() << "\n"
            "Expected loader id: " << expectedLoader << ". Actual: " << info.loader_id() << std::endl;
         OutputDebugString(errorStream.str().c_str());
         #endif
         throw std::runtime_error("The resource type or loader ID is not correct.");
      }
   }

   struct LIB_EXPORT wstring_file_loader
   {
      std::shared_ptr<std::wstring> operator()(const content_file& f);
   };

   struct LIB_EXPORT string_file_loader
   {
      std::shared_ptr<std::string> operator()(const content_file& f);
   };

   struct LIB_EXPORT content_store_config_loader
   {
      std::shared_ptr<content_store_config> operator()(const content_file& f);
   };
}