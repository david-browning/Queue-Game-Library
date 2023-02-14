#include "pch.h"

using namespace winrt;
using namespace winrt::Windows;
using namespace winrt::Windows::ApplicationModel::Core;
using namespace winrt::Windows::Foundation::Numerics;
using namespace winrt::Windows::UI;
using namespace winrt::Windows::UI::Core;
using namespace winrt::Windows::UI::Composition;
using namespace winrt::Windows::UI::ViewManagement;
using namespace winrt::Windows::ApplicationModel;

using namespace qgl;
using namespace qgl::graphics;
using namespace DirectX;

void consoleCallback(const std::string&)
{

}

void dsvAllocCallback(qgl::graphics::gpu::igpu_allocator* alloc_p)
{
   auto& info = alloc_p->stats();
}

qgl::result_t cameraCallback(qgl::components::camera&,
                             qgl::components::game_context&)
{
   return S_OK;
}

struct App : implements<App, IFrameworkViewSource, IFrameworkView>
{
   bool m_wndClosed = false;
   bool m_wndVisible = true;
   qgl::basic_console<char> m_console;
   std::unique_ptr<graphics_device> qdev_p;

   IFrameworkView CreateView()
   {
      return *this;
   }

   void Initialize(CoreApplicationView const& appView)
   {
      appView.Activated({ this, &App::Activated });
      CoreApplication::Suspending({ this, &App::Suspending });
      CoreApplication::Resuming({ this, &App::Resuming });
   }

   void SetWindow(CoreWindow const& window)
   {
      window.PointerPressed({ this, &App::OnPointerPressed });
      window.PointerMoved({ this, &App::OnPointerMoved });
      window.Closed({ this, &App::OnWindowClosed });
   }

   void Load(hstring const&)
   {
      // Set up device resources.
   }

   void Uninitialize()
   {
      // Called when the app is closed.
      return;
   }

   void Run()
   {
      qgl::console outCon;
      outCon.insert_cout(consoleCallback);
      error_reporter eReporter{ {&outCon} };

      CoreWindow window = CoreWindow::GetForCurrentThread();
      ApplicationView view{ ApplicationView::GetForCurrentView() };

      auto w_p = std::make_shared<CoreWindow>(window);
      auto v_p = std::make_shared<ApplicationView>(view);
      auto qwp = std::make_shared<graphics::window>(w_p, v_p);

      descriptors::engine_descriptor eDesc;
      eDesc.console = true;
      eDesc.tearing = 1;
      eDesc.refresh = 244;
      //eDesc.fullscreen = 1;
      descriptors::hdr_descriptor hdrDesc;
      gpu_config defaultConfig{ eDesc, hdrDesc };
      qdev_p = std::make_unique<graphics_device>(defaultConfig, qwp);
      qdev_p->message_dispatcher().register_callback(&consoleCallback);

      descriptors::camera_descriptor cameraDesc;
      cameraDesc.look = { {0,1}, {0,1}, {1, 1}, {1,1} };
      cameraDesc.up = { {0,1}, {1,1}, {0,1}, {1,1} };
      cameraDesc.position = { {0,1}, {1,2}, {-3,1}, {1,1} };

      // Create descriptor heaps
      gpu::rtv_descriptor_heap rtvHeap{ qdev_p->dev_3d(), eDesc.buffers };
      gpu::dsv_descriptor_heap dsvHeap{ qdev_p->dev_3d(), eDesc.buffers };

      descriptors::depth_stencil_descriptor dsvDesc;
      D3D12_CLEAR_VALUE dsvClear;
      dsvClear.Format = DXGI_FORMAT_D32_FLOAT;
      dsvClear.DepthStencil.Depth = dsvDesc.depth.operator float();
      dsvClear.DepthStencil.Stencil = dsvDesc.stencil;
      auto dsvAlloc_p = std::make_shared<graphics::gpu::tex2d_allocator>(
         qdev_p.get(), 24 * 1024 * 1024, 0, dsvClear);
      dsvAlloc_p->register_callback(&dsvAllocCallback);

      // Create RTVs
      std::vector<std::unique_ptr<gpu::frame>> frame_ps;
      for (size_t i = 0; i < eDesc.buffers; i++)
      {
         gpu::depth_stencil dpsnc_p{
            dsvDesc,
            dsvAlloc_p.get(),
            qdev_p.get(),
            dsvHeap,
            i };

         gpu::render_target rt{ qdev_p.get(), rtvHeap, i };
         gpu::viewport viewp{ *qdev_p };
         gpu::scissor scisr{ viewp };
         gpu::blender blndr{ descriptors::blender_descriptor{} };
         gpu::rasterizer rster{ descriptors::rasterizer_descriptor{} };

         auto f_p = std::make_unique<graphics::gpu::frame>(
            std::move(rt),
            std::move(scisr),
            std::move(viewp),
            std::move(rster),
            std::move(blndr),
            std::move(dpsnc_p));

         frame_ps.push_back(std::move(f_p));
      }

      auto frameStager = stagers::make_frame_stager(frame_ps.begin(),
                                                    frame_ps.end());

      WCHAR path[512];
      UINT pathSize = 0;
      DWORD size = GetModuleFileName(nullptr, path, 512);
      if (size == 0 || size == pathSize)
      {
         // Method failed or path was truncated.
         throw std::exception();
      }

      WCHAR* lastSlash = wcsrchr(path, L'\\');
      if (lastSlash)
      {
         *(lastSlash + 1) = L'\0';
      }

      std::wstring shaderFile = std::wstring(path) +
         std::wstring(L"Assets\\shaders.hlsl");
      UINT compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;

      shaders::shader_descriptor vsDesc;
      vsDesc.payload = shaders::shader_payloads::compiled;
      vsDesc.type = shaders::shader_types::vs;
      vsDesc.vendor = shaders::shader_vendors::directx;
      vsDesc.compile_params.type = vsDesc.type;
      vsDesc.compile_params.shader_model = 5;
      vsDesc.compile_params.effect_type = 0;
      vsDesc.compile_params.entry.copy("VSMain", 7);
      vsDesc.compile_params.flags = compileFlags;

      shaders::shader_descriptor psDesc;
      psDesc.payload = shaders::shader_payloads::compiled;
      psDesc.type = shaders::shader_types::ps;
      psDesc.vendor = shaders::shader_vendors::directx;
      psDesc.compile_params.type = psDesc.type;
      psDesc.compile_params.shader_model = 5;
      psDesc.compile_params.effect_type = 0;
      psDesc.compile_params.entry.copy("PSMain", 7);
      psDesc.compile_params.flags = compileFlags;

      shaders::shader_buffer vShader{
         vsDesc,
         shaderFile
      };

      shaders::shader_buffer pShader{
         psDesc,
         shaderFile
      };

      shaders::shader_metadata vsMeta{ vShader };
      shaders::shader_metadata psMeta{ pShader };

      auto shaderStager = stagers::make_shader_stager({
         &vShader,
         &pShader
      });

      shaders::vertex_layout vLayout{
         vsMeta,
         D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE,
         D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED,
         0,
         D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA
      };

      struct wvp
      {
         DirectX::XMFLOAT4X4 m;
         char padding[256 - sizeof(DirectX::XMFLOAT4X4)];
      };

      auto wvpBufferAllocInfo = helpers::buffer_alloc_info(
         sizeof(wvp), qdev_p->dev_3d(), 0);
      auto cameraAllocInfo = helpers::buffer_alloc_info(
         sizeof(qgl::components::camera), qdev_p->dev_3d(), 0);
      auto biggerAlignment = std::max<size_t>(wvpBufferAllocInfo.alignment,
                                              cameraAllocInfo.alignment);
      auto alignedBuffSize = wvpBufferAllocInfo.bytes + cameraAllocInfo.bytes;

      gpu::copy_dst_allocator wvpAllocator{
         qdev_p.get(), alignedBuffSize, biggerAlignment };
      gpu::const_buffer<wvp> wvpBuffer{ &wvpAllocator };
      components::camera sceneCamera {
         cameraDesc,
         static_cast<float>(qwp->width()) / static_cast<float>(qwp->height()),
         &wvpAllocator, 
         &cameraCallback };

      gpu::cbv_descriptor_heap cbvHeap{ qdev_p->dev_3d(), 1 };
      cbvHeap.insert(qdev_p.get(), 0, wvpBuffer);
      auto cbvTable = cbvHeap.table(0, 1, 0, 0, D3D12_DESCRIPTOR_RANGE_FLAG_NONE);

      // Create root signature
      auto rootSignBindings = stagers::make_ibindable_stager({
         &cbvTable
      });

      graphics::gpu::root_signature rootSign{
         rootSignBindings,
         *qdev_p
      };

      graphics::multisampler msmplr{ descriptors::multisampler_descriptor{} };
      graphics::gpu::graphics_pso pso{ rootSign, frameStager, shaderStager,
         msmplr, vLayout, qdev_p.get(), 0 };

      shaders::shader_lib_descriptor slibDesc;
      slibDesc.payload = shaders::shader_payloads::source;
      slibDesc.vendor = shaders::shader_vendors::directx;
      slibDesc.compile_params.type = shaders::shader_types::lib;
      slibDesc.compile_params.name.copy("ShaderLibrary", 14);
      std::wstring shaderLibFile = std::wstring(path) +
         std::wstring(L"Assets\\Common.hlsli");
      graphics::shaders::shader_lib shaderLib{
         slibDesc, shaderLibFile, &eReporter };


      fixed_buffer<float, 4> clearColor;
      clearColor[0] = 0.0f;
      clearColor[1] = 0.2f;
      clearColor[2] = 0.6f;
      clearColor[3] = 1.0f;

      size_t frmIdx = qdev_p->buffer_idx();
      gpu::cmd_executor<D3D12_COMMAND_LIST_TYPE_DIRECT> executor{ qdev_p->default_queue() };
      std::vector<std::unique_ptr<gpu::graphics_command_list>> cmdLists;
      gpu::fence<uint64_t> syncFence{ *qdev_p, qdev_p->default_queue() };
      std::vector<gpu::sync_object<uint64_t>> frameFences;

      for (size_t i = 0; i < eDesc.buffers; i++)
      {
         frameFences.push_back(syncFence.current());
         cmdLists.push_back(std::make_unique<gpu::graphics_command_list>(
            *qdev_p, pso, 0, nullptr));
      }

      std::vector<D3D12_RECT> frameScissor{
         D3D12_RECT{
            0, 0,
            static_cast<long>(qwp->width() / 2),
            static_cast<long>(qwp->height() / 2)
         }
      };
      frame_ps[0]->frame_stencil().rectangles(
         frameScissor.begin(), frameScissor.end());

      std::vector<gpu::geom2d_vert> cube =
      {
         {XMFLOAT4(-0.5f, -0.5f, -0.5f, 1.0f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f)},
         {XMFLOAT4(-0.5f, -0.5f,  0.5f, 1.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f)},
         {XMFLOAT4(-0.5f,  0.5f, -0.5f, 1.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)},
         {XMFLOAT4(-0.5f,  0.5f,  0.5f, 1.0f), XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f)},
         {XMFLOAT4(0.5f, -0.5f, -0.5f, 1.0f),  XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)},
         {XMFLOAT4(0.5f, -0.5f,  0.5f, 1.0f),  XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f)},
         {XMFLOAT4(0.5f,  0.5f, -0.5f, 1.0f),  XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f)},
         {XMFLOAT4(0.5f,  0.5f,  0.5f, 1.0f),  XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f)},
      };

      std::vector<unsigned short> cubeIndices =
      {
         0,2,1, // -x
         1,2,3,

         4,5,6, // +x
         5,7,6,

         0,1,5, // -y
         0,5,4,

         2,6,7, // +y
         2,7,3,

         0,4,6, // -z
         0,6,2,

         1,3,7, // +z
         1,7,5,
      };

      auto vBufferAllocInfo = helpers::buffer_alloc_info(
         sizeof(cube[0]) * cube.size(),
         qdev_p->dev_3d(),
         0);

      auto iBufferAllocInfo = helpers::buffer_alloc_info(
         sizeof(cubeIndices[0]) * cubeIndices.size(),
         qdev_p->dev_3d(),
         0);

      gpu::vert_allocator vbufferAllocator{
         qdev_p.get(),
         mem::align_address(vBufferAllocInfo.bytes + iBufferAllocInfo.bytes,
            vBufferAllocInfo.alignment),
         vBufferAllocInfo.alignment
      };

      gpu::vertex_buffer<qgl::graphics::gpu::geom2d_vert, D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST> vBuffer{
         &vbufferAllocator, cube
      };


      gpu::index_buffer<unsigned short> iBuffer{
         &vbufferAllocator, cubeIndices
      };

      // Upload the vertex and index buffer.
      {
         gpu::copy_dst_allocator uploadAllocator{
            qdev_p.get(),
            mem::align_address(vBufferAllocInfo.bytes + iBufferAllocInfo.bytes,
               vBufferAllocInfo.alignment),
            vBufferAllocInfo.alignment
         };

         gpu::upload_buffer uvBuffer{ &uploadAllocator, vBuffer };
         gpu::upload_buffer uiBuffer{ &uploadAllocator, iBuffer };

         gpu::cmd_executor<D3D12_COMMAND_LIST_TYPE_DIRECT> cpyExecutor{ *qdev_p, D3D12_COMMAND_LIST_TYPE_DIRECT };
         gpu::upload_command_list cpyList{ *qdev_p, pso, 0, L"Copy List" };
         gpu::fence<uint64_t> cpyFence{ *qdev_p, cpyExecutor.get() };

         cpyExecutor.clear();
         cpyList.begin();
         cpyList.upload(vBuffer, uvBuffer);
         cpyList.transition(vBuffer, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);
         cpyList.upload(iBuffer, uiBuffer);
         cpyList.transition(iBuffer, D3D12_RESOURCE_STATE_INDEX_BUFFER);
         cpyList.end();
         cpyExecutor.queue(&cpyList);
         cpyExecutor.execute();

         auto waitFor = cpyFence.next();
         cpyFence.signal(waitFor);

         // Do not advance the fence value or the copy fence's destructor will
         // wait for a value that will never be signaled.
         //cpyFence.next();
         cpyFence.wait(waitFor);
      }

      qgl::timer<int64_t> t{ TICKS_PER_SECOND / eDesc.refresh };
      while (!m_wndClosed)
      {
         if (m_wndVisible)
         {
            CoreWindow::GetForCurrentThread().Dispatcher().ProcessEvents(
               CoreProcessEventsOption::ProcessAllIfPresent);

            t.tick();
            auto tState = t.state();
            qwp->title(std::to_wstring(tState.fps()));
            clearColor[1] = (((int)(clearColor[1] * 255) + 1) % 255) / 255.0;

            auto cameraMatrix = XMMatrixTranspose(
               DirectX::XMMatrixRotationY(45) *
               sceneCamera.mapping()->view *
               sceneCamera.mapping()->projection);

            wvpBuffer.map();
            DirectX::XMStoreFloat4x4(&wvpBuffer.mapping()->m,
                                     cameraMatrix);
            wvpBuffer.unmap();

            auto cmdList = cmdLists[frmIdx].get();
            executor.clear();
            cmdList->begin();

            cmdList->root_sig(rootSign);
            cmdList->frame_buffer(*frame_ps[frmIdx]);
            cmdList->clear_color(clearColor);
            cmdList->clear_frame();
            cmdList->clear_depth();
            cmdList->descriptors({ &cbvHeap });
            cmdList->table(cbvTable);
            cmdList->vertices(vBuffer);
            cmdList->indices(iBuffer);
            cmdList->idraw(1000);
            cmdList->end();

            executor.queue(cmdList);
            executor.execute();

            qdev_p->present();

            // Done recording commands for the current frame.
            // Tell anyone waiting for this frame that its done.
            syncFence.signal(syncFence.current());

            // Advance the frame index.
            frmIdx = qdev_p->buffer_idx();

            // Wait until the next frame is done processing.
            syncFence.wait(frameFences[frmIdx]);

            // Increment the current frame's sync value. We will wait on this
            // next time we render this frame.
            frameFences[frmIdx] = syncFence.next();
         }
         else
         {
            CoreWindow::GetForCurrentThread().Dispatcher().ProcessEvents(
               CoreProcessEventsOption::ProcessOneAndAllPending);
         }
      }
   }

   void Activated(IInspectable const&,
                  Activation::IActivatedEventArgs const&)
   {
      CoreWindow window = CoreWindow::GetForCurrentThread();
      window.Activate();
      m_wndVisible = true;
   }

   winrt::fire_and_forget Suspending(IInspectable const&,
                                     SuspendingEventArgs const& args)
   {
      auto lifetime = get_strong();

      // https://learn.microsoft.com/en-us/windows/uwp/gaming/how-to-suspend-an-app-directx-and-cpp
      auto def = args.SuspendingOperation().GetDeferral();

      co_await winrt::resume_background();
      //qdev_p->suspend();
      def.Complete();
   }

   void Resuming(IInspectable const&, IInspectable const&)
   {
      // https://learn.microsoft.com/en-us/windows/uwp/gaming/how-to-resume-an-app-directx-and-cpp
      return;
   }

   void OnPointerPressed(IInspectable const&, PointerEventArgs const& args)
   {
      float2 const point = args.CurrentPoint().Position();
      return;
   }

   void OnPointerMoved(IInspectable const&, PointerEventArgs const&)
   {

   }

   void OnWindowClosed(CoreWindow const&, CoreWindowEventArgs const&)
   {
   }
};

int __stdcall wWinMain(HINSTANCE, HINSTANCE, PWSTR, int)
{
   CoreApplication::Run(make<App>());
}
