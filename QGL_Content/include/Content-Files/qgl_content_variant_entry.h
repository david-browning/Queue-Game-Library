#pragma once
#include "include/qgl_content_include.h"
#include "include/Content-Files/qgl_data_content_entry.h"
#include "include/Content-Files/qgl_shared_content_entry.h"
#include <variant>

namespace qgl::content
{
   /*
    Content data can either be a buffer, baked into the file, or a shared
    entry that references an external file.
    */
   enum class CONTENT_DATA_USE_TYPES
   {
      CONTENT_DATA_USE_TYPE_BUFFER,
      CONTENT_DATA_USE_TYPE_SHARED,
   };

   /*
    Stores either a content buffer, or information about an external file.
    */
   class content_variant_entry
   {
      public:
      /*
       Sets the use type for this variant, but has not data backing it.
       */
      content_variant_entry(CONTENT_DATA_USE_TYPES useType);

      /*
       Constructs the variant so it holds a content buffer.
       */
      content_variant_entry(const DATA_CONTENT_ENTRY& b);

      /*
       Constructs the variant so it references an external content file.
       */
      content_variant_entry(const SHARED_CONTENT_ENTRY& b);

      /*
       Copy constructor.
       */
      content_variant_entry(const content_variant_entry&) = default;

      /*
       Move constructor.
       */
      content_variant_entry(content_variant_entry&&) = default;

      /*
       Destructor.
       */
      ~content_variant_entry() noexcept = default;

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

      private:
      CONTENT_DATA_USE_TYPES m_useType;
      std::shared_ptr<DATA_CONTENT_ENTRY> m_buffer;
      std::shared_ptr<SHARED_CONTENT_ENTRY> m_sharedBuffer;
      bool m_loaded;
   };
}