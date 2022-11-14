#pragma once
#include <QGLMath.h>
#include <collection.h>
#include <ppltasks.h>
#include "UnitTestApp.xaml.h"
#include "CPPUnitTest.h"

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
   template<> static std::wstring ToString<qgl::math::rational<int>>
   (const qgl::math::rational<int>& r)
   {
      std::wstringstream ws;
      ws << r.numerator() << '/' << r.denominator();
      return ws.str();
   }

   template<> static std::wstring ToString<DirectX::XMVECTOR>(
      const DirectX::XMVECTOR& v)
   {
      using namespace DirectX;
      std::wstringstream ws;
      ws << "<" << XMVectorGetX(v) << ", " << XMVectorGetY(v) << ", " <<
         XMVectorGetZ(v) << ", " << XMVectorGetW(v) << ">";
      return ws.str();
   }

   template<> static std::wstring ToString<DirectX::XMMATRIX>(
      const DirectX::XMMATRIX& m)
   {
      using namespace DirectX;
      std::wstringstream ws;

      XMFLOAT4X4 inspect;
      XMStoreFloat4x4(&inspect, m);

      for (size_t row = 0; row < 4; row++)
      {
         for (size_t col = 0; col < 4; col++)
         {
            auto f = inspect(row, col);
            ws << f << " ";
         }

         ws << '\n';
      }

      return ws.str();
   }
}