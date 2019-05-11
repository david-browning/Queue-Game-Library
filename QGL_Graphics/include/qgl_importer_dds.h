#pragma once
#include "qgl_graphics_include.h"
#include "qgl_texture_buffer.h"
#include "qgl_dds.h"
#include <QGLContent.h>

namespace qgl::content
{
   using namespace qgl::graphics;

   template<typename ContentCharT = char, typename IDT = content_id>
   class dds_importer : public ientry_importer<ContentCharT, IDT, TEXTURE_BUFFER>
   {
      public:
      using LoadT = TEXTURE_BUFFER;

      constexpr dds_importer(const winrt::com_ptr<d3d_device>& dev_p) :
         m_dev_p(dev_p),
         ientry_importer(CONTENT_LOADER_IDS::CONTENT_LOADER_ID_DDS)
      {

      }

      virtual ~dds_importer() noexcept
      {

      }

      virtual LoadT load(_IN_HANDLE_ const winrt::file_handle& fileHandle,
                           _IN_ const CONTENT_DICTIONARY_ENTRY_BUFFER& lookup,
                           const IDT& newID = -1) const
      {
         qgl::graphics::TEXTURE_BUFFER ret(lookup.size());

         //Read the texture data into the texture buffer.
         read_file_sync<uint8_t>(fileHandle, ret.size(), lookup.offset(), ret.data());


         //Use the dds texture loader to create the texture buffer from the raw data.
         qgl::graphics::low::load_texture_data(m_dev_p,
                                               &ret);

         return ret;
      }

      virtual const CONTENT_DICTIONARY_ENTRY_BUFFER dict_entry(const LoadT& data,
                                                               const std::string& objName,
                                                               size_t offset = -1) const
      {
         static constexpr RESOURCE_TYPES rType = RESOURCE_TYPES::RESOURCE_TYPE_TEXTURE;
         static CONTENT_METADATA_BUFFER info(rType, this->id());
         return CONTENT_DICTIONARY_ENTRY_BUFFER(data.size(), info, objName, offset);
      }

      private:
      winrt::com_ptr<d3d_device> m_dev_p;
   };
}