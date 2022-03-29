#pragma once
#include "include/qgl_graphics_include.h"
#include "include/qgl_graphics_device.h"
#include "include/GPU/Root-Signature/qgl_ibindable.h"

namespace qgl::graphics::gpu
{
   class root_signature
   {
      public:
      /*
       gdev: Pointer to the graphics device.
       descriptors: Pointer to an array of descriptors.
       nodeMask: Which GPU to upload the root signature to.
       */
      template<class IBindableIt>
      root_signature(const graphics_device_ptr& gdev,
                     IBindableIt first,
                     IBindableIt last,
                     size_t nodeMask = 0)
      {
         collect(first, last);
         construct(gdev, nodeMask);
      }

      /*
       Copy constructor. Cannot have multiple instances of a root signature.
       */
      root_signature(const root_signature&) = delete;

      /*
       Move constructor.
       */
      root_signature(root_signature&& r) = default;

      /*
       Destructor.
       */
      ~root_signature() noexcept = default;

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
      template<class IBindableIt>
      void collect(IBindableIt first, IBindableIt last)
      {
         using itType = std::remove_reference<decltype(*first)>::type;
         static_assert(
            std::is_same<itType, ibindable*>::value,
            "IBindableIt must point to an ibindable pointer.");

         std::unordered_set<ibindable::index_type> seenSlots;
         for (auto it = first; it != last; it++)
         {
            auto cur = *it;
            if (seenSlots.count(cur->root_index()))
            {
               //Throw an exception.
               char exceptionMessage[128];
               sprintf_s(exceptionMessage,
                         "Two root parameters cannot use the same slot. "
                         "Slot %i is already occupied.",
                         cur->root_index());
               throw std::invalid_argument(exceptionMessage);
            }

            seenSlots.insert(cur->root_index());
            m_params.push_back(cur->root_param());
         }
      }

      void construct(const graphics_device_ptr& gdev, size_t nodeMask)
      {
         D3D12_FEATURE_DATA_ROOT_SIGNATURE featureData = {};

         //This is the highest version the sample supports. 
         //If CheckFeatureSupport succeeds, the HighestVersion returned will 
         //not be greater than this.
         featureData.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_1;

         HRESULT hr = gdev->dev_3d()->CheckFeatureSupport(
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
            m_params.data(),
            0,
            nullptr,
            D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

         //Serialize the signature.
         winrt::com_ptr<ID3DBlob> signature;
         winrt::com_ptr<ID3DBlob> error;
         winrt::check_hresult(D3DX12SerializeVersionedRootSignature(&desc,
            featureData.HighestVersion,
            signature.put(),
            error.put()));

         //Create the signature.
         winrt::check_hresult(gdev->dev_3d()->CreateRootSignature(nodeMask,
            signature->GetBufferPointer(),
            signature->GetBufferSize(),
            IID_PPV_ARGS(m_rootSig_p.put())));

         NAME_D3D12_OBJECT(m_rootSig_p.get());
      }

      winrt::com_ptr<ID3D12RootSignature> m_rootSig_p;
      std::vector<D3D12_ROOT_PARAMETER1> m_params;
   };
}