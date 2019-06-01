#pragma once
#include "include/qgl_graphics_include.h"
#include "include/GPU/Root-Signature/qgl_ibindable.h"
#include "include/GPU/Descriptors/qgl_descriptor_range.h"

namespace qgl::graphics::gpu
{
   class QGL_GRAPHICS_API descriptor_table : public ibindable
   {
      public:
      descriptor_table(
         std::initializer_list<descriptor_range> ranges,
         const CD3DX12_GPU_DESCRIPTOR_HANDLE& handle,
         UINT rootIndex,
         D3D12_SHADER_VISIBILITY visibility = D3D12_SHADER_VISIBILITY_ALL);

      descriptor_table(
         descriptor_range* ranges,
         size_t numRanges,
         const CD3DX12_GPU_DESCRIPTOR_HANDLE& handle,
         UINT rootIndex,
         D3D12_SHADER_VISIBILITY visibility = D3D12_SHADER_VISIBILITY_ALL);

      /*
       Bindable objects cannot be copied.
       */
      descriptor_table(const descriptor_table&) = delete;

      /*
       Move constructor.
       */
      descriptor_table(descriptor_table&&);

      /*
       Destructor.
       */
      virtual ~descriptor_table() noexcept;

      /*
       Overload required by ibindable.
       */
      virtual const CD3DX12_ROOT_PARAMETER1* root_param() const;

      /*
       Returns the GPU address where the table begins.
       */
      virtual D3D12_GPU_DESCRIPTOR_HANDLE where() const noexcept;

      /*
       Returns the number of descriptor ranges in this descriptor table.
       */
      size_t size() const noexcept;

      private:
      struct impl;
      impl* m_impl_p;
   };
}