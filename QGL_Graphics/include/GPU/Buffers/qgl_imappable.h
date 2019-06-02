#pragma once
#include "include/qgl_graphics_include.h"

namespace qgl::graphics::gpu::buffers
{
   /*
    
    */
   template<typename T>
   class imappable
   {
      public:
      imappable() :
         m_buffer(nullptr)
      {
         //The CPU pointer is populated by map()
      }

      imappable(const imappable&) = delete;

      /*
       Copies the pointer from r and unmaps r.
       */
      imappable(imappable&& r) :
         m_buffer(r.m_buffer)
      {
         r.unmap();
      }

      virtual ~imappable()
      {
         unmap();
      }

      void map()
      {
         if (!mapped())
         {
            p_map();
         }
      }

      void unmap()
      {
         if (mapped())
         {
            p_unmap();
            m_buffer = nullptr;
         }
      }

      const T* mapping() const noexcept
      {
         return m_buffer;
      }

      bool mapped() const noexcept
      {
         return m_buffer != nullptr;
      }

      protected:

      T* mapping() noexcept
      {
         return m_buffer;
      }

      void** map_put()
      {
         return reinterpret_cast<void**>(&m_buffer);
      }

      private:

      /*
       Maps a GPU resource to the CPU pointer. Use map_put() to get a pointer 
       to the buffer pointer.
       */
      virtual void p_map() = 0;

      virtual void p_unmap() = 0;
      
      /*
       Pointer to the cpu pointer.
       */
      T* m_buffer;
   };
}