#pragma once
#include "qgl_graphics_include.h"

namespace qgl::graphics::low
{
   class graphics_command_list;
   class copy_command_list;

   enum class RESOURCE_TYPES
   {
      RESOURCE_TYPE_UNKNOWN,
      RESOURCE_TYPE_RENDER_TARGET,
      RESOURCE_TYPE_TEXTURE,
      RESOURCE_TYPE_CONST_BUFFER,
      RESOURCE_TYPE_DEPTH_STENCIL,
      RESOURCE_TYPE_SAMPLER,
   };

   /*
    ResourceDescriptionT describes the resource. For a buffer, this may be a 
      D3D12_SUBRESOURCE_DATA.
    ViewDescriptionT is the type of view that gets exposed to the shader pipeline. Most likely it
       is D3D_*_BUFFER_VIEW
    resourceT is the type of resource. Often, this will be a d3d_resource.
    UAlignment is the alignment of the resource.

    GPU Buffers can offer their resources to the OS to free video memory. It is the application's
    responsibility to offer and reclaim a resource when it is needed. A good time to offer 
    resources is when the app is minimized or suspended.
    */
   template<typename ResourceDescriptionT, typename ViewDescriptionT, typename resourceT>
   class LIB_EXPORT igpu_buffer
   {
      friend class graphics_command_list;
      friend class copy_command_list;

      public:
      igpu_buffer(const winrt::com_ptr<d3d_device>& dev_p) :
         m_dev_p(dev_p),
         m_resource(nullptr),
         m_curState(D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COMMON)
      {
      }

      igpu_buffer(const igpu_buffer& r) = delete;

      /*
       Move constructor
       */
      igpu_buffer(igpu_buffer&& r) :
         m_dev_p(std::move(r.m_dev_p)),
         m_curState(std::move(r.m_curState)),
         m_resource(std::move(r.m_resource))
      {

      }

      //com_ptrs are automatically destructed.
      virtual ~igpu_buffer() noexcept = default;

      /*
       Returns the state the resource is in.
       */
      inline D3D12_RESOURCE_STATES state() const
      {
         return m_curState;
      }

      /*
       Returns a description of the resource.
       */
      virtual ResourceDescriptionT description() const = 0;

      /*
       Returns a view that can be bound to the shader pipeline.
       */
      virtual ViewDescriptionT view() const = 0;

      #pragma region Getters for the underlying resource

      winrt::com_ptr<resourceT>& com_get()
      {
         return m_resource;
      }

      const winrt::com_ptr<resourceT>& com_get() const
      {
         return m_resource;
      }

      resourceT* get()
      {
         return m_resource.get();
      }

      const resourceT* get() const
      {
         return m_resource.get();
      }
      #pragma endregion

      friend void swap(igpu_buffer& first, igpu_buffer& second) noexcept
      {
         using std::swap;
         swap(first.m_dev_p, second.m_dev_p);
         swap(first.m_resource, second.m_resource);
         swap(first.m_curState, second.m_curState);
      }

      igpu_buffer& operator=(igpu_buffer r) noexcept
      {
         swap(*this, r);
         return *this;
      }

      protected:
      /*
       Changes the state of the resource. This is protected to only
       certain classes can change the state in the correct context.
       */
      inline void state(D3D12_RESOURCE_STATES newState)
      {
         m_curState = newState;;
      }

      #pragma warning(push)
      #pragma warning(disable: 4251)
      /*
       Pointer to the graphics device.
       */
      winrt::com_ptr<d3d_device> m_dev_p;

      /*
       Pointer to the resource. This could be a D3D12 Heap or D3D12 Resource.
       */
      winrt::com_ptr<resourceT> m_resource;
      #pragma warning(pop)

      private:
      D3D12_RESOURCE_STATES m_curState;
   };
}