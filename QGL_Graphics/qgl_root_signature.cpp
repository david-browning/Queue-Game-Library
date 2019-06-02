#include "pch.h"
#include "include\GPU\Root-Signature\qgl_root_signature.h"

namespace qgl::graphics::gpu
{
   struct root_signature::impl
   {
      impl(d3d_device* gdev,
           const ibindable** descriptors,
           size_t numDescriptors,
           UINT nodeMask)
      {
         collect(numDescriptors, descriptors);
         construct(gdev, nodeMask);
      }

      impl(const impl&) = default;

      impl(impl&&) = default;

      ~impl() noexcept = default;

      /*
       Verify that no descriptors share the same root index.
       Then push back the bindings for each resource.
       A slot is occupied if the void* is not nullptr.
       Create a list of duplicates and fill it with nullptr to indicate
       each register slot is empty.
      */
      void collect(size_t numDescriptors,
                   const ibindable** descriptors)
      {
         std::vector<void*> noDuplicateList(numDescriptors, nullptr);
         for (size_t i = 0; i < numDescriptors; i++)
         {
            auto param = descriptors[i];
            auto reg = param->root_index();

            if (noDuplicateList[reg] != nullptr)
            {
               //Throw an exception.
               char exceptionMessage[128];
               sprintf_s(exceptionMessage,
                         "Two root parameters cannot use the same slot. "
                         "Slot %i is already occupied.",
                         reg);
               throw std::invalid_argument(exceptionMessage);
            }

            //Put something besides nullptr in this slot so we know it cannot 
            //be used later.
            static const auto notNullPtr = reinterpret_cast<void*>(-1);
            noDuplicateList[reg] = notNullPtr;

            //Add the root parameter to the list of root signature parameters.
            Params.push_back(*param->root_param());
         }
      }

      void construct(d3d_device* gdev,
                     UINT nodeMask)
      {
         D3D12_FEATURE_DATA_ROOT_SIGNATURE featureData = {};

         //This is the highest version the sample supports. 
         //If CheckFeatureSupport succeeds, the HighestVersion returned will 
         //not be greater than this.
         featureData.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_1;

         HRESULT hr = gdev->CheckFeatureSupport(D3D12_FEATURE_ROOT_SIGNATURE,
                                                &featureData,
                                                sizeof(featureData));
         if (FAILED(hr))
         {
            featureData.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_0;
         }

         //Populate a root signature description
         CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC desc;
         desc.Init_1_1(
            static_cast<UINT>(Params.size()),
            Params.data(),
            0,
            nullptr,
            D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

         //Serialize the signature.
         winrt::com_ptr<ID3DBlob> signature;
         winrt::com_ptr<ID3DBlob> error;
         hr = D3DX12SerializeVersionedRootSignature(&desc,
                                                    featureData.HighestVersion,
                                                    signature.put(),
                                                    error.put());
         winrt::check_hresult(hr);

         //Create the signature.
         hr = gdev->CreateRootSignature(nodeMask,
                                        signature->GetBufferPointer(),
                                        signature->GetBufferSize(),
                                        IID_PPV_ARGS(RootSig.put()));
         winrt::check_hresult(hr);

         NAME_D3D12_OBJECT(RootSig.get());
      }

      winrt::com_ptr<ID3D12RootSignature> RootSig;
      std::vector<D3D12_ROOT_PARAMETER1> Params;
   };

   root_signature::root_signature(d3d_device* gdev,
                                  const ibindable** descriptors,
                                  size_t numDescriptors,
                                  UINT nodeMask) :
      m_impl_p(new impl(gdev, descriptors, numDescriptors, nodeMask))
   {

   }

   root_signature::root_signature(root_signature&& r)
   {
      delete m_impl_p;
      m_impl_p = r.m_impl_p;
      r.m_impl_p = nullptr;
   }

   root_signature::~root_signature() noexcept
   {
      delete m_impl_p;
      m_impl_p = nullptr;
   }

   const ID3D12RootSignature* root_signature::get() const noexcept
   {
      return m_impl_p->RootSig.get();
   }

   ID3D12RootSignature* root_signature::get() noexcept
   {
      return m_impl_p->RootSig.get();
   }

   const D3D12_ROOT_PARAMETER1* root_signature::parameters() const noexcept
   {
      return m_impl_p->Params.data();
   }

   size_t root_signature::size() const noexcept
   {
      return m_impl_p->Params.size();
   }
}