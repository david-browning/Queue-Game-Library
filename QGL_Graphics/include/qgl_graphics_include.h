#pragma once
#include <Unknwn.h>

#include <QGLModel.h>
#include <QGLContent.h>
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

   inline UINT CalculateConstantBufferByteSize(UINT byteSize)
   {
       // Constant buffer size is required to be aligned.
      return (byteSize + (D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT - 1)) & ~(D3D12_CONSTANT_BUFFER_DATA_PLACEMENT_ALIGNMENT - 1);
   }
   typedef ID3D12Device3 d3d_device;

   typedef ID3D11On12Device d3d11_device;
   typedef ID3D11DeviceContext d3d11_context;

   typedef ID2D1Device6 d2d_device;
   typedef ID2D1DeviceContext6 d2d_context;

   typedef IDXGIFactory6 dxgi_factory;
   typedef ID2D1Factory7 d2d_factory;
   typedef IDWriteFactory7 d_write_factory;

   typedef ID3D12GraphicsCommandList3 d3d_command_list;
   typedef ID3D12CommandAllocator d3d_cmd_allocator;
   typedef ID3D12CommandQueue d3d_cmd_queue;

   typedef ID3D12Resource1 d3d_resource;
   typedef ID3D12Heap1 d3d_heap;
   typedef ID3D12Fence1 d3d_fence;

   typedef d3d_resource d3d_render_target;
   typedef IDXGISwapChain4 d3d_swap_chain;
   typedef ID2D1Bitmap1 d2d_render_target;
   typedef ID3D11Resource d3d_wrapped_render_target;

}