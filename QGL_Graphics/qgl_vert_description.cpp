#include "pch.h"
#include "include/Content/qgl_vert_description.h"

namespace qgl::content
{
   struct vertex_description::impl
   {
      impl(const buffers::VERTEX_DESC_HEADER* hdr_p,
           const buffers::VERTEX_ELEMENT_BUFFER* elements) :
         Header(*hdr_p)
      {
         Elements.resize(Header.Elements);
         SemanticNames.resize(Header.Elements);
         
         //Keep track of any elements trying to occupy the same index.
         std::set<size_t> seenIndices;

         for (size_t i = 0; i < Header.Elements; i++)
         {
            auto e = elements + i;
            auto position = e->Index;
            
            //Make sure the index isn't already used.
            if (seenIndices.count(position) > 0)
            {
               throw std::invalid_argument(
                  "Two elements occupy the same index.");
            }

            //This index is now occupied.
            seenIndices.insert(position);

            //Save the semantic name
            SemanticNames[position] = std::string(e->SemanticName);

            //Create the D3D12_INPUT_ELEMENT_DESC
            Elements[position] = 
            {
               SemanticNames[position].c_str(),
               e->SemanticIndex,
               static_cast<DXGI_FORMAT>(e->Format),
               e->Slot,
               D3D12_APPEND_ALIGNED_ELEMENT,
               static_cast<D3D12_INPUT_CLASSIFICATION>(e->Class),
               0
            };
         }

         //Verify there are no holes.
         for (size_t i = 0; i < Header.Elements; i++)
         {
            if (seenIndices.count(i) == 0)
            {
               throw std::invalid_argument("Not all indices are populated.");
            }
         }
      }

      impl(const impl&) = default;

      impl(impl&&) = default;

      ~impl() noexcept = default;

      std::vector<D3D12_INPUT_ELEMENT_DESC> Elements;
      std::vector<std::string> SemanticNames;

      const buffers::VERTEX_DESC_HEADER Header;
   };

   vertex_description::vertex_description(
      const buffers::VERTEX_DESC_HEADER* hdr_p,
      const buffers::VERTEX_ELEMENT_BUFFER* elements, 
      const wchar_t* name, 
      const qgl::content::content_id id) :
      m_impl(new impl(hdr_p, elements)),
      content_item(name, id,
                   RESOURCE_TYPE_DESCRIPTION,
                   CONTENT_LOADER_ID_VERTEX_DESCRIPTION)
   {

   }

   vertex_description::vertex_description(const vertex_description& r) :
      content_item(r)
   {
      delete m_impl;
      m_impl = new impl(*r.m_impl);
   }

   vertex_description::vertex_description(vertex_description&& r) :
      content_item(std::move(r))
   {
      delete m_impl;
      m_impl = r.m_impl;
      r.m_impl = nullptr;
   }

   vertex_description::~vertex_description() noexcept
   {
      delete m_impl;
   }

   size_t vertex_description::size() const noexcept
   {
      return m_impl->Elements.size();
   }

   const D3D12_INPUT_ELEMENT_DESC* vertex_description::data() const noexcept
   {
      return m_impl->Elements.data();
   }

   D3D12_INDEX_BUFFER_STRIP_CUT_VALUE vertex_description::strip_cut() const noexcept
   {
      return static_cast<D3D12_INDEX_BUFFER_STRIP_CUT_VALUE>(
         m_impl->Header.StripCut);
   }

   D3D12_PRIMITIVE_TOPOLOGY_TYPE vertex_description::topology() const noexcept
   {
      return static_cast<D3D12_PRIMITIVE_TOPOLOGY_TYPE>(
         m_impl->Header.Topology);
   }
}