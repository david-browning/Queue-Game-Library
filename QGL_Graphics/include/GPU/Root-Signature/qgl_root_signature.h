#pragma once
#include "include/qgl_graphics_include.h"
#include "include/qgl_graphics_device.h"
#include "include/GPU/Root-Signature/qgl_ibindable.h"

namespace qgl::graphics::gpu
{
   using ibindable_stager = typename std::list<ibindable*>;

   /*
    Creates a collection of ibindable pointers that will be passed to another 
    object. The ibindable stager returned by this contains only references to 
    the ibindables. Freeing the ibindables that were passed to this will cause 
    this to point to invalid memory.
    */
   template<class ForwardIt>
   inline ibindable_stager make_ibindable_stager(ForwardIt first, 
                                                 ForwardIt last)
   {
      using itType = std::remove_reference<decltype(*first)>::type;
      ibindable_stager ret;
      if constexpr (std::is_pointer<itType>::value)
      {
         // If the iterators point to a pointer
         static_assert(std::is_same<ibindable*, itType>::value,
                    "Dereferencing first does not yield a pointer to 'ibindable'.");

         for (; first != last; first++)
         {
            ret.push_back(*first);
         }
      }
      else if constexpr(std::is_reference<itType>::value ||
                        std::is_same<itType, std::reference_wrapper<ibindable>>::value)
      {
         // The iterator points to a reference.
         static_assert(std::is_same<
                          std::add_lvalue_reference<ibindable>::type, 
                          itType>::value ||
                       std::is_same<
                          std::reference_wrapper<ibindable>, 
                          itType>::value,
                       "Dereferencing first does not yield a pointer to 'ibindable'.");

         for (; first != last; first++)
         {
            ret.push_back(std::addressof(first->get()));
         }
      }
      else
      {
         static_assert(false,
                       "Dereferencing first must yield a pointer or reference.");
      }

      return ret;
   }

   /*
    Creates a collection of ibindable pointers that will be passed to another
    object. The ibindable stager returned by this contains only references to
    the ibindables. Freeing the ibindables that were passed to this will cause
    this to point to invalid memory.
    */
   inline ibindable_stager make_ibindable_stager(
      std::initializer_list<std::reference_wrapper<ibindable>> bindings)
   {
      ibindable_stager ret;
      for (auto& b : bindings)
      {
         ret.push_back(std::addressof(b.get()));
      }

      return ret;
   }

   /*
    Creates a collection of ibindable pointers that will be passed to another
    object. The ibindable stager returned by this contains only references to
    the ibindables. Freeing the ibindables that were passed to this will cause
    this to point to invalid memory.
    */
   inline ibindable_stager make_ibindable_stager(
      std::initializer_list<std::shared_ptr<ibindable>> bindings)
   {
      ibindable_stager ret;
      for (auto& b : bindings)
      {
         ret.push_back(b.get());
      }

      return ret;
   }

   /*
    Creates a collection of ibindable pointers that will be passed to another
    object. The ibindable stager returned by this contains only references to
    the ibindables. Freeing the ibindables that were passed to this will cause
    this to point to invalid memory.
    */
   inline ibindable_stager make_ibindable_stager(
      std::initializer_list<std::unique_ptr<ibindable>> bindings)
   {
      ibindable_stager ret;
      for (auto& b : bindings)
      {
         ret.push_back(b.get());
      }

      return ret;
   }

   class root_signature
   {
      public:
      /*
       nodeMask: Which GPU to upload the root signature to.
       */
      root_signature(const ibindable_stager& bindables,
                     i3d_device* dev_p,
                     size_t nodeMask = 0)
      {
         collect(bindables);
         construct(dev_p, nodeMask);
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
      void collect(const ibindable_stager& bindables)
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

      void construct(i3d_device* dev_p, size_t nodeMask)
      {
         D3D12_FEATURE_DATA_ROOT_SIGNATURE featureData = {};

         //This is the highest version the sample supports. 
         //If CheckFeatureSupport succeeds, the HighestVersion returned will 
         //not be greater than this.
         featureData.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_1;

         HRESULT hr = dev_p->CheckFeatureSupport(
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
         winrt::com_ptr<ID3DBlob> signature;
         winrt::com_ptr<ID3DBlob> error;
         winrt::check_hresult(D3DX12SerializeVersionedRootSignature(&desc,
                                                                    featureData.HighestVersion,
                                                                    signature.put(),
                                                                    error.put()));

         //Create the signature.
         winrt::check_hresult(dev_p->CreateRootSignature(
            static_cast<UINT>(nodeMask),
            signature->GetBufferPointer(),
            signature->GetBufferSize(),
            IID_PPV_ARGS(m_rootSig_p.put())));

         name_d3d(m_rootSig_p.get(), L"Root Signature");
      }

      winrt::com_ptr<ID3D12RootSignature> m_rootSig_p;
      std::vector<D3D12_ROOT_PARAMETER1> m_params;
   };
}