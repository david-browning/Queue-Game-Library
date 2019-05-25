#include "pch.h"
#include "include/Content-Store/qgl_content_accessor.h"

namespace qgl::content
{
   struct content_accessor::impl
   {
      public:
      impl() :
         m_ptr(nullptr),
         m_id(INVALID_CONTENT_ID),
         m_metadata()
      {

      }

      impl(id_t id,
           const CONTENT_METADATA_BUFFER& meta,
           std::shared_ptr<void>& ptr) :
         m_metadata(meta),
         m_id(id),
         m_ptr(ptr)
      {

      }

      impl(const impl&) = default;

      impl(impl&&) = default;

      virtual ~impl() noexcept = default;

      id_t id() const
      {
         return m_id;
      }

      void* get()
      {
         return m_ptr.get();
      }

      const void* get() const
      {
         return m_ptr.get();
      }

      std::shared_ptr<void>& shared_get()
      {
         return m_ptr;
      }

      const std::shared_ptr<void>& shared_get() const
      {
         return m_ptr;
      }

      const CONTENT_METADATA_BUFFER& metadata() const
      {
         return m_metadata;
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

   content_accessor::content_accessor() :
      m_impl_p(new impl())
   {
   }

   content_accessor::content_accessor(const content_accessor& c) :
      m_impl_p(new impl(*c.m_impl_p))
   {

   }

   content_accessor::content_accessor(content_accessor&& m) :
      m_impl_p(m.m_impl_p)
   {
      m.m_impl_p = nullptr;
   }

   content_accessor::~content_accessor() noexcept
   {
      delete m_impl_p;
   }

   id_t content_accessor::id() const
   {
      return m_impl_p->id();
   }

   void* content_accessor::get()
   {
      return m_impl_p->get();
   }

   const void* content_accessor::get() const
   {
      return m_impl_p->get();
   }

   std::shared_ptr<void>& content_accessor::shared_get()
   {
      return m_impl_p->shared_get();
   }

   const std::shared_ptr<void>& content_accessor::shared_get() const
   {
      return m_impl_p->shared_get();
   }

   const CONTENT_METADATA_BUFFER& content_accessor::metadata() const
   {
      return m_impl_p->metadata();
   }

   void swap(content_accessor& first, content_accessor& second)
   {
      using std::swap;
      swap(first.m_impl_p, second.m_impl_p);
   }

   content_accessor& content_accessor::operator=(content_accessor other)
   {
      swap(*this, other);
      return *this;
   }

   void content_accessor::construct(id_t id,
                                    const CONTENT_METADATA_BUFFER& meta,
                                    std::shared_ptr<void>& ptr)
   {
      m_impl_p = new impl(id, meta, ptr);
   }
}