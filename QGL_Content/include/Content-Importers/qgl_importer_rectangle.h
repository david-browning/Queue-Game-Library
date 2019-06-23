#pragma once
#include "include/qgl_content_include.h"
#include "include/Content-Importers/qgl_iimporter.h"
#include "include/qgl_file_helpers.h"
#include "include/Content-Buffers/qgl_rectangle_buffer.h"
#include "include/Content-Importers/qgl_importer_struct.h"

namespace qgl::content::loaders
{
   using rectangle_buffer_importer = struct_importer<
      buffers::RECTANGLE_BUFFER,
      RESOURCE_TYPE_SHAPE,
      CONTENT_LOADER_ID_STRUCT>;

   class rectangle_load_entry_functor
   {
      public:
      qgl::math::rectangle operator()(
         const file_handle& fileHandle,
         const CONTENT_DICTIONARY_ENTRY_BUFFER& lookup)
      {
         if (lookup.size() != sizeof(buffers::RECTANGLE_BUFFER))
         {
            throw std::invalid_argument("The lookup size is not expected.");
         }

         static rectangle_buffer_importer importer;

         //Read the buffer in.
         auto buff = importer.load(fileHandle,
                                   lookup);

         //Create rectangle.
         return qgl::math::rectangle(
            DirectX::XMVectorSet(static_cast<float>(buff.TopLeftX),
                                 static_cast<float>(buff.TopLeftY),
                                 static_cast<float>(buff.TopLeftZ),
                                 0.0f),
            DirectX::XMVectorSet(static_cast<float>(buff.BottomRightX),
                                 static_cast<float>(buff.BottomRightY),
                                 static_cast<float>(buff.BottomRightZ),
                                 0.0f));
      }
   };

   class rectangle_dict_export_functor
   {
      public:
      CONTENT_DICTIONARY_ENTRY_BUFFER operator()(
         [[maybe_unused]] const qgl::math::rectangle& data,
         const wchar_t* name,
         size_t offset)
      {
         static buffers::RECTANGLE_BUFFER m_buffer;
         static rectangle_buffer_importer m_importer;

         return m_importer.dict_entry(m_buffer,
                                      name,
                                      offset);
      }
   };
}