#pragma once
#include "include/qgl_graphics_include.h"
#include "include/GPU/qgl_graphics_device.h"
#include "include/GPU/Root-Signature/qgl_ibindable.h"
#include "include/GPU/qgl_graphics_device.h"

namespace qgl::graphics::gpu
{
   /*
    gdev: Pointer to the graphics device.
    nodeMask: Which GPU to upload the root signature to.
    */
   class QGL_GRAPHICS_API root_signature
   {
      /*
       gdev: Pointer to the graphics device.
       nodeMask: Which GPU to upload the root signature to.
       */
      root_signature(const graphics_device* gdev,
                     ibindable** descriptors,
                     size_t numDescriptors,
                     UINT nodeMask = 0);

      /*
       Copy constructor. Creates new instances of the backing D3D objects.
       */
      root_signature(const root_signature& r);

      /*
       Move constructor.
       */
      root_signature(root_signature&& r);

      /*
       Destructor.
       */
      virtual ~root_signature() noexcept;

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
      impl* m_impl_p;
   };
}