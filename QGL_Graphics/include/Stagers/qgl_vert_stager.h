#pragma once
#include "include/qgl_graphics_include.h"
#include "include/Shaders/qgl_vertex_layout_descriptor.h"
#include "include/Shaders/qgl_vert_types.h"

namespace qgl::stagers
{
   class vert_stager final
   {
      public:
      /*
       Instantiates an empty stager.
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

         // Save the char* in the element description.
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
         size_t i = 0;
         auto strIt = m_strBuffers.begin();
         for (; i < size(); i++, strIt++)
         {
            m_descs[i].SemanticName = strIt->c_str();
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
       Use a list so that when items are added, it doesn't invalidate the old
       string pointers.
       */
      std::list<std::string> m_strBuffers;
   };

   /*
    Creates a D3D12_INPUT_ELEMENT_DESC from an ivert_element.
    */
   inline D3D12_INPUT_ELEMENT_DESC to_d3d_element(
      const graphics::shaders::ivert_element* e_p,
      graphics::input_data_class_t inClass,
      graphics::input_slot_t slot) noexcept
   {
      // https://learn.microsoft.com/en-us/windows/win32/api/d3d12/ns-d3d12-d3d12_input_element_desc
      D3D12_INPUT_ELEMENT_DESC ret;
      ret.SemanticName = e_p->name().c_str();
      ret.SemanticIndex = static_cast<UINT>(e_p->index());
      ret.InputSlotClass = graphics::to_d3d_input_class(inClass);
      ret.InputSlot = graphics::to_d3d_input_slot(slot);
      ret.Format = e_p->format();
      ret.AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
      ret.InstanceDataStepRate = 0;
      return ret;
   }

   /*
    Creates a D3D12_INPUT_ELEMENT_DESC from an ivert_element.
    */
   inline D3D12_INPUT_ELEMENT_DESC to_d3d_element(
     const graphics::shaders::ivert_element& e,
      graphics::input_data_class_t inClass,
      graphics::input_slot_t slot) noexcept
   {
      // https://learn.microsoft.com/en-us/windows/win32/api/d3d12/ns-d3d12-d3d12_input_element_desc
      D3D12_INPUT_ELEMENT_DESC ret;
      ret.SemanticName = e.name().c_str();
      ret.SemanticIndex = static_cast<UINT>(e.index());
      ret.InputSlotClass = graphics::to_d3d_input_class(inClass);
      ret.InputSlot = graphics::to_d3d_input_slot(slot);
      ret.Format = e.format();
      ret.AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
      ret.InstanceDataStepRate = 0;
      return ret;
   }

   /*
    Creates a D3D12_INPUT_ELEMENT_DESC from an ivert_element.
    */
   inline D3D12_INPUT_ELEMENT_DESC to_d3d_element(
      const std::reference_wrapper<graphics::shaders::ivert_element>& e,
      graphics::input_data_class_t inClass,
      graphics::input_slot_t slot) noexcept
   {
      // https://learn.microsoft.com/en-us/windows/win32/api/d3d12/ns-d3d12-d3d12_input_element_desc
      D3D12_INPUT_ELEMENT_DESC ret;
      ret.SemanticName = e.get().name().c_str();
      ret.SemanticIndex = static_cast<UINT>(e.get().index());
      ret.InputSlotClass = graphics::to_d3d_input_class(inClass);
      ret.InputSlot = graphics::to_d3d_input_slot(slot);
      ret.Format = e.get().format();
      ret.AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
      ret.InstanceDataStepRate = 0;
      return ret;
   }

   /*
    Creates a D3D12_INPUT_ELEMENT_DESC from an ivert_element.
    */
   inline D3D12_INPUT_ELEMENT_DESC to_d3d_element(
      const std::shared_ptr<graphics::shaders::ivert_element>& e_p,
      graphics::input_data_class_t inClass,
      graphics::input_slot_t slot) noexcept
   {

      // https://learn.microsoft.com/en-us/windows/win32/api/d3d12/ns-d3d12-d3d12_input_element_desc
      D3D12_INPUT_ELEMENT_DESC ret;
      ret.SemanticName = e_p->name().c_str();
      ret.SemanticIndex = static_cast<UINT>(e_p->index());
      ret.InputSlotClass = graphics::to_d3d_input_class(inClass);
      ret.InputSlot = graphics::to_d3d_input_slot(slot);
      ret.Format = e_p->format();
      ret.AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
      ret.InstanceDataStepRate = 0;
      return ret;
   }

   /*
    Creates a D3D12_INPUT_ELEMENT_DESC from an ivert_element.
    */
   inline D3D12_INPUT_ELEMENT_DESC to_d3d_element(
      const std::unique_ptr<graphics::shaders::ivert_element>& e_p,
      graphics::input_data_class_t inClass,
      graphics::input_slot_t slot) noexcept
   {

      // https://learn.microsoft.com/en-us/windows/win32/api/d3d12/ns-d3d12-d3d12_input_element_desc
      D3D12_INPUT_ELEMENT_DESC ret;
      ret.SemanticName = e_p->name().c_str();
      ret.SemanticIndex = static_cast<UINT>(e_p->index());
      ret.InputSlotClass = graphics::to_d3d_input_class(inClass);
      ret.InputSlot = graphics::to_d3d_input_slot(slot);
      ret.Format = e_p->format();
      ret.AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
      ret.InstanceDataStepRate = 0;
      return ret;
   }

   /*
    Creates a D3D12_INPUT_ELEMENT_DESC from an ivert_element.
    */
   inline D3D12_INPUT_ELEMENT_DESC to_d3d_element(
      const graphics::shaders::vertex_element_descriptor& e,
      graphics::input_data_class_t inClass,
      graphics::input_slot_t slot) noexcept
   {

      // https://learn.microsoft.com/en-us/windows/win32/api/d3d12/ns-d3d12-d3d12_input_element_desc
      D3D12_INPUT_ELEMENT_DESC ret;
      ret.SemanticName = e.semantic_name.data();
      ret.SemanticIndex = static_cast<UINT>(e.semantic_index);
      ret.InputSlotClass = graphics::to_d3d_input_class(inClass);
      ret.InputSlot = graphics::to_d3d_input_slot(slot);
      ret.Format = static_cast<DXGI_FORMAT>(e.format);
      ret.AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
      ret.InstanceDataStepRate = 0;
      return ret;
   }

   /*
    Returns a reference to the ivert_element's name.
    */
   inline const std::string& element_name(
      const graphics::shaders::ivert_element& e) noexcept
   {
      return e.name();
   }

   /*
    Returns a reference to the ivert_element's name.
    */
   inline const std::string& element_name(
      const std::reference_wrapper<graphics::shaders::ivert_element>& e) noexcept
   {
      return e.get().name();
   }

   /*
    Returns a reference to the ivert_element's name.
    */
   inline const std::string& element_name(
      const graphics::shaders::ivert_element* e_p) noexcept
   {
      return e_p->name();
   }

   /*
    Returns a reference to the ivert_element's name.
    */
   inline const std::string& element_name(
      const std::shared_ptr<graphics::shaders::ivert_element>& e_p) noexcept
   {
      return e_p->name();
   }

   /*
    Returns a reference to the ivert_element's name.
    */
   inline const std::string& element_name(
      const std::unique_ptr<graphics::shaders::ivert_element>& e_p) noexcept
   {
      return e_p->name();
   }

   /*
    Returns a reference to the ivert_element's name.
    */
   inline auto element_name(
      const graphics::shaders::vertex_element_descriptor& e) noexcept
   {
      return e.semantic_name;
   }

   /*
    Gets the ivert_element's index.
    */
   inline auto element_index(
      const graphics::shaders::ivert_element& e) noexcept
   {
      return e.index();
   }

   /*
    Gets the ivert_element's index.
    */
   inline auto element_index(
      const std::reference_wrapper<graphics::shaders::ivert_element>& e) noexcept
   {
      return e.get().index();
   }

   /*
    Gets the ivert_element's index.
    */
   inline auto element_index(
      const graphics::shaders::ivert_element* e_p) noexcept
   {
      return e_p->index();
   }

   /*
    Gets the ivert_element's index.
    */
   inline auto element_index(
      const std::shared_ptr<graphics::shaders::ivert_element>& e_p) noexcept
   {
      return e_p->index();
   }

   /*
    Gets the ivert_element's index.
    */
   inline auto element_index(
      const std::unique_ptr<graphics::shaders::ivert_element>& e_p) noexcept
   {
      return e_p->index();
   }

   /*
    Gets the ivert_element's index.
    */
   inline auto element_index(
      const graphics::shaders::vertex_element_descriptor& e) noexcept
   {
      return e.semantic_index;
   }

   /*
    Creates a vertex shader from iterators. The iterators must point to 
    some form of an ivert_element pointer or reference since ivert_element is
    an abstract type.
    */
   template<class VertForwardIt>
   inline vert_stager make_vert_stager(VertForwardIt first, 
                                       VertForwardIt last,
                                       graphics::input_slot_t slot,
                                       graphics::input_data_class_t inClass)
   {
      using namespace qgl::graphics::shaders;
      using itType = typename std::remove_reference<decltype(*first)>::type;
      vert_stager ret;
      // Use this map to make sure no semantics have duplicate indices.
      using idxMap_t =
         typename std::unordered_set<decltype(
            vertex_element_descriptor::semantic_index)>;
      std::unordered_map<std::string, std::unique_ptr<idxMap_t>> semIdxMap;

      for (; first != last; first++)
      {
         std::string semName{ element_name(*first) };

         if (semIdxMap.count(semName) == 0)
         {
            semIdxMap[semName] = std::make_unique<idxMap_t>();
         }
         else
         {
            auto& idxMap = semIdxMap[semName];
            auto semIdx = element_index(*first);

            if (idxMap->count(semIdx) == 0)
            {
               idxMap->insert(semIdx);
            }
            else
            {
               char msg[128] = { 0 };
               sprintf_s(msg, "Semantic \"%s\" is already bound to slot %u.",
                         semName.c_str(), semIdx);
               throw std::out_of_range(msg);
            }
         }

         ret.push_back(to_d3d_element(*first, inClass, slot));
      }

      return ret;
   }

   /*
    Creates a vertex shader from a vertex_layout_description.
    */
   inline vert_stager make_vert_stager(
      const graphics::shaders::vertex_layout_descriptor& desc)
   {
      using namespace qgl::graphics::shaders;
      vert_stager ret;
      // Use this map to make sure no semantics have duplicate indices.
      using idxMap_t =
         typename std::unordered_set<decltype(
            vertex_element_descriptor::semantic_index)>;
      std::unordered_map<std::string, std::unique_ptr<idxMap_t>> semIdxMap;

      for (uint8_t i = 0; i < desc.element_count; i++)
      {
         std::string semName{ desc.elements[i].semantic_name.data() };

         if (semIdxMap.count(semName) == 0)
         {
            semIdxMap[semName] = std::make_unique<idxMap_t>();
         }
         else
         {
            auto& idxMap = semIdxMap[semName];
            auto semIdx = element_index(desc.elements[i]);

            if (idxMap->count(semIdx) == 0)
            {
               idxMap->insert(semIdx);
            }
            else
            {
               char msg[128] = { 0 };
               sprintf_s(msg, "Semantic \"%s\" is already bound to slot %u.",
                         semName.c_str(), semIdx);
               throw std::out_of_range(msg);
            }
         }

         ret.push_back(to_d3d_element(desc.elements[i], 
                                      desc.data_class, 
                                      desc.slot));
      }

      return ret;
   }

   /*
    Creates a vertex shader from an initializer list. The list elements must 
    point to some form of an ivert_element pointer or reference since 
    ivert_element is an abstract type.
    */
   template<class VertT>
   inline vert_stager make_vert_stager(
      std::initializer_list<VertT> vertElements)
   {
      return make_vert_stager(vertElements.begin(), vertElements.end());
   }
}