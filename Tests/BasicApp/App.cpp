#include "pch.h"

using namespace winrt;
using namespace winrt::Windows;
using namespace winrt::Windows::ApplicationModel::Core;
using namespace winrt::Windows::Foundation::Numerics;
using namespace winrt::Windows::UI;
using namespace winrt::Windows::UI::Core;
using namespace winrt::Windows::UI::Composition;
using namespace winrt::Windows::UI::ViewManagement;

using namespace qgl;

void consoleCallback(const std::string& s)
{
   std::cout << s << std::endl;
   return;
}

struct App : implements<App, IFrameworkViewSource, IFrameworkView>
{
   bool m_wndClosed = false;
   qgl::basic_console<char> m_console;

   IFrameworkView CreateView()
   {
      return *this;
   }

   void Initialize(CoreApplicationView const&)
   {
   }

   void Load(hstring const&)
   {
   }

   void Uninitialize()
   {
   }

   void Run()
   {
      auto outCon_p = std::make_shared<qgl::console>();
      outCon_p->insert_cout(consoleCallback);
      error_reporter eReporter{ outCon_p };


      CoreWindow window = CoreWindow::GetForCurrentThread();
      window.Activate();

      ApplicationView view{ ApplicationView::GetForCurrentView() };

      auto w_p = std::make_shared<CoreWindow>(window);
      auto v_p = std::make_shared<ApplicationView>(view);
      auto qwp = std::make_shared<graphics::window>(w_p, v_p);

      descriptors::engine_descriptor eDesc;
      eDesc.console = true;
      descriptors::hdr_descriptor hdrDesc;
      graphics::gpu_config defaultConfig{ eDesc, hdrDesc };
      auto qdev_p = std::make_shared<graphics::graphics_device>(
         defaultConfig, qwp, std::initializer_list<decltype(outCon_p)>{outCon_p});

      // Create descriptor heaps
      graphics::gpu::rtv_descriptor_heap rtvHeap{ qdev_p->dev_3d(), eDesc.buffers };
      graphics::gpu::dsv_descriptor_heap dsvHeap{ qdev_p->dev_3d(), eDesc.buffers };

      // Create PSO objects
      // Set clear color!
      auto dsvAlloc_p = std::make_shared<graphics::gpu::committed_allocator>(
         qdev_p.get(),
         CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
         D3D12_HEAP_FLAG_NONE);

      // Create RTVs
      std::vector<std::unique_ptr<graphics::gpu::frame>> frame_ps;
      for (size_t i = 0; i < eDesc.buffers; i++)
      {
         graphics::gpu::depth_stencil dpsnc_p{
            descriptors::depth_stencil_descriptor{},
            dsvAlloc_p.get(),
            qdev_p.get(),
            dsvHeap,
            i };

         graphics::gpu::render_target rt{
            qdev_p.get(),
            rtvHeap,
            i };

         graphics::gpu::viewport viewp{ *qdev_p };
         graphics::gpu::scissor scisr{ viewp };
         graphics::gpu::blender blndr { descriptors::blender_descriptor{} };
         graphics::gpu::rasterizer rster{ descriptors::rasterizer_descriptor{} };
         
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

      graphics::shaders::shader_descriptor vsDesc;
      vsDesc.payload = graphics::shaders::shader_payloads::compiled;
      vsDesc.type = graphics::shaders::shader_types::vs;
      vsDesc.vendor = graphics::shaders::shader_vendors::directx;
      vsDesc.compile_params.type = vsDesc.type;
      vsDesc.compile_params.shader_model = 5;
      vsDesc.compile_params.effect_type = 0;
      vsDesc.compile_params.entry.copy("VSMain", 7);
      vsDesc.compile_params.flags = compileFlags;

      graphics::shaders::shader_descriptor psDesc;
      psDesc.payload = graphics::shaders::shader_payloads::compiled;
      psDesc.type = graphics::shaders::shader_types::ps;
      psDesc.vendor = graphics::shaders::shader_vendors::directx;
      psDesc.compile_params.type = psDesc.type;
      psDesc.compile_params.shader_model = 5;
      psDesc.compile_params.effect_type = 0;
      psDesc.compile_params.entry.copy("PSMain", 7);
      psDesc.compile_params.flags = compileFlags;

      graphics::shaders::shader vShader{
         vsDesc, 
         shaderFile
      };

      graphics::shaders::shader pShader{
         psDesc, 
         shaderFile 
      };

      graphics::shaders::shader_metadata vsMeta{ vShader };
      graphics::shaders::shader_metadata psMeta{ pShader };

      auto shaderStager = stagers::make_shader_stager({
         &vShader,
         &pShader
      });

      graphics::shaders::vertex_layout vLayout{
         vsMeta,
         D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE,
         D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED,
         0,
         D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA
      };

      // Create root signature
      //auto rootSignBindings = graphics::gpu::make_ibindable_stager({

      //});
      stagers::ibindable_stager rootSignBindings;

      graphics::gpu::root_signature rootSign{
         rootSignBindings,
         *qdev_p
      };

      graphics::multisampler msmplr{ 
         descriptors::multisampler_descriptor{} };

      graphics::gpu::graphics_pso pso{ rootSign, frameStager, shaderStager,
         msmplr, vLayout, qdev_p.get(), 0};

      descriptors::text_format_descriptor txtDesc;
      descriptors::screen_space_descriptor txtLayout;
      txtLayout.space(descriptors::screen_spaces::relative);
      txtLayout.rect = descriptors::vector_descriptor{
         {1, 2},
         {1, 2},
         {1, 2},
         {1, 2}
      };

      graphics::gpu::graphics_command_list cmdList{ *qdev_p, pso };

      components::wtext txt{ 
         L"Test", 
         txtLayout,
         qgl::components::success_component_functor<components::wtext>};

      graphics::shaders::shader_lib_descriptor slibDesc;
      slibDesc.payload = graphics::shaders::shader_payloads::source;
      slibDesc.vendor = graphics::shaders::shader_vendors::directx;
      slibDesc.compile_params.type = graphics::shaders::shader_types::lib;
      slibDesc.compile_params.name.copy("ShaderLibrary", 14);
      std::wstring shaderLibFile = std::wstring(path) + 
         std::wstring(L"Assets\\Common.hlsli");
      graphics::shaders::shader_lib shaderLib{ 
         slibDesc, shaderLibFile, &eReporter };


      size_t frmIdx = 0;
      CoreDispatcher dispatcher = window.Dispatcher();
      qgl::timer<int64_t> t;
      auto fps = t.fps();
      while (!m_wndClosed)
      {
         dispatcher.ProcessEvents(
            CoreProcessEventsOption::ProcessAllIfPresent);
         t.tick();
         auto tState = t.state();
         fps = t.fps();

         // Reset cmd allocator

         // Reset cmd list

         // Set root signature

         // Set viewport

         // Set scissors

         // Indicate that the back buffer will be used as a render target.

         // Record rendering commands







         // Close the command list

         // Acquire wrapped resources

         // Render D2D content

         // Release wrapped resources

         // Flush D3D11 context

         // Present the swapchain.

         // Move to next frame.
      }

      return;
   }

   void SetWindow(CoreWindow const& window)
   {
      window.PointerPressed({ this, &App::OnPointerPressed });
      window.PointerMoved({ this, &App::OnPointerMoved });
      window.Closed({ this, &App::OnWindowClosed });

      window.PointerReleased([&](auto && ...)
      {
      });
   }

   void OnPointerPressed(IInspectable const&, PointerEventArgs const& args)
   {
      float2 const point = args.CurrentPoint().Position();
   }

   void OnPointerMoved(IInspectable const&, PointerEventArgs const&)
   {

   }

   void OnWindowClosed(CoreWindow const&, CoreWindowEventArgs const&)
   {
      m_wndClosed = true;
   }
};

int __stdcall wWinMain(HINSTANCE, HINSTANCE, PWSTR, int)
{
   CoreApplication::Run(make<App>());
}
