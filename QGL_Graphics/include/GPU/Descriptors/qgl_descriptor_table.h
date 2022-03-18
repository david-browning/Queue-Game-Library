#pragma once
#include "include/qgl_graphics_include.h"
#include "include/GPU/Root-Signature/qgl_ibindable.h"
#include "include/GPU/Descriptors/qgl_descriptor_range.h"

namespace qgl::graphics::gpu
{
   /*
    Descriptor tables are a collection of shader resources.
    Descriptor tables are bound to a root signature.
    */
   class descriptor_table : public ibindable
   {
      public:
      /*
       Sets the descriptor ranges and GPU handle.
       rootIndex: index in the root signature where the table will be bound.
       */
      descriptor_table(
         std::initializer_list<descriptor_range> ranges,
         CD3DX12_GPU_DESCRIPTOR_HANDLE handle,
         UINT rootIndex,
         D3D12_SHADER_VISIBILITY visibility = D3D12_SHADER_VISIBILITY_ALL) :
         m_where(handle),
         m_visibility(visibility),
         m_ranges(ranges.size()),
         ibindable(rootIndex)
      {
         size_t i = 0;
         for (auto& r : ranges)
         {
            m_ranges[i] = *r.range();
            i++;
         }

         construct();
      }

      /*
       Sets the descriptor ranges and GPU handle.
       rootIndex: index in the root signature where the table will be bound.
       */
      descriptor_table(
         descriptor_range* ranges,
         size_t numRanges,
         CD3DX12_GPU_DESCRIPTOR_HANDLE handle,
         UINT rootIndex,
         D3D12_SHADER_VISIBILITY visibility = D3D12_SHADER_VISIBILITY_ALL) :
         m_where(handle),
         m_visibility(visibility),
         m_ranges(numRanges),
         ibindable(rootIndex)
      {
         for (size_t i = 0; i < numRanges; i++)
         {
            m_ranges[i] = *ranges[i].range();
         }

         construct();
      }

      /*
       Bindable objects cannot be copied.
       */
      descriptor_table(const descriptor_table&) = delete;

      /*
       Move constructor.
       */
      descriptor_table(descriptor_table&&) = default;

      /*
       Destructor.
       */
      virtual ~descriptor_table() noexcept = default;

      /*
       Overload required by ibindable.
       */
      virtual const CD3DX12_ROOT_PARAMETER1& root_param() const
      {
         return m_table;
      }

      /*
       Returns the GPU address where the table begins.
       */
      virtual D3D12_GPU_DESCRIPTOR_HANDLE where() const noexcept
      {
         return m_where;
      }

      /*
       Returns the number of descriptor ranges in this descriptor table.
       */
      size_t size() const noexcept
      {
         return m_ranges.size();
      }

      private:

      void construct()
      {
         m_table.InitAsDescriptorTable(static_cast<UINT>(m_ranges.size()),
            m_ranges.data(),
            m_visibility);
      }

      /*
       Collection of ranges that the table contains.
       */
      std::vector<D3D12_DESCRIPTOR_RANGE1> m_ranges;

      /*
       Where, on the GPU, the start of the table is.
       */
      CD3DX12_GPU_DESCRIPTOR_HANDLE m_where;

      /*
       The table that can actually be set in a root signature.
       */
      CD3DX12_ROOT_PARAMETER1 m_table;

      /*
       Which shader stages can see the table.
       */
      D3D12_SHADER_VISIBILITY m_visibility;
   };
}