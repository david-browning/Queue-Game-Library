#pragma once
#include "include/qgl_graphics_include.h"
#include "include/Descriptors/qgl_vertex_layout_descriptor.h"
#include "include/Helpers/qgl_vert_stager.h"

namespace qgl::components
{
   static constexpr guid VERTEXLAYOUT_GUID{ "38FD0DB11662469BAAD1FA310842F1A3" };

   /*
    Describes the vertex layout for the input assembler.
    */
   class vertex_layout : public game_component<vertex_layout>
   {
      public:
      vertex_layout(const graphics::vert::vert_stager& stager,
                    D3D12_PRIMITIVE_TOPOLOGY_TYPE tplgy,
                    D3D12_INDEX_BUFFER_STRIP_CUT_VALUE stripCut,
                    const game_update_functor<vertex_layout>& updateFunctor) :
         m_stager(stager),
         m_topo(tplgy),
         m_strip_cut(stripCut),
         component(VERTEXLAYOUT_GUID, updateFunctor)
      {
        
      }

      vertex_layout(const descriptors::vertex_layout_descriptor& desc,
                    const game_update_functor<vertex_layout>& updateFunctor) :
         vertex_layout(
            graphics::vert::make_vert_stager(desc),
            static_cast<D3D12_PRIMITIVE_TOPOLOGY_TYPE>(desc.topology),
            static_cast<D3D12_INDEX_BUFFER_STRIP_CUT_VALUE>(desc.strip_cut),
            updateFunctor)
      {

      }

      vertex_layout(const vertex_layout& r) = default;

      vertex_layout(vertex_layout&&) noexcept = default;

      virtual ~vertex_layout() noexcept = default;

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
      graphics::vert::vert_stager m_stager;
      D3D12_PRIMITIVE_TOPOLOGY_TYPE m_topo;
      D3D12_INDEX_BUFFER_STRIP_CUT_VALUE m_strip_cut;
   };
}