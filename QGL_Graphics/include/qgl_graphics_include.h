#pragma once
// https://devblogs.microsoft.com/directx/gettingstarted-dx12agility/
//extern "C" { __declspec(dllexport) extern const UINT D3D12SDKVersion = 600; }
//
//extern "C" { __declspec(dllexport) extern const char* D3D12SDKPath = u8".\\D3D12\\"; }

#include <Unknwn.h>

#include <QGLModel.h>
#include <QGLMath.h>

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <winrt/base.h>
#include "winrt/Windows.ApplicationModel.Core.h"
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.UI.Core.h>
#include <winrt/Windows.UI.ViewManagement.Core.h>
#include <winrt/Windows.Graphics.Display.h>

#pragma warning(push)
#pragma warning(disable : 26812)
#include <d3d12.h>
#include <d2d1_3.h>
#include <dxgi1_6.h>
#include <d3d11on12.h>
#include <dwrite_3.h>
#include <DirectXMath.h>
#include <DirectXCollision.h>
#include <DirectXTex.h>
#include <d2d1_1helper.h>
#include <d2d1_3helper.h>
#include "include/Helpers/d3dx12.h"
#include <d3d12shader.h>
#include <d3dcompiler.h>
#pragma warning(pop)

#ifdef DEBUG
#include <D3d12SDKLayers.h>
#endif

#ifdef QGL_GRAPHICS_EXPORTS
#define QGL_GRAPHICS_API __declspec(dllexport)
#define QGL_GRAPHICS_TEMPLATE
#else
#define QGL_GRAPHICS_API __declspec(dllimport)
#define QGL_GRAPHICS_TEMPLATE extern
#endif

namespace qgl::graphics
{
   //Assign a name to the object to aid with debugging.
#ifdef DEBUG
   inline void name_d3d(ID3D12Object* pObject, LPCWSTR name)
   {
      pObject->SetName(name);
   }

   inline void name_d3d_indexed(ID3D12Object* pObject, LPCWSTR name, UINT index)
   {
      WCHAR fullName[50];
      if (swprintf_s(fullName, L"%s[%u]", name, index) > 0)
      {
         pObject->SetName(fullName);
      }
   }
#else
   inline void name_d3d(ID3D12Object*, LPCWSTR)
   {
   }

   inline void name_d3d_indexed(ID3D12Object*, LPCWSTR, UINT)
   {
   }

#endif

   constexpr UINT CalculateConstantBufferByteSize(UINT byteSize)
   {
      // Constant buffer size is required to be aligned.
      return (byteSize + (D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT - 1)) &
         ~(D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT - 1);
   }

   using igpu_factory = typename IDXGIFactory6;
   using i2d_factory = typename ID2D1Factory7;
   using itext_factory = typename IDWriteFactory7;

   using igpu_adapter = typename IDXGIAdapter4;
   using igpu_output = typename IDXGIOutput6;

   using i3d_device = typename ID3D12Device3;
   using iswap_chain = typename IDXGISwapChain4;
   using i3d_context = typename ID3D11DeviceContext;
   using i2d_device = typename ID2D1Device6;
   using i2d_context = typename ID2D1DeviceContext6;

   using icmd_list = typename ID3D12GraphicsCommandList3;
   using icmd_allocator = typename ID3D12CommandAllocator;
   using icmd_queue = typename ID3D12CommandQueue;

   using igpu_resource = typename ID3D12Resource1;
   using igpu_heap = typename ID3D12Heap1;
   using igpu_fence = typename ID3D12Fence1;

   using i3d_bridge_device = typename ID3D11On12Device;
   using i3d_render_target = typename igpu_resource;
   using i2d_render_target = typename ID2D1Bitmap1;
   using i3d_bridge_render_target = typename ID3D11Resource;

   /*
    Device independent pixel.
    */
   using dip_t = typename float;

   /*
    Physical pixel.
    This must be an integer type so it can be serialized.
    */
   using physp_t = typename uint32_t;

   /*
    Dots per inch.
    */
   using dpi_t = typename dip_t;

   /*
    D3D12_INDEX_BUFFER_STRIP_CUT_VALUE
    When using triangle strip primitive topology, vertex positions are 
    interpreted as vertices of a continuous triangle “strip”. There is a 
    special index value that represents the desire to have a discontinuity in 
    the strip, the cut index value.
    */
   using strip_cut_t = typename uint8_t;

   constexpr D3D12_INDEX_BUFFER_STRIP_CUT_VALUE to_d3d_strip_cut(strip_cut_t s)
   {
      return static_cast<D3D12_INDEX_BUFFER_STRIP_CUT_VALUE>(s);
   }

   /*
    D3D12_PRIMITIVE_TOPOLOGY_TYPE
    Specifies how the pipeline interprets geometry or hull shader input 
    primitives.
    */
   using topology_t = typename uint8_t;

   constexpr D3D12_PRIMITIVE_TOPOLOGY_TYPE to_d3d_topo(topology_t t)
   {
      return static_cast<D3D12_PRIMITIVE_TOPOLOGY_TYPE>(t);
   }

   /*
    UINT
    Data enters the IA stage through inputs called input slots.
    The IA stage has n input slots, which are designed to accommodate up to
    n vertex buffers that provide input data. Each vertex buffer must be
    assigned to a different slot; this information is stored in the
    input-layout declaration when the input-layout object is created.
    You may also specify an offset from the start of each buffer to the
    first element in the buffer to be read.
    */
   using input_slot_t = typename uint8_t;

   constexpr UINT to_d3d_input_slot(input_slot_t i)
   {
      return static_cast<UINT>(i);
   }

   /*
    D3D12_INPUT_CLASSIFICATION
    A value that identifies the input data class for a single input slot.
    */
   using input_data_class_t = typename uint8_t;

   constexpr D3D12_INPUT_CLASSIFICATION to_d3d_input_class(
      input_data_class_t c)
   {
      return static_cast<D3D12_INPUT_CLASSIFICATION>(c);
   }
}