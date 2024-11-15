#pragma once
#include "include/qgl_content_include.h"
#include "include/Loaders/qgl_iloader_metadata.h"

namespace qgl::content
{
   /*
    A content loader is used to convert an object to and from a serializable
    binary buffer.
    */
   class content_loader
   {
      public:
      /*
       Makes a copy of the metadata pointed to. After construction, the pointer
       passed to this can be freed.
       */
      content_loader(iloader_metadata* meta_p) :
         m_loaderMeta_p(qgl::make_unique(meta_p))
      {

      }

      /*
       Makes a copy of the metadata pointed to. After construction, the pointer
       passed to this can be freed.
       */
      content_loader(qgl_unique_ptr<iloader_metadata>& meta_p) :
         m_loaderMeta_p(duplicate(meta_p))
      {

      }

      /*
       Moves the metadata pointer into this. After construction, the pointer
       passed to this is invalid.
       */
      content_loader(qgl_unique_ptr<iloader_metadata>&& meta_p) :
         m_loaderMeta_p(std::forward<qgl_unique_ptr<iloader_metadata>>(meta_p))
      {

      }

      /*
       Copy constructor.
       */
      content_loader(const content_loader& r) :
         m_loaderMeta_p(duplicate(r.m_loaderMeta_p))
      {

      }

      /*
       Move constructor.
       */
      content_loader(content_loader&& r) noexcept :
         m_loaderMeta_p(std::move(r.m_loaderMeta_p))
      {
         r.m_loaderMeta_p = nullptr;
      }

      virtual ~content_loader() noexcept = default;

      friend void swap(content_loader& l, content_loader& r) noexcept
      {
         using std::swap;
         swap(l.m_loaderMeta_p, r.m_loaderMeta_p);
      }

      /*
       Copy assign
       */
      content_loader& operator=(content_loader r) noexcept
      {
         swap(*this, r);
         return *this;
      }

      /*
       Returns a reference to this loader's GUID.
       */
      const qgl::guid& id() const noexcept
      {
         qgl::guid ret;
         m_loaderMeta_p->id(&ret);
         return ret;
      }

      /*
       Converts a byte buffer to a unique pointer pointing to the loaded,
       in-memory representation of an object.
       */
      template<class T>
      std::unique_ptr<T, void(*)(T*)> input(size_t bytes,
                                            const std::byte* fileData) const
      {
         // Figure out how big the buffer for the unique pointer needs to be.
         uint64_t bufSize = 0;
         check_result(m_loaderMeta_p->input(bytes, fileData,
                                            &bufSize, nullptr));

         // Allocate memory for the unique pointer and load it from the file
         // data.
         auto buf = malloc(bufSize);
         check_result(m_loaderMeta_p->input(bytes, fileData,
                                            &bufSize, buf));

         std::unique_ptr<T, void(*)(T*)> ret(
            reinterpret_cast<T*>(buf),
            [](T* ptr)
         {
            free(ptr);
         });

         return ret;
      }

      /*
       Exports the data pointed to by object_p into a serializable byte buffer.
       */
      template<class T>
      file_buffer_t output(T* object_p) const
      {
         uint64_t fileSize = 0;
         check_result(m_loaderMeta_p->output(sizeof(T),
                                             object_p,
                                             &fileSize,
                                             nullptr));

         // Allocate a buffer to export the object to.
         file_buffer_t ret{ fileSize };
         check_result(m_loaderMeta_p->output(sizeof(T), object_p,
                                             &fileSize, ret.data()));
         return ret;
      }

      private:
      qgl_unique_ptr<iloader_metadata> m_loaderMeta_p;
   };
}