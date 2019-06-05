#pragma once
#include "include/qgl_model_include.h"

namespace qgl::physical
{
   class QGL_MODEL_API body
   {
      public:
      DirectX::XMVECTOR XM_CALLCONV position() const;

      void XM_CALLCONV position(const DirectX::FXMVECTOR pos);

      DirectX::XMMATRIX XM_CALLCONV world() const;
   };
}