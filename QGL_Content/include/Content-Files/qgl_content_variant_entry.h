#pragma once
#include "include/qgl_content_include.h"
#include "include/Content-Files/qgl_data_content_entry.h"
#include "include/Content-Files/qgl_shared_content_entry.h"
#include <variant>

namespace qgl::content
{
   template class QGL_CONTENT_API std::shared_ptr<DATA_CONTENT_ENTRY>;
   template class QGL_CONTENT_API std::shared_ptr<SHARED_CONTENT_ENTRY>;

   /*
    Content data can either be a buffer, baked into the file, or a shared
    entry that references an external file.
    */
   enum class CONTENT_DATA_USE_TYPES
   {
      CONTENT_DATA_USE_TYPE_UNKNOWN,
      CONTENT_DATA_USE_TYPE_BUFFER,
      CONTENT_DATA_USE_TYPE_SHARED,
   };

   /*
    Stores either a content buffer, or information about an external file.
    */
   class QGL_CONTENT_API content_variant_entry
   {
      public:

      content_variant_entry();

      /*
       Sets the use type for this variant, but has not data backing it.
       */
      content_variant_entry(CONTENT_DATA_USE_TYPES useType);

      /*
       Constructs the variant so it holds a content buffer.
       Copies the buffer. The buffer can be freed after constructing this.
       */
      content_variant_entry(const void* buff,
                            size_t numBytes);

      /*
       Constructs a variant so it references an external content file.
       Copies the buffer. The buffer can be freed after constructing this.
       */
      content_variant_entry(const wchar_t* sharedPath);

      /*
       Copy constructor.
       */
      content_variant_entry(const content_variant_entry&);

      /*
       Move constructor.
       */
      content_variant_entry(content_variant_entry&&);

      /*
       Destructor.
       */
      ~content_variant_entry() noexcept;

      /*
       Returns true if the variant references an external file.
       */
      bool shared() const noexcept
      {
         return m_useType ==
            CONTENT_DATA_USE_TYPES::CONTENT_DATA_USE_TYPE_SHARED;
      }

      /*
       Returns a const reference to the content buffer.
       Throws std::bad_variant_access if the variant does not hold a content
       buffer.
       */
      const DATA_CONTENT_ENTRY& buffer() const
      {
         if (shared())
         {
            throw std::bad_variant_access();
         }

         return *m_buffer;
      }

      /*
       Returns a const reference to the shared file buffer.
       Throws std::bad_variant_access if the variant does not hold a reference
       to an external file.
       */
      const SHARED_CONTENT_ENTRY& shared_buffer() const
      {
         if (!shared())
         {
            throw std::bad_variant_access();
         }

         return *m_sharedBuffer;
      }

      friend void swap(content_variant_entry& first,
                       content_variant_entry& second) noexcept
      {
         using std::swap;
         swap(first.m_useType, second.m_useType);
         swap(first.m_buffer, second.m_buffer);
         swap(first.m_sharedBuffer, second.m_sharedBuffer);
         swap(first.m_loaded, second.m_loaded);
      }

      content_variant_entry& operator=(content_variant_entry r) noexcept
      {
         swap(*this, r);
         return *this;
      }

      friend bool operator==(const content_variant_entry& r,
                             const content_variant_entry& l) noexcept
      {
         return r.m_useType == l.m_useType &&
            r.m_buffer == l.m_buffer &&
            r.m_sharedBuffer == l.m_sharedBuffer;
      }

      private:
      CONTENT_DATA_USE_TYPES m_useType;
      DATA_CONTENT_ENTRY* m_buffer;
      SHARED_CONTENT_ENTRY* m_sharedBuffer;
      bool m_loaded;
   };
}