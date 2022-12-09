#pragma once
#include "include/qgl_graphics_include.h"
#include "include/Descriptors/qgl_vertex_layout_descriptor.h"
#include "include/Helpers/qgl_vert_types.h"

namespace qgl::graphics::vert
{
   class vert_stager final
   {
      public:
      /*
       Instanciates an empty stager.
       */
      vert_stager()
      {

      }

      /*
       Copies the D3D12_INPUT_ELEMENT_DESC from the iterators.
       */
      template<class ForwardIt>
      vert_stager(ForwardIt first, ForwardIt last)
      {
         using itType = typename std::remove_reference<decltype(*first)>::type;
         static_assert(std::is_same_v<itType, D3D12_INPUT_ELEMENT_DESC>,
                       "first must point to a D3D12_INPUT_ELEMENT_DESC");

         while (first != last)
         {
            push_back(*first);
            first++;
         }
      }

      vert_stager(const vert_stager& r) :
         m_descs(r.m_descs),
         m_strBuffers(r.m_strBuffers)
      {
         fix_pointers();
      }

      vert_stager(vert_stager&& r) noexcept :
         m_descs(std::move(r.m_descs)),
         m_strBuffers(std::move(r.m_strBuffers))
      {
         fix_pointers();
      }

      ~vert_stager() noexcept = default;

      /*
       Copies the D3D12_INPUT_ELEMENT_DESC to the internal data.
       */
      void push_back(const D3D12_INPUT_ELEMENT_DESC& desc)
      {
         const std::lock_guard l{ m_threadLock };
         m_strBuffers.emplace_back(desc.SemanticName);
         m_descs.push_back(desc);
         m_descs.back().SemanticName = m_strBuffers.back().c_str();
      }

      /*
       Returns a pointer to the list of D3D12_INPUT_ELEMENT_DESCs
       */
      D3D12_INPUT_ELEMENT_DESC* data() noexcept
      {
         return m_descs.data();
      }

      /*
       Returns a pointer to the list of D3D12_INPUT_ELEMENT_DESCs
       */
      const D3D12_INPUT_ELEMENT_DESC* data() const noexcept
      {
         return m_descs.data();
      }

      /*
       Returns the number of element descriptors in this.
       */
      size_t size() const noexcept
      {
         return m_descs.size();
      }

      private:
      void fix_pointers()
      {
         for (size_t i = 0; i < size(); i++)
         {
            m_descs[i].SemanticName = m_strBuffers[i].c_str();
         }
      }

      /*
       Use a mutex when doing operations in case this gets multi-threaded.
       */
      std::mutex m_threadLock;

      /*
       The input element descriptions.
       */
      std::vector<D3D12_INPUT_ELEMENT_DESC> m_descs;

      /*
       The input element descriptions store points to the semantic names and not
       the actual data. Store the string data here.
       */
      std::vector<std::string> m_strBuffers;
   };

   template<class VertForwardIt>
   inline vert_stager make_vert_stager(VertForwardIt first, VertForwardIt last)
   {
      using itType = typename std::remove_reference<decltype(*first)>::type;
      static_assert(std::is_base_of<ivert_element, itType>::value,
                    "VertForwardIt must point to a type that is derived from ivert_element.");

      vert_stager ret;

      // Use this map to make sure no semantics have duplicate indices.
      using idxMap_t = typename std::unordered_set<size_t>;
      std::unordered_map<std::string, std::unique_ptr<idxMap_t>> semIdxMap;
      for (; first != last; first++)
      {
         const auto& semName = first->name();
         if (semIdxMap.count(semName) == 0)
         {
            semIdxMap[semName] = std::make_unique<idxMap_t>();
         }
         else
         {
            auto& idxMap = semIdxMap[semName];
            auto idx = first->index();
            if (idxMap->count(idx) == 0)
            {
               idxMap->insert(idx);
            }
            else
            {
               char msg[128] = { 0 };
               sprintf_s(msg, "Semantic \"%s\" is already bound to slot %zu.",
                         semName.c_str(), idx);
               throw std::out_of_range(msg);
            }
         }

         // https://learn.microsoft.com/en-us/windows/win32/api/d3d12/ns-d3d12-d3d12_input_element_desc
         D3D12_INPUT_ELEMENT_DESC e;
         e.SemanticName = semName.c_str();
         e.SemanticIndex = static_cast<UINT>(first->index());
         e.InputSlotClass = first->classification();
         e.InputSlot = static_cast<UINT>(first->slot());
         e.Format = first->format();
         e.AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
         e.InstanceDataStepRate = 0;
         ret.push_back(e);
      }

      return ret;
   }

   inline vert_stager make_vert_stager(std::initializer_list<ivert_element> verts)
   {
      return make_vert_stager(verts.begin(), verts.end());
   }

   inline vert_stager make_vert_stager(
      const descriptors::vertex_layout_descriptor& desc)
   {
      vert_stager ret;

      // Use this map to make sure no semantics have duplicate indices.
      using idxMap_t =
         typename std::unordered_set<decltype(
            descriptors::vertex_element_descriptor::semantic_index)>;
      std::unordered_map<std::string, std::unique_ptr<idxMap_t>> semIdxMap;
      for (size_t i = 0; i < desc.element_count; i++)
      {
         auto& cur = desc.elements.at(i);
         std::string semName{
            cur.semantic_name.data(),
            descriptors::vertex_element_descriptor::MAX_SEMANTIC_NAME_LEN };
         if (semIdxMap.count(semName) == 0)
         {
            semIdxMap[semName] = std::make_unique<idxMap_t>();
         }
         else
         {
            auto& idxMap = semIdxMap[semName];
            auto idx = cur.semantic_index;
            if (idxMap->count(idx) == 0)
            {
               idxMap->insert(idx);
            }
            else
            {
               char msg[128] = { 0 };
               sprintf_s(msg, "Semantic \"%s\" is already bound to slot %u.",
                         semName.c_str(), idx);
               throw std::out_of_range(msg);
            }
         }

         // https://learn.microsoft.com/en-us/windows/win32/api/d3d12/ns-d3d12-d3d12_input_element_desc
         D3D12_INPUT_ELEMENT_DESC e;
         e.SemanticName = semName.c_str();
         e.SemanticIndex = cur.semantic_index;
         e.InputSlotClass =
            static_cast<D3D12_INPUT_CLASSIFICATION>(cur.data_class);
         e.InputSlot = cur.slot;
         e.Format = static_cast<DXGI_FORMAT>(cur.format);
         e.AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
         e.InstanceDataStepRate = 0;
         ret.push_back(e);
      }

      return ret;
   }
}