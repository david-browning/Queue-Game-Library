#pragma once
#include "include/qgl_content_include.h"
#include "include/Content-Files/qgl_content_file.h"
#include "include/Content-Store/qgl_content_store_config.h"
#include "include/qgl_content_types.h"

namespace qgl::content
{
   /*
    Checks if the metadata buffer's loader ID is the expected ID.
    Throws a runtime_error if the metadata loader ID is not the expected ID.
    */
   template<CONTENT_LOADER_IDS expected>
   void check_loader_id(const CONTENT_METADATA_BUFFER& info)
   {
      if (info.loader_id() != expected)
      {
         #ifdef DEBUG
         std::wstringstream errorStream;
         errorStream << "Incorrect content loader ID. Expected " <<
            expected << ". Actual: " << info.loader_id() << std::endl;
         OutputDebugString(errorStream.str().c_str());
         #endif
         throw std::runtime_error("The content loader ID is not correct.");
      }
   }

   /*
    Checks if the metadata buffer's resource type is the expected resource
    type.
    Throws a runtime_error if the metadata resource type is not the expected
    resource type.
    */
   template<RESOURCE_TYPES expected>
   void check_resource_type(const CONTENT_METADATA_BUFFER& info)
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

   /*
    Checks if the metadata buffer's resource type and loader ID match the
    expected resource type and loader ID.
    */
   template<RESOURCE_TYPES expectedResource, CONTENT_LOADER_IDS expectedLoader>
   void check_loader_and_resource(const CONTENT_METADATA_BUFFER& info)
   {
      if (info.resource_type() != expectedResource ||
          info.loader_id() != expectedLoader)
      {
         #ifdef DEBUG
         std::wstringstream errorStream;
         errorStream << "Incorrect content loader ID or resource ID.\n"
            "Expected resource: " << expectedResource << ". Actual: " <<
            info.resource_type() << "\nExpected loader id: " <<
            expectedLoader << ". Actual: " << info.loader_id() << std::endl;
         OutputDebugString(errorStream.str().c_str());
         #endif
         throw std::runtime_error(
            "The resource type or loader ID is not correct.");
      }
   }
}