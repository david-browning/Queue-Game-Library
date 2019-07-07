#include "pch.h"
#include "include/GPU/Render/qgl_frame.h"

namespace qgl::graphics::gpu::render
{
   class frame_1_0 : public iframe
   {
      public:

      frame_1_0()
      {

      }

      virtual void release()
      {
         //Calls the destructor.
         delete this;
      }

      HRESULT make(static_ptr_ref<igraphics_device> dev_p,
                   IFRAME_CREATION_PARAMS* params_p) noexcept
      {
         m_renderTarget_p = std::make_unique<render_target>(
            dev_p,
            params_p->RTVHeap,
            params_p->FrameIndex);

         m_depthStencil_p = std::make_unique<depth_stencil>(
            params_p->DepthStencilConfig.get(),
            dev_p,
            params_p->DSVHeap,
            params_p->Window,
            params_p->FrameIndex);

         m_viewport_p = std::make_unique<viewport>(dev_p,
                                                   params_p->Window);

         m_scissor_p = std::make_unique<scissor>(m_viewport_p.get());

         m_rasterizer_p = std::make_unique<rasterizer>(
            params_p->RasterizerConfig.get());

         m_blender_p = std::make_unique<blender>(
            params_p->BlenderConfig.get());

         return S_OK;
      }

      virtual render_target* frame_buffer() noexcept
      {
         return m_renderTarget_p.get();
      }

      virtual const render_target* const_frame_buffer() const noexcept
      {
         return m_renderTarget_p.get();
      }

      virtual depth_stencil* frame_stencil() noexcept
      {
         return m_depthStencil_p.get();
      }

      virtual const depth_stencil* const_frame_stencil() const noexcept
      {
         return m_depthStencil_p.get();
      }

      virtual viewport* frame_viewport() noexcept
      {
         return m_viewport_p.get();
      }

      virtual const viewport* const_frame_viewport() const noexcept
      {
         return m_viewport_p.get();
      }

      virtual scissor* frame_scissor() noexcept
      {
         return m_scissor_p.get();
      }

      virtual const scissor* const_frame_scissor() const noexcept
      {
         return m_scissor_p.get();
      }

      virtual rasterizer* frame_rasterizer() noexcept
      {
         return m_rasterizer_p.get();
      }

      virtual const rasterizer* const_frame_rasterizer() const noexcept
      {
         return m_rasterizer_p.get();
      }

      virtual blender* frame_blender() noexcept
      {
         return m_blender_p.get();
      }

      const blender* const_frame_blender() const noexcept
      {
         return m_blender_p.get();
      }

      virtual void acquire(static_ptr_ref<igraphics_device> dev_p)
      {
         m_renderTarget_p->acquire_resources(dev_p->d3d11on12_device());
      }


      virtual void release(static_ptr_ref<igraphics_device> dev_p)
      {
         m_renderTarget_p->release_resources(dev_p->d3d11on12_device());
      }

      virtual void dispose(static_ptr_ref<igraphics_device> dev_p)
      {
         m_renderTarget_p->dispose(dev_p->d3d11on12_device());
      }


      private:
      std::unique_ptr<render_target> m_renderTarget_p;
      std::unique_ptr<depth_stencil> m_depthStencil_p;
      std::unique_ptr<viewport> m_viewport_p;
      std::unique_ptr<scissor> m_scissor_p;
      std::unique_ptr<rasterizer> m_rasterizer_p;
      std::unique_ptr<blender> m_blender_p;
   };

   HRESULT qgl_make_frame(static_ptr_ref<igraphics_device> dev_p,
                          IFRAME_CREATION_PARAMS* params_p,
                          qgl_version_t v,
                          iframe** out_p) noexcept
   {
      //Make sure out_p is not nullptr.
      if (out_p == nullptr)
      {
         #ifdef DEBUG
         OutputDebugString(L"out_p cannot be nullptr.");
         #endif
         return E_INVALIDARG;
      }

      iframe* ret = nullptr;

      switch (std::hash<qgl_version_t>{}(v))
      {
         case qgl::hashes::VERSION_0_1_HASH:
         case qgl::hashes::VERSION_0_2_HASH:
         {
            ret = new(std::nothrow)frame_1_0();

            if (ret == nullptr)
            {
               #ifdef DEBUG
               OutputDebugString(L"Out of memory!");
               #endif
               return E_OUTOFMEMORY;
            }

            auto hr = (dynamic_cast<frame_1_0*>(ret)->make(dev_p,
                                                           params_p));
            if (FAILED(hr))
            {
               return hr;
            }

            break;
         }
         default:
         {
            #ifdef DEBUG
            OutputDebugString(L"This QGL version is not supported.");
            #endif
            return E_NOINTERFACE;
         }
      }

      *out_p = ret;
      return S_OK;
   }
}