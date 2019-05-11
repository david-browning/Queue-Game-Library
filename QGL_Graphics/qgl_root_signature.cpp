#include "pch.h"
#include "include/qgl_root_signature.h"

qgl::graphics::root_signature::root_signature(const winrt::com_ptr<d3d_device>& gDev_p,
                                              const std::initializer_list<low::ibindable*>& descriptors,
                                              UINT nodeMask) :
   m_dev_p(gDev_p),
   m_params(0),
   m_nodeMask(nodeMask),
   m_rootSig_p(nullptr)
{
   //Verify that no descriptors share the same root index.
   //Then push back the bindings for each resource.
   //A slot is occupied if the void* is not nullptr. Create a list of duplicates and fill it with
   //nullptr to indicate each register slot is empty.
   std::vector<void*> noDuplicateList(descriptors.size(), nullptr);
   for (auto& param : descriptors)
   {
      auto reg = param->root_index();

      //If the root shader slot is occupied.
      if (noDuplicateList[reg] != nullptr)
      {
         //Throw an exception.
         char exceptionMessage[128];
         sprintf_s(exceptionMessage,
                   "Two root parameters cannot use the same slot. Slot %i is already occupied.",
                   reg);
         throw std::invalid_argument(exceptionMessage);
      }

      //Put something besides nullptr in this slot so we know it cannot be used later.
      static const auto notNullPtr = reinterpret_cast<void*>(-1);
      noDuplicateList[reg] = notNullPtr;

      //Add the root parameter to the list of root signature parameters.
      m_params.push_back(param->root_param());
   }

   p_allocate();
}

qgl::graphics::root_signature::root_signature(const root_signature& r) :
   m_dev_p(r.m_dev_p),
   m_params(r.m_params),
   m_nodeMask(r.m_nodeMask),
   m_rootSig_p(nullptr)
{
   p_allocate();
}

qgl::graphics::root_signature::root_signature(root_signature&& r) :
   m_dev_p(std::move(r.m_dev_p)),
   m_params(std::move(r.m_params)),
   m_nodeMask(r.m_nodeMask),
   m_rootSig_p(nullptr)
{
   r.m_rootSig_p = nullptr;
   p_allocate();
}

qgl::graphics::root_signature::~root_signature() noexcept
{
}

void qgl::graphics::root_signature::p_allocate()
{

   D3D12_FEATURE_DATA_ROOT_SIGNATURE featureData = {};

   // This is the highest version the sample supports. If CheckFeatureSupport succeeds, 
   //the HighestVersion returned will not be greater than this.
   featureData.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_1;
   if (FAILED(m_dev_p->CheckFeatureSupport(D3D12_FEATURE_ROOT_SIGNATURE,
                                           &featureData,
                                           sizeof(featureData))))
   {
      featureData.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_0;
   }

   //Populate a root signature description
   CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC desc;
   desc.Init_1_1(static_cast<UINT>(m_params.size()), m_params.data(), 0, nullptr,
                 D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

   //Serialize the signature.
   winrt::com_ptr<ID3DBlob> signature;
   winrt::com_ptr<ID3DBlob> error;
   winrt::check_hresult(D3DX12SerializeVersionedRootSignature(&desc,
                                                              featureData.HighestVersion,
                                                              signature.put(),
                                                              error.put()));

   //Create the signature.
   winrt::check_hresult(m_dev_p->CreateRootSignature(m_nodeMask,
                                                     signature->GetBufferPointer(),
                                                     signature->GetBufferSize(),
                                                     IID_PPV_ARGS(m_rootSig_p.put())));

   NAME_D3D12_OBJECT(m_rootSig_p.get());
}
