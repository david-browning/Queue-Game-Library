#pragma once
#include "include/qgl_content_include.h"
#include "include/Content-Buffers/qgl_content_metadata_buffer.h"

namespace qgl::content
{
   template class QGL_CONTENT_API std::shared_ptr<void>;
   using id_t = uint64_t;
   static constexpr id_t INVALID_ID = static_cast<id_t>(-1);

   class content_accessor
   {
      public:

      content_accessor() :
         m_ptr(nullptr),
         m_id(INVALID_ID),
         m_metadata()
      {

      }

      template<class T>
      content_accessor(id_t id,
                       const CONTENT_METADATA_BUFFER& meta,
                       std::shared_ptr<T>& ptr) :
         m_id(id),
         m_ptr(ptr),
         m_metadata(meta)
      {

      }

      /*
       Copy constructor.
       */
      content_accessor(const content_accessor&) = default;

      /*
       Move constructor.
       */
      content_accessor(content_accessor&&) = default;

      /*
       Destructor
       */
      virtual ~content_accessor() noexcept = default;

      /*
       Returns this content's ID.
       */
      id_t id() const
      {
         return m_id;
      }

      void* get()
      {
         return m_ptr.get();
      }

      std::shared_ptr<void>& shared_get()
      {
         return m_ptr;
      }

      const CONTENT_METADATA_BUFFER& metadata() const
      {
         return m_metadata;
      }

      friend void swap(content_accessor& first,
                       content_accessor& second)
      {
         using std::swap;
         swap(first.m_id, second.m_id);
         swap(first.m_ptr, second.m_ptr);
         swap(first.m_metadata, second.m_metadata);
      }

      content_accessor& operator=(content_accessor other)
      {
         swap(*this, other);
         return *this;
      }

      private:
      
      /*
       Content ID
       */
      id_t m_id;

      /*
       Shared pointer to content
       */
      std::shared_ptr<void> m_ptr;

      CONTENT_METADATA_BUFFER m_metadata;
   };

   QGL_CONTENT_TEMPLATE template class QGL_CONTENT_API 
      std::allocator<content_accessor>;
}