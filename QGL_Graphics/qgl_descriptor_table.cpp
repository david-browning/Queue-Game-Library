#include "pch.h"
#include "include/qgl_descriptor_table.h"

qgl::graphics::low::descriptor_table::descriptor_table(std::initializer_list<descriptor_range>& ranges,
                                                       const CD3DX12_GPU_DESCRIPTOR_HANDLE& handle,
                                                       UINT rootIndex,
                                                       D3D12_SHADER_VISIBILITY visibility) :
   m_where(handle),
   m_visibility(visibility),
   m_ranges(ranges.size()),
   ibindable(rootIndex)
{
   size_t i = 0;
   for (auto& r : ranges)
   {
      m_ranges[i] = r.range();
      i++;
   }

   p_allocate();
}

qgl::graphics::low::descriptor_table::descriptor_table(const std::vector<descriptor_range>& ranges,
                                                       const CD3DX12_GPU_DESCRIPTOR_HANDLE& handle,
                                                       UINT rootIndex,
                                                       D3D12_SHADER_VISIBILITY visibility) :
   m_where(handle),
   m_visibility(visibility),
   m_ranges(ranges.size()),
   ibindable(rootIndex)
{
   size_t i = 0;
   for (auto& r : ranges)
   {
      m_ranges[i] = r.range();
      i++;
   }

   p_allocate();
}

qgl::graphics::low::descriptor_table::descriptor_table(const descriptor_table& r) :
   m_where(r.m_where),
   m_visibility(r.m_visibility),
   m_ranges(r.m_ranges),
   ibindable(r)
{
   p_allocate();
}

qgl::graphics::low::descriptor_table::descriptor_table(descriptor_table&& r) :
   m_where(std::move(r.m_where)),
   m_visibility(std::move(r.m_visibility)),
   m_ranges(std::move(r.m_ranges)),
   ibindable(std::move(r))
{
   p_allocate();
}

qgl::graphics::low::descriptor_table::~descriptor_table()
{
}

void qgl::graphics::low::descriptor_table::p_allocate()
{
   m_table.InitAsDescriptorTable(static_cast<UINT>(m_ranges.size()), m_ranges.data());
}
