#pragma once
#include "include/qgl_graphics_include.h"
#include "include/Interfaces/qgl_igraphics_device.h"

namespace qgl::graphics::gpu
{
   class graphics_command_list;
}

namespace qgl::graphics::gpu::buffers
{
   /*
    ResourceDescriptionT describes the resource. For a buffer, this may be a
     D3D12_SUBRESOURCE_DATA.
    ViewDescriptionT is the type of view that gets exposed to the shader
     pipeline. Most likely it is D3D_*_BUFFER_VIEW
    ResourceT is the type of resource. Often, this will be a d3d_resource.

    GPU Buffers can offer their resources to the OS to free video memory.
    It is the application's responsibility to offer and reclaim a resource
    when it is needed. A good time to offer resources is when the app is
    minimized or suspended.
    */
   template<typename ResourceDescriptionT,
      typename ViewDescriptionT,
      typename ResourceT>
      class igpu_buffer
   {
      public:
      igpu_buffer() :
         m_curState(D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COMMON)
      {

      }

      igpu_buffer(const igpu_buffer&) = default;

      igpu_buffer(igpu_buffer&&) = default;

      virtual ~igpu_buffer() noexcept = default;

      /*
       Returns a description of the resource.
       Derived classes should throw an exception if the description is not
       applicable.
       */
      virtual const ResourceDescriptionT* description() const = 0;

      /*
       Returns a view pointer that can be bound to the shader pipeline.
       Derived classes should throw an exception if the view is not
       applicable.
       */
      virtual const ViewDescriptionT* view() const = 0;

      /*
       Returns the state the resource is in.
       */
      D3D12_RESOURCE_STATES state() const noexcept
      {
         return m_curState;
      }

      /*
       Returns a const pointer to the GPU resource.
       */
      const ResourceT* get() const noexcept
      {
         return m_resource.get();
      }
      
      ResourceT* get() noexcept
      {
         return m_resource.get();
      }

      protected:
      friend class graphics_command_list;
      ResourceT** put() noexcept
      {
         return m_resource.put();
      }

      void nullify()
      {
         m_resource = nullptr;
      }

      /*
       Changes the state of the resource. This is protected to only
       certain classes can change the state in the correct context.
       */
      void state(D3D12_RESOURCE_STATES newState) noexcept
      {
         m_curState = newState;;
      }

      private:
      D3D12_RESOURCE_STATES m_curState;

      /*
       Pointer to the resource. This could be a D3D12 Heap or D3D12 Resource.
       */
      winrt::com_ptr<ResourceT> m_resource;
   };
}