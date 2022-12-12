#pragma once
#include "include/qgl_graphics_include.h"
#include "include/Shaders/qgl_vertex_layout_descriptor.h"
#include "include/Stagers/qgl_vert_stager.h"

namespace qgl::graphics::shaders
{
   /*
    Describes the vertex layout for the input assembler.
    */
   class vertex_layout final
   {
      public:
      /*
       Constructs a vertex_layout using a collection of vertex elements.
       The topology and strip cut must also be provided.
       */
      vertex_layout(const stagers::vert_stager& stager,
                    topology_t topo,
                    strip_cut_t stripCut) :
         m_stager(stager),
         m_topo(to_d3d_topo(topo)),
         m_strip_cut(to_d3d_strip_cut(stripCut))
      {
        
      }

      /*
       Constructs the vertex layout using the layout descriptor.
       */
      vertex_layout(const vertex_layout_descriptor& desc) :
         vertex_layout(stagers::make_vert_stager(desc),
                       desc.topology,
                       desc.strip_cut)
      {

      }

      /*
       Constructs a vertex_layout using the shader metadata.
       Not all necessary parameters are stored in the shader metadata so they
       must be specified as well.
       The input slot and classification are applied to all elements in the
       vertex layout.
       */
      vertex_layout(const shader_metadata& m,
                    topology_t topo,
                    strip_cut_t stripCut,
                    input_slot_t inputSlot,
                    input_data_class_t slotClass)
      {
         if (m.type() == shader_types::unknown)
         {
            throw std::invalid_argument(
               "Shader type is unknown. "
               "Libraries cannot be used to create a vertex layout.");
         }

         m_topo = to_d3d_topo(topo);
         m_strip_cut = to_d3d_strip_cut(stripCut);

         for (auto inIt = m.cbegin_inputs(); inIt != m.cend_inputs(); inIt++)
         {
            D3D12_INPUT_ELEMENT_DESC desc;
            desc.SemanticName = inIt->semantic_name.c_str();
            desc.SemanticIndex = static_cast<UINT>(inIt->semantic_idx);
            desc.AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
            desc.Format = vformat(inIt->component_type, inIt->component_mask);
            desc.InputSlot = to_d3d_input_slot(inputSlot);
            desc.InputSlotClass = to_d3d_input_class(slotClass);
            desc.InstanceDataStepRate = 0;

            m_stager.push_back(desc);
         }
      }

      vertex_layout(const vertex_layout& r) = default;

      vertex_layout(vertex_layout&&) noexcept = default;

      ~vertex_layout() noexcept = default;

      /*
       Returns the number of input element descriptions.
       */
      size_t size() const noexcept
      {
         return m_stager.size();
      }

      /*
       Returns a pointer to the array of element descriptions.
       */
      const D3D12_INPUT_ELEMENT_DESC* data() const noexcept
      {
         return m_stager.data();
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
      stagers::vert_stager m_stager;
      D3D12_PRIMITIVE_TOPOLOGY_TYPE m_topo;
      D3D12_INDEX_BUFFER_STRIP_CUT_VALUE m_strip_cut;
   };
}