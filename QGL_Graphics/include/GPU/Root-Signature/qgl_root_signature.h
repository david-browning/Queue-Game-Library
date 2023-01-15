#pragma once
#include "include/qgl_graphics_include.h"
#include "include/qgl_graphics_device.h"
#include "include/GPU/Root-Signature/qgl_ibindable.h"
#include "include/Stagers/qgl_ibindable_stager.h"

namespace qgl::graphics::gpu
{
   class root_signature final
   {
      public:
      /*
       nodeMask: Which GPU to upload the root signature to.
       */
      root_signature(const stagers::ibindable_stager& bindables,
                     graphics_device& dev,
                     gpu_idx_t nodeMask = 0)
      {
         collect(bindables);
         construct(dev, nodeMask);
      }

      /*
       Copy constructor. Cannot have multiple instances of a root signature.
       */
      root_signature(const root_signature&) = delete;

      /*
       Move constructor.
       */
      root_signature(root_signature&& r) noexcept :
         m_rootSig_p(std::move(r.m_rootSig_p)),
         m_params(std::move(r.m_params))
      {
         r.m_rootSig_p = nullptr;
      }

      /*
       Destructor.
       */
      ~root_signature() noexcept
      {
         m_rootSig_p = nullptr;
      }

      /*
       Returns a const pointer to the D3D root signature.
       */
      const ID3D12RootSignature* get() const noexcept
      {
         return m_rootSig_p.get();
      }

      /*
       Returns a pointer to the D3D root signature.
       */
      ID3D12RootSignature* get() noexcept
      {
         return m_rootSig_p.get();
      }

      /*
       Returns a pointer to an array of root signature parameters.
       */
      const D3D12_ROOT_PARAMETER1* parameters() const noexcept
      {
         return m_params.data();
      }

      /*
       Returns the number of root signature parameters.
       */
      size_t size() const noexcept
      {
         return m_params.size();
      }

      private:
      void collect(const stagers::ibindable_stager& bindables)
      {
         std::unordered_set<ibindable::index_t> seenSlots;
         for (auto it = bindables.begin(); it != bindables.end(); it++)
         {
            if (seenSlots.count((*it)->root_index()))
            {
               //Throw an exception.
               char exceptionMessage[128];
               sprintf_s(exceptionMessage,
                         "Two root parameters cannot use the same slot. "
                         "Slot %i is already occupied.",
                         (*it)->root_index());
               throw std::invalid_argument(exceptionMessage);
            }

            seenSlots.insert((*it)->root_index());
            m_params.push_back((*it)->root_param());
         }
      }

      void construct(graphics_device& dev, gpu_idx_t nodeMask)
      {
         D3D12_FEATURE_DATA_ROOT_SIGNATURE featureData = {};

         //This is the highest version the sample supports. 
         //If CheckFeatureSupport succeeds, the HighestVersion returned will 
         //not be greater than this.
         featureData.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_1;

         HRESULT hr = dev.dev_3d()->CheckFeatureSupport(
            D3D12_FEATURE_ROOT_SIGNATURE,
            &featureData,
            sizeof(featureData));
         if (FAILED(hr))
         {
            featureData.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_0;
         }

         //Populate a root signature description
         CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC desc;
         desc.Init_1_1(
            static_cast<UINT>(m_params.size()),
            m_params.size() > 0 ?
               m_params.data() :
               nullptr,
            0,
            nullptr,
            D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

         //Serialize the signature.
         pptr<ID3DBlob> signature;
         pptr<ID3DBlob> error;
         check_result(D3DX12SerializeVersionedRootSignature(
            &desc,
            featureData.HighestVersion,
            signature.put(),
            error.put()));

         //Create the signature.
         check_result(dev.dev_3d()->CreateRootSignature(
            nodeMask,
            signature->GetBufferPointer(),
            signature->GetBufferSize(),
            IID_PPV_ARGS(m_rootSig_p.put())));

         name_d3d(m_rootSig_p.get(), L"Root Signature");
      }

      pptr<ID3D12RootSignature> m_rootSig_p;
      std::vector<D3D12_ROOT_PARAMETER1> m_params;
   };
}