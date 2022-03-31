#pragma once
#include "include/qgl_graphics_include.h"
#include "include/Content/Content-Descriptors/qgl_vertex_layout_descriptor.h"
#include "include/Content/Content-Descriptors/qgl_vertex_element_descriptor.h"

namespace qgl::graphics
{
   /*
    Describes the vertex layout for the input assembler.
    */
   class vertex_layout
   {
      public:
      template<class ElementIt>
      vertex_layout(ElementIt first, 
                    ElementIt last,
                    const descriptors::vertex_layout_descriptor& desc) :
         m_topo(static_cast<D3D12_PRIMITIVE_TOPOLOGY_TYPE>(desc.topology)),
         m_strip_cut(static_cast<D3D12_INDEX_BUFFER_STRIP_CUT_VALUE>(desc.strip_cut))

      {
         auto count = std::distance(first, last);

         using idx_type = decltype(descriptors::vertex_element_descriptor::index);

         // Keep track of the indicies so that no element can occupie the same
         // index.
         std::unordered_set<idx_type> seen;
         for (auto it = first; it != last; it++)
         {
            //Make sure the index isn't already used.
            if (seen.count(it->index) > 0)
            {
               throw std::invalid_argument(
                  "Two elements occupy the same index.");
            }

            // Mark the index as occupied
            seen.insert(it->index);

            // Copy the sematic name.
            m_sematicNames.emplace_back(it->semantic_index);

            // Create a D3D12_INPUT_ELEMENT_DESC from the current element.
            D3D12_INPUT_ELEMENT_DESC e;
            e.SemanticName = m_sematicNames.back().c_str();
            e.SemanticIndex = first->index;
            e.Format = static_cast<DXGI_FORMAT>(first->format);
            e.InputSlot = first->slot;
            e.AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
            e.InputSlotClass =
               static_cast<D3D12_INPUT_CLASSIFICATION>(first->slot);
            e.InstanceDataStepRate = first->data_class;

            m_elements.push_back(std::move(e));
         }

         //Verify there are no holes.
         for (size_t i = 0; i < count; i++)
         {
            if (seen.count(i) == 0)
            {
               throw std::invalid_argument("Not all indices are populated.");
            }
         }
      }

      vertex_layout(const vertex_layout&) = default;

      vertex_layout(vertex_layout&&) = default;

      ~vertex_layout() noexcept = default;

      /*
       Returns the number of input element descriptions.
       */
      constexpr size_t size() const noexcept
      {
         return m_elements.size();
      }

      /*
       Returns a pointer to the array of element descriptions.
       */
      const D3D12_INPUT_ELEMENT_DESC* data() const noexcept
      {
         return m_elements.data();
      }

      /*
       When using triangle strip primitive topology, vertex positions are
       interpreted as vertices of a continuous triangle “strip”. There is a
       special index value that represents the desire to have a discontinuity
       in the strip, the cut index value. This enum lists the supported cut
       values.
       */
      D3D12_INDEX_BUFFER_STRIP_CUT_VALUE strip_cut() const noexcept
      {
         return m_strip_cut;
      }

      /*
       Specifies how the pipeline interprets geometry or hull shader input
       primitives.
       */
      D3D12_PRIMITIVE_TOPOLOGY_TYPE topology() const noexcept
      {
         return m_topo;
      }

      private:
      D3D12_PRIMITIVE_TOPOLOGY_TYPE m_topo;
      D3D12_INDEX_BUFFER_STRIP_CUT_VALUE m_strip_cut;
      std::vector<D3D12_INPUT_ELEMENT_DESC> m_elements;
      std::vector<std::string> m_sematicNames;

   };
}