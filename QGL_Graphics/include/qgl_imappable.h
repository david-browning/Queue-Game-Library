#pragma once
#include "qgl_graphics_include.h"

namespace qgl::graphics::low
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

      imappable(const imappable& r) :
         m_buffer(r.m_buffer),
         m_isMapped(r.m_isMapped)
      {

      }

      imappable(imappable&& r) :
         m_buffer(r.m_buffer),
         m_isMapped(r.m_isMapped)
      {
         r.m_isMapped = false;
         r.m_buffer = nullptr;
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

      inline T* mapping()
      {
         return m_buffer;
      }

      inline const T* mapping() const
      {
         return m_buffer;
      }

      inline bool mapped() const
      {
         return m_isMapped;
      }

      protected:
      T* m_buffer;

      private:
      virtual void p_map() = 0;

      virtual void p_unmap() = 0;

      bool m_isMapped;
   };
}