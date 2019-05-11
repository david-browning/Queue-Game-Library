#pragma once
#include "qgl_graphics_include.h"
#include "qgl_d3d_device.h"
#include "qgl_ibindable.h"

namespace qgl::graphics
{
   namespace low
   {
      class graphics_command_list;
   }

   class LIB_EXPORT root_signature
   {
      public:
      /*
       numRootParams is the number of descriptor tables, constants, and constant buffers.
       */
      root_signature(const winrt::com_ptr<d3d_device>& gDev_p,
                     const std::initializer_list<low::ibindable*>& descriptors,
                     UINT nodeMask = 0);

      /*
       Copy constructor.
       Performs a deep copy.
       */
      root_signature(const root_signature& r);

      /*
       Move constructor.
       Performs a deep copy and invalidates the right-hand operand.
       */
      root_signature(root_signature&& r);

      virtual ~root_signature() noexcept;

      #pragma region Getters
      /*
       Returns a com_ptr to the root signature that DirectX 12 uses.
       */
      inline winrt::com_ptr<ID3D12RootSignature>& com_get()
      {
         return m_rootSig_p;
      }

      inline const winrt::com_ptr<ID3D12RootSignature>& com_get() const
      {
         return m_rootSig_p;
      }

      inline ID3D12RootSignature* get()
      {
         return m_rootSig_p.get();
      }

      inline const ID3D12RootSignature* get() const
      {
         return m_rootSig_p.get();
      }
      #pragma endregion

      private:
      friend class low::graphics_command_list;

      const std::vector<D3D12_ROOT_PARAMETER1>& params() const
      {
         return m_params;
      }

      /*
       Builds a DirectX root signature using the root parameters and sampler descriptions
       current stored here.
       */
      void p_allocate();

      #pragma warning(push)
      #pragma warning(disable: 4251)
      /*
       Collection or root parameters that will be bound to the root signature.
       */
      std::vector<D3D12_ROOT_PARAMETER1> m_params;

      /*
       Pointer to the d3d device used to create and upload the root signature.
       */
      winrt::com_ptr<d3d_device> m_dev_p;

      /*
       Root Signature.
       This gets set by p_finalize() which is called when trying to access the root signature.
       */
      mutable winrt::com_ptr<ID3D12RootSignature> m_rootSig_p;

      #pragma warning(pop)

      /*
       Which GPU to upload the root signature to.
       */
      UINT m_nodeMask;

   };
}