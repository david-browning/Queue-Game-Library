#include "pch.h"

using namespace winrt;
using namespace Windows;
using namespace Windows::ApplicationModel::Core;
using namespace Windows::Foundation::Numerics;
using namespace Windows::UI;
using namespace Windows::UI::Core;
using namespace Windows::UI::Composition;
using namespace qgl::graphics;
using namespace qgl::content;

struct App : implements<App, IFrameworkViewSource, IFrameworkView>
{
   std::shared_ptr<content_store<wchar_t, int64_t>> m_cstore_p;

   GRAPHICS_CONFIG_BUFFER graphicsConfigBuffer;
   std::shared_ptr<graphics_device> gDevice_p;
   std::shared_ptr<fence<UINT64>> syncFence_p;
   std::shared_ptr<window> window_p;
   std::vector<std::shared_ptr<frame>> frames;
   std::shared_ptr<rtv_descriptor_heap> rtvHeap;
   std::shared_ptr<dsv_descriptor_heap> dsvHeap;
   std::shared_ptr<render_engine<UINT64>> rEngine;

   std::shared_ptr<std::wstring> m_sampleTextString_p;
   std::shared_ptr<wtext> m_sampleText;
   std::shared_ptr<text_format> m_sampleTextFormat;
   std::shared_ptr<brush> m_sampleTextBrush;
   bool m_windowClosed;

   IFrameworkView CreateView()
   {
      return *this;
   }

   void Initialize(CoreApplicationView const&)
   {

   }

   void Load(hstring const&)
   {
      CoreWindow wnd = CoreWindow::GetForCurrentThread();
      ViewManagement::ApplicationView appView = ViewManagement::ApplicationView::GetForCurrentView();

      auto fldr = winrt::Windows::Storage::ApplicationData::Current().LocalFolder();
      content_store_config storeConfig(fldr.Path());
      m_cstore_p = std::make_shared<content_store<wchar_t, int64_t>>(storeConfig);

      m_windowClosed = false;
      wnd.Closed({ this, &App::on_window_closed });
      wnd.KeyDown({ this,  &App::on_key_down });
      wnd.SizeChanged({ this, &App::on_resize_complete });

      //Create a window
      auto coreWin_p = std::make_shared<CoreWindow>(wnd);
      auto appView_p = std::make_shared<ViewManagement::ApplicationView>(appView);
      window_p = std::make_shared<qgl::graphics::window>(appView_p, coreWin_p);

      //Create the graphics device.
      gDevice_p = std::make_shared<graphics_device>(graphicsConfigBuffer, window_p);

      //Load content
      m_sampleTextString_p = std::make_shared<std::wstring>(L"Sample Text");
      D2D_RECT_F textRectangle{ 0.0f, 0.0f, 30.0f, 100.0f };
      m_sampleText = std::make_shared<wtext>(m_sampleTextString_p,
                                             textRectangle,
                                             L"", 0);

      TEXT_FORMAT_BUFFER txtFmtBuffer;
      m_sampleTextFormat = std::make_shared<text_format>(txtFmtBuffer,
                                                         gDevice_p->com_write_factory(),
                                                         L"", 0);

      BRUSH_BUFFER brshBuffer;
      m_sampleTextBrush = std::make_shared<brush>(brshBuffer,
                                                  gDevice_p->com_d2d1_context(),
                                                  L"", 0);
   }

   void Uninitialize()
   {
   }

   void rebuild_frames()
   {
      //Need to release all render resources first.
      rEngine->release();

      //Reset the graphics device with the new configuration.
      gDevice_p->reset(graphicsConfigBuffer);

      //Rebuild the frames.
      DEPTH_STENCIL_BUFFER dsBuffer;
      make_frames<std::vector, frame>(*gDevice_p, *rtvHeap, dsBuffer, *dsvHeap, *window_p, frames);

      graphic_options opts(*gDevice_p);
      assert(opts.valid(graphicsConfigBuffer));
   }

   void gamma_up()
   {

   }

   void gamme_down()
   {

   }

   void on_key_down(IInspectable const&, KeyEventArgs const& arg)
   {
      switch (arg.VirtualKey())
      {
         case Windows::System::VirtualKey::H:
         {
            break;
         }
         case Windows::System::VirtualKey::Left:
         {
            break;
         }
         case Windows::System::VirtualKey::Right:
         {
            break;
         }
         case Windows::System::VirtualKey::Up:
         {
            gamma_up();
            break;
         }
         case Windows::System::VirtualKey::Down:
         {
            gamme_down();
            break;
         }
      }
   }

   void on_window_closed(IInspectable const&, CoreWindowEventArgs const&)
   {
      m_windowClosed = true;
   }

   void on_resize_complete(IInspectable const&, WindowSizeChangedEventArgs const&)
   {
      rebuild_frames();
   }

   void Run()
   {
      CoreWindow wnd = CoreWindow::GetForCurrentThread();
      wnd.Activate();

      //Create an inter-engine synchronization fence.
      syncFence_p = std::make_shared<fence<UINT64>>(gDevice_p->com_d3d12_device(),
                                                    gDevice_p->com_cmd_queue());

      //Create the depth stencil buffer, depth stencil view heap, and render target view 
      //heap.
      dsvHeap = std::make_shared<dsv_descriptor_heap>(gDevice_p->com_d3d12_device(),
                                                      graphicsConfigBuffer.buffers(),
                                                      D3D12_DESCRIPTOR_HEAP_FLAG_NONE);
      rtvHeap = std::make_shared<rtv_descriptor_heap>(gDevice_p->com_d3d12_device(),
                                                      graphicsConfigBuffer.buffers(),
                                                      D3D12_DESCRIPTOR_HEAP_FLAG_NONE);

      //Create the frames
      frames = make_frames<std::vector, frame>(*gDevice_p,
                                               *rtvHeap,
                                               DEPTH_STENCIL_BUFFER(),
                                               *dsvHeap,
                                               *window_p);

      //Create SRV descriptor heap and table.
      srv_descriptor_heap srvHeap(gDevice_p->com_d3d12_device(),
                                  1,
                                  D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE,
                                  0);
      auto srvTable = srvHeap.table(0, 1, 0, 0, D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC);

      //Create sampler descriptor heap and table.
      sampler_descriptor_heap samplerHeap(gDevice_p->com_d3d12_device(),
                                          1,
                                          D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE,
                                          0);
      auto smplrTable = samplerHeap.table(0,
                                          1,
                                          srvTable.root_index() + 1,
                                          0,
                                          D3D12_DESCRIPTOR_RANGE_FLAG_NONE);

      //Create the root signature, passing the descriptor tables.
      root_signature rootSig(gDevice_p->com_d3d12_device(), { &srvTable, &smplrTable }, 0);

      //Load the shaders
      ishader<SHADER_TYPES::SHADER_TYPE_VERTEX> vsShader(L"vs.cso");
      ishader<SHADER_TYPES::SHADER_TYPE_PIXEL> psShader(L"ps.cso");

      //Create the pipeline state
      blender bldr;
      rasterizer rstzr;
      pipeline_state ps(gDevice_p->com_d3d12_device(), rootSig);
      ps.blend_state(bldr);
      ps.rasterizer_state(rstzr);
      ps.sample_description();
      ps.frames(frames.begin(), frames.end());
      ps.shader(vsShader);
      ps.shader(psShader);

      std::initializer_list<D3D12_INPUT_ELEMENT_DESC> elementDesc =
      {
         { "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
         { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
      };

      ps.layout(elementDesc, D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);

      //Create the render engine.
      rEngine = std::make_shared<render_engine<UINT64>>(gDevice_p, frames.begin(), frames.end(), syncFence_p, ps, 0);

      text_render_component txtRComponent(m_sampleTextBrush, m_sampleTextFormat);
      float clearColor[4] = { 1.0f, 0.0f, 1.0f, 0.0f };
      rEngine->clear_color(clearColor);
      rebuild_frames();

      CoreDispatcher dispatcher = wnd.Dispatcher();
      qgl::timer<float, int64_t> gameTimer(qgl::timer<float, int64_t>::TICK_120_HZ);
      while (!m_windowClosed)
      {
         dispatcher.ProcessEvents(CoreProcessEventsOption::ProcessAllIfPresent);
         gameTimer.tick();

         std::wstringstream fpsStream;
         fpsStream << L"Updates per Second: " << gameTimer.fps();
         *m_sampleTextString_p = fpsStream.str();

         rEngine->begin();
         rEngine->clear_frames();
         auto d2dContext_p = gDevice_p->d2d1_context();
         txtRComponent.update(d2dContext_p, *m_sampleText);
         auto sync = rEngine->flush();
         rEngine->present();
         rEngine->wait(sync);
      }
   }

   void SetWindow(CoreWindow const& window)
   {

   }
};

int __stdcall wWinMain(HINSTANCE, HINSTANCE, PWSTR, int)
{
   CoreApplication::Run(make<App>());
}
