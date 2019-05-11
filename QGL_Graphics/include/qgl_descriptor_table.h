#pragma once
#include "qgl_graphics_include.h"
#include "qgl_ibindable.h"
#include "qgl_descriptor_range.h"

namespace qgl::graphics::low
{
   class LIB_EXPORT descriptor_table : public low::ibindable
   {
      public:
       /*
       Constructor.
       Sets the descriptor ranges, and GPU handle.
       rootIndex: Index in the root signature where the table will be bound.
       */
      descriptor_table(std::initializer_list<descriptor_range>& ranges,
                       const CD3DX12_GPU_DESCRIPTOR_HANDLE& handle,
                       UINT rootIndex,
                       D3D12_SHADER_VISIBILITY visibility = D3D12_SHADER_VISIBILITY_ALL);

      descriptor_table(const std::vector<descriptor_range>& ranges,
                       const CD3DX12_GPU_DESCRIPTOR_HANDLE& handle,
                       UINT rootIndex,
                       D3D12_SHADER_VISIBILITY visibility = D3D12_SHADER_VISIBILITY_ALL);

      descriptor_table(const descriptor_table& r);

      descriptor_table(descriptor_table&& r);

      virtual ~descriptor_table();

      /*
       Overload required by ibindable.
       */
      virtual const CD3DX12_ROOT_PARAMETER1 root_param() const
      {
         return m_table;
      }

      /*
       Returns the GPU address where the table begins.
       */
      virtual D3D12_GPU_DESCRIPTOR_HANDLE where() const
      {
         return m_where;
      }

      inline size_t size() const
      {
         return m_ranges.size();
      }

      private:
      void p_allocate();

      #pragma warning(push)
      #pragma warning(disable: 4251)
      /*
       Collection of ranges that the table contains.
       */
      std::vector<D3D12_DESCRIPTOR_RANGE1> m_ranges;
      #pragma warning(pop)

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