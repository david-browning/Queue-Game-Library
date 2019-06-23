#include "pch.h"
#include "include/Shapes/qgl_rectangle.h"
using namespace DirectX;

namespace qgl::math
{
   rectangle::rectangle(DirectX::FXMVECTOR upperLeft, 
                        DirectX::FXMVECTOR bottomRight)
   {
      m_ul = upperLeft;
      m_br = bottomRight;
   }

   DirectX::XMVECTOR XM_CALLCONV rectangle::ul() const noexcept
   {
      return m_ul;
   }
   
   DirectX::XMVECTOR XM_CALLCONV rectangle::br() const noexcept
   {
      return m_br;
   }
  
   void XM_CALLCONV rectangle::ul(DirectX::FXMVECTOR pos) noexcept
   {
      m_ul = pos;
   }

   void XM_CALLCONV rectangle::br(DirectX::FXMVECTOR pos) noexcept
   {
      m_br = pos;
   }
   
   DirectX::XMVECTOR XM_CALLCONV rectangle::norm() const noexcept
   {
      /*
      
       U------
       |\    |
       | \   |
       |  \  |
       |   \ |
       |    \|
       a-----B

       N = aU X aB
       aU = a - U
       aB = a - B
       a = min(U, B)
      */
      const DirectX::XMVECTOR a = XMVectorMin(m_ul, m_br);
      m_norm = XMVector3Cross(XMVectorSubtract(a, m_ul),
                              XMVectorSubtract(a, m_br));
      return m_norm;
   }

   DirectX::XMVECTOR XM_CALLCONV rectangle::norm_cache() const noexcept
   {
      return m_norm;
   }
}