#include "pch.h"
#include "include\GPU\Descriptors\qgl_descriptor_table.h"

namespace qgl::graphics::gpu
{
   struct descriptor_table::impl
   {
      impl(std::initializer_list<descriptor_range> ranges,
           CD3DX12_GPU_DESCRIPTOR_HANDLE handle,
           D3D12_SHADER_VISIBILITY visibility) :
         Where(handle),
         Visibility(visibility),
         Ranges(ranges.size())
      {
         size_t i = 0;
         for (auto& r : ranges)
         {
            Ranges[i] = *r.range();
            i++;
         }

         construct();
      }

      impl(descriptor_range* ranges,
           size_t numRanges,
           CD3DX12_GPU_DESCRIPTOR_HANDLE handle,
           D3D12_SHADER_VISIBILITY visibility) :
         Where(handle),
         Visibility(visibility),
         Ranges(numRanges)
      {
         for (size_t i = 0; i < numRanges; i++)
         {
            Ranges[i] = *ranges[i].range();
         }

         construct();
      }

      impl(const impl&) = default;

      impl(impl&&) = default;

      ~impl() noexcept = default;

      void construct()
      {
         Table.InitAsDescriptorTable(static_cast<UINT>(Ranges.size()),
                                     Ranges.data(),
                                     Visibility);
      }

      /*
       Collection of ranges that the table contains.
       */
      std::vector<D3D12_DESCRIPTOR_RANGE1> Ranges;

      /*
       Where, on the GPU, the start of the table is.
       */
      CD3DX12_GPU_DESCRIPTOR_HANDLE Where;

      /*
       The table that can actually be set in a root signature.
       */
      CD3DX12_ROOT_PARAMETER1 Table;

      /*
       Which shader stages can see the table.
       */
      D3D12_SHADER_VISIBILITY Visibility;
   };

   descriptor_table::descriptor_table(
      std::initializer_list<descriptor_range> ranges,
      CD3DX12_GPU_DESCRIPTOR_HANDLE handle,
      UINT rootIndex,
      D3D12_SHADER_VISIBILITY visibility) :
      m_impl_p(new impl(ranges, handle, visibility)),
      ibindable(rootIndex)
   {
   }

   descriptor_table::descriptor_table(
      descriptor_range* ranges,
      size_t numRanges,
      CD3DX12_GPU_DESCRIPTOR_HANDLE handle,
      UINT rootIndex, D3D12_SHADER_VISIBILITY visibility) :
      m_impl_p(new impl(ranges, numRanges, handle, visibility)),
      ibindable(rootIndex)
   {
   }

   descriptor_table::descriptor_table(descriptor_table&& r) :
      ibindable(std::move(r))
   {
      delete m_impl_p;
      m_impl_p = r.m_impl_p;
      r.m_impl_p = nullptr;
   }

   descriptor_table::~descriptor_table() noexcept
   {
      delete m_impl_p;
      m_impl_p = nullptr;
   }

   const CD3DX12_ROOT_PARAMETER1* descriptor_table::root_param() const
   {
      return &m_impl_p->Table;
   }

   D3D12_GPU_DESCRIPTOR_HANDLE descriptor_table::where() const noexcept
   {
      return m_impl_p->Where;
   }

   size_t descriptor_table::size() const noexcept
   {
      return m_impl_p->Ranges.size();
   }
}
