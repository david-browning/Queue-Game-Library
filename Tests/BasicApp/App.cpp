#include "pch.h"
#include "App.h"
#include "GameCallbacks.h"

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

IFrameworkView App::CreateView()
{
   return *this;
}

void App::Initialize(CoreApplicationView const& appView)
{
   appView.Activated({ this, &App::Activated });
   CoreApplication::Suspending({ this, &App::Suspending });
   CoreApplication::Resuming({ this, &App::Resuming });
}

void App::SetWindow(CoreWindow const& window)
{
   window.PointerPressed({ this, &App::OnPointerPressed });
   window.PointerMoved({ this, &App::OnPointerMoved });
   window.Closed({ this, &App::OnWindowClosed });
}

void App::Load(hstring const&)
{
   // Set up device resources.
}

void App::Uninitialize()
{
   // Called when the app is closed.
   return;
}

void App::Activated(IInspectable const&,
                    Activation::IActivatedEventArgs const&)
{
   CoreWindow window = CoreWindow::GetForCurrentThread();
   window.Activate();
   m_wndVisible = true;
}

void App::Run()
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

   auto shaderStager = stagers::make_shader_stager({
      &vShader,
      &pShader
   });





   std::list<std::unique_ptr<shaders::ivert_element>> geom3dElements;
   geom3dElements.push_back(std::make_unique<shaders::pos_velement>());
   geom3dElements.push_back(std::make_unique<shaders::norm_velement>());
   geom3dElements.push_back(std::make_unique<shaders::col_velement>());
   geom3dElements.push_back(std::make_unique<shaders::tex_velement>());

   auto geom3dVertStager = stagers::make_vert_stager(
      geom3dElements.begin(), geom3dElements.end(),
      0, 0);

   shaders::vertex_layout cubeVertLayout{
      geom3dVertStager,
      D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE,
      D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED
   };





   struct wvp
   {
      DirectX::XMFLOAT4X4 m;
      char padding[256 - sizeof(DirectX::XMFLOAT4X4)];
   };

   auto wvpBufferAllocInfo = helpers::buffer_alloc_info(
      sizeof(wvp), qdev_p->dev_3d(), 0);
   auto cameraAllocInfo = helpers::buffer_alloc_info(
      sizeof(qgl::components::CONST_CAMERA_BUFFER), qdev_p->dev_3d(), 0);
   auto biggerAlignment = std::max<size_t>(wvpBufferAllocInfo.alignment,
                                           cameraAllocInfo.alignment);
   auto alignedBuffSize = wvpBufferAllocInfo.bytes + cameraAllocInfo.bytes;

   gpu::copy_dst_allocator wvpAllocator{
      qdev_p.get(), alignedBuffSize, biggerAlignment };

   gpu::const_buffer<wvp> wvpBuffer{ &wvpAllocator };
   descriptors::camera_descriptor cameraDesc;
   cameraDesc.look = { {0,1}, {0,1}, {1, 1}, {1,1} };
   cameraDesc.up = { {0,1}, {1,1}, {0,1}, {1,1} };
   cameraDesc.position = { {0,1}, {1,2}, {-3,1}, {1,1} };
   components::camera sceneCamera{
      cameraDesc,
      static_cast<float>(qwp->width()) / static_cast<float>(qwp->height()),
      &wvpAllocator,
      &cameraCallback };

   
   gpu::cbv_descriptor_heap cbvHeap{ qdev_p->dev_3d(), 2 };
   cbvHeap.insert(qdev_p.get(), 0, wvpBuffer);
   cbvHeap.insert(qdev_p.get(), 1, sceneCamera);
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
      msmplr, cubeVertLayout, qdev_p.get(), 0
   };




   std::vector<gpu::geom3d_vert> cubeVerts =
   {
      { XMFLOAT3(-0.5f, -0.5f, -0.5f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },
      { XMFLOAT3(-0.5f, -0.5f,  0.5f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },
      { XMFLOAT3(-0.5f,  0.5f, -0.5f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },
      { XMFLOAT3(-0.5f,  0.5f,  0.5f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },
      { XMFLOAT3(0.5f,  -0.5f, -0.5f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },
      { XMFLOAT3(0.5f,  -0.5f,  0.5f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },
      { XMFLOAT3(0.5f,  0.5f,  -0.5f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },
      { XMFLOAT3(0.5f,  0.5f,   0.5f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT2(0.0f, 0.0f) },
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
      sizeof(cubeVerts[0]) * cubeVerts.size(),
      qdev_p->dev_3d(),
      0);
   auto iBufferAllocInfo = helpers::buffer_alloc_info(
      sizeof(cubeIndices[0]) * cubeIndices.size(),
      qdev_p->dev_3d(),
      0);
   auto geomAlignment = std::max<size_t>(vBufferAllocInfo.alignment,
                                         iBufferAllocInfo.alignment);
   auto geomBufferTotalSize = mem::align_address(
      vBufferAllocInfo.bytes + iBufferAllocInfo.bytes,
      geomAlignment);
   gpu::vert_allocator geomAllocator {
      qdev_p.get(),
      mem::align_address(vBufferAllocInfo.bytes + iBufferAllocInfo.bytes,
      vBufferAllocInfo.alignment),
      vBufferAllocInfo.alignment
   };

   qgl::components::model_mesh<gpu::geom3d_vert, uint16_t> cubeMesh{
      cubeVerts, cubeIndices, &geomAllocator, &cubeMeshCallback
   };

   // Upload the cube geometry.
   {
      const auto& geomStats = geomAllocator.stats();
      gpu::copy_dst_allocator uploadAllocator{
         qdev_p.get(), geomStats.committed, geomStats.alignment
      };

      gpu::upload_buffer uvBuffer{ &uploadAllocator, cubeMesh.vbuffer() };
      gpu::upload_buffer uiBuffer{ &uploadAllocator, cubeMesh.ibuffer() };

      gpu::cmd_executor<D3D12_COMMAND_LIST_TYPE_DIRECT> cpyExecutor{
         *qdev_p, D3D12_COMMAND_LIST_TYPE_DIRECT };
      gpu::upload_command_list cpyList{ *qdev_p, pso, 0, L"Copy List" };
      gpu::fence<uint64_t> cpyFence{ *qdev_p, cpyExecutor.get() };

      cpyExecutor.clear();
      cpyList.begin();
      cpyList.upload(cubeMesh.vbuffer(), uvBuffer);
      cpyList.transition(cubeMesh.vbuffer(), D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);
      cpyList.upload(cubeMesh.ibuffer(), uiBuffer);
      cpyList.transition(cubeMesh.ibuffer(), D3D12_RESOURCE_STATE_INDEX_BUFFER);
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



   std::array<float, 4> clearColor = { 0.0f, 0.2f, 0.6f, 1.0f };
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
         cmdList->vertices(cubeMesh.vbuffer());
         cmdList->indices(cubeMesh.ibuffer());
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