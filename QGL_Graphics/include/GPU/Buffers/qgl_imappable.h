#pragma once
#include "include/qgl_graphics_include.h"

namespace qgl::graphics::gpu::buffers
{
   template<typename T>
   class imappable
   {
      public:
      imappable() :
         m_buffer(nullptr),
         m_isMapped(false)
      {
         //The CPU pointer is populated by map()
      }

      imappable(const imappable&) = delete;

      imappable(imappable&& r) :
         m_buffer(r.m_buffer),
         m_isMapped(r.m_isMapped)
      {
         r.unmap();
      }

      virtual ~imappable()
      {
         unmap();
      }

      void map()
      {
         if (!m_isMapped)
         {
            p_map();
            m_isMapped = true;
         }
      }

      void unmap()
      {
         if (m_isMapped)
         {
            p_unmap();
            m_buffer = nullptr;
            m_isMapped = false;
         }
      }

      const T* mapping() const noexcept
      {
         return m_buffer;
      }

      bool mapped() const noexcept
      {
         return m_isMapped;
      }

      protected:

      T* mapping() noexcept
      {
         return m_buffer;
      }

      private:

      virtual void p_map() = 0;

      virtual void p_unmap() = 0;
      
      T* m_buffer;
      bool m_isMapped;
   };
}