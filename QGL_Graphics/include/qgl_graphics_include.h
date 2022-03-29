#pragma once
#include <Unknwn.h>

#include <QGLModel.h>
#include <QGLMath.h>

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <d3d12.h>
#include <d2d1_3.h>
#include <dxgi1_6.h>
#include <d3d11on12.h>
#include <dwrite_3.h>
#include <DirectXMath.h>
#include <DirectXCollision.h>
#include "include/Helpers/d3dx12.h"

#include <winrt/base.h>
#include "winrt/Windows.ApplicationModel.Core.h"
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.UI.Core.h>
#include <winrt/Windows.UI.ViewManagement.Core.h>
#include <winrt/Windows.Graphics.Display.h>

#ifdef _DEBUG
#include <D3d12SDKLayers.h>
#endif

#ifdef _DEBUG
#define D3DCOMPILE_DEBUG 1
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
#if defined(_DEBUG) || defined(DBG)
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

   // Naming helper for ComPtr<T>.
   // Assigns the name of the variable as the name of the object.
   // The indexed variant will include the index in the name of the object.
#define NAME_D3D12_OBJECT(x) (name_d3d(((ID3D12Object*)(x)), L#x))
#define NAME_D3D12_OBJECT_INDEXED(x, n) name_d3d_indexed((x)[n].Get(), L#x, n)

   constexpr UINT CalculateConstantBufferByteSize(UINT byteSize)
   {
      // Constant buffer size is required to be aligned.
      return (byteSize + (D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT - 1)) & ~(D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT - 1);
   }

   using factory_gpu = typename IDXGIFactory6;
   using factory_2d = typename ID2D1Factory7;
   using factory_text = typename IDWriteFactory7;

   using gpu_adapter = typename IDXGIAdapter4;
   using gpu_output = typename IDXGIOutput6;

   using device_3d = typename ID3D12Device3;
   using swap_chain = typename IDXGISwapChain4;
   using device_2d = typename ID2D1Device6;
   using context_2d = typename ID2D1DeviceContext6;

   using cmd_list = typename ID3D12GraphicsCommandList3;
   using cmd_allocator = typename ID3D12CommandAllocator;

   using cmd_queue = typename ID3D12CommandQueue;

   using gpu_resource = typename ID3D12Resource1;
   using gpu_heap = typename ID3D12Heap1;
   using gpu_fence = typename ID3D12Fence1;

   using device_back_compat = typename ID3D11On12Device;
   using d3d11_context = typename ID3D11DeviceContext;
   using d3d_render_target = typename gpu_resource;
   using d2d_render_target = typename ID2D1Bitmap1;
   using d3d_wrapped_render_target = typename ID3D11Resource;

   /*
    Explicit specializations of com_ptr.
    The definitions are stored in pch.cpp.
    */
   QGL_GRAPHICS_TEMPLATE template struct QGL_GRAPHICS_API
      winrt::com_ptr<gpu_resource>;

   QGL_GRAPHICS_TEMPLATE template struct QGL_GRAPHICS_API
      winrt::com_ptr<ID3D12DescriptorHeap>;
}