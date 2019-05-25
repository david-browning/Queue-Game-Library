#pragma once
#include "include/qgl_content_include.h"
#include "include/Content-Buffers/qgl_content_metadata_buffer.h"

namespace qgl::content
{
   using id_t = uint64_t;
   static constexpr id_t INVALID_CONTENT_ID = static_cast<id_t>(-1);

   class QGL_CONTENT_API content_accessor
   {
      public:
      /*
       Default Constructor. The pointer is nullptr, the ID is invalid, and the
       metadata is default.
       */
      content_accessor();

      template<class T>
      content_accessor(id_t id,
                       const CONTENT_METADATA_BUFFER& meta,
                       std::shared_ptr<T>& ptr)
      {
         construct(id, meta, std::reinterpret_pointer_cast<void, T>(ptr));
      }

      /*
       Copy constructor.
       */
      content_accessor(const content_accessor&);

      /*
       Move constructor.
       */
      content_accessor(content_accessor&&);

      /*
       Destructor
       */
      virtual ~content_accessor() noexcept;

      /*
       Returns this content's ID.
       */
      id_t id() const;

      void* get();

      const void* get() const;

      std::shared_ptr<void>& shared_get();

      const std::shared_ptr<void>& shared_get() const;

      const CONTENT_METADATA_BUFFER& metadata() const;

      friend void swap(content_accessor& first,
                       content_accessor& second);

      content_accessor& operator=(content_accessor other);

      private:
      void construct(id_t id,
                     const CONTENT_METADATA_BUFFER& meta,
                     std::shared_ptr<void>& ptr);


      struct impl;
      impl* m_impl_p;
   };
}