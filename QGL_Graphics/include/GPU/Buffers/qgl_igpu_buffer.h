#pragma once
#include "include/qgl_graphics_include.h"
#include "include/GPU/Memory/qgl_igpu_allocator.h"

namespace qgl::graphics::gpu
{
   /*
    ResDescT describes the resource. For a buffer, this may be a
     D3D12_SUBRESOURCE_DATA.
    ViewDescT is the type of view that gets exposed to the shader
     pipeline. Most likely it is D3D_*_BUFFER_VIEW
    ResourceT is the type of resource. Often, this will be a igpu_resource.

    GPU buffers can offer their resources to the OS to free video memory.
    It is the application's responsibility to offer and reclaim a resource
    when it is needed. A good time to offer resources is when the app is
    minimized or suspended.
    */
   template<class ResDescT, class ViewDescT, class ResourceT>
   class igpu_buffer
   {
      public:
      //igpu_buffer(gpu_allocator_ptr&&) { }

      virtual ~igpu_buffer() noexcept = default;

      /*
       Returns a description of the resource.
       Derived classes should throw an exception if the description is not
       applicable.
       */
      virtual const ResDescT* description() const = 0;

      /*
       Returns a view pointer that can be bound to the shader pipeline.
       Derived classes should throw an exception if the view is not
       applicable.
       */
      virtual const ViewDescT* view() const = 0;

      /*
       Returns the size, in bytes, of the buffer.
       */
      virtual size_t size() const noexcept = 0;

      ///*
      // Returns the state the resource is in.
      // */
      //virtual D3D12_RESOURCE_STATES state() const noexcept = 0;

      virtual gpu_alloc_handle alloc_handle() const noexcept = 0;

      /*
       Returns a pointer to the GPU resource.
       */
      virtual const ResourceT* get() const = 0;

      /*
       Returns a pointer to the GPU resource.
       */
      virtual ResourceT* get() = 0;
   };
}