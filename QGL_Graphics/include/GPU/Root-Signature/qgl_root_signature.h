#pragma once
#include "include/qgl_graphics_include.h"
#include "include/Interfaces/qgl_igraphics_device.h"
#include "include/GPU/Root-Signature/qgl_ibindable.h"

namespace qgl::graphics::gpu
{
   class QGL_GRAPHICS_API root_signature
   {
      public:
      /*
       gdev: Pointer to the graphics device.
       descriptors: Pointer to an array of descriptors.
       nodeMask: Which GPU to upload the root signature to.
       */
      root_signature(static_ptr_ref<igraphics_device> gdev,
                     const ibindable** descriptors,
                     size_t numDescriptors,
                     UINT nodeMask = 0);

      /*
       Copy constructor. Cannot have multiple instances of a root signature.
       */
      root_signature(const root_signature&) = delete;

      /*
       Move constructor.
       */
      root_signature(root_signature&& r);

      /*
       Destructor.
       */
      ~root_signature() noexcept;

      /*
       Returns a const pointer to the D3D root signature.
       */
      const ID3D12RootSignature* get() const noexcept;

      /*
       Returns a pointer to the D3D root signature.
       */
      ID3D12RootSignature* get() noexcept;

      /*
       Returns a pointer to an array of root signature parameters.
       */
      const D3D12_ROOT_PARAMETER1* parameters() const noexcept;

      /*
       Returns the number of root signature parameters.
       */
      size_t size() const noexcept;

      private:
      struct impl;
      impl* m_impl_p = nullptr;
   };
}