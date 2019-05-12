#pragma once
#include "include/qgl_content_include.h"

namespace qgl::content
{
   template<typename IDT>
   class content_accessor
   {
      public:
      template<class T>
      content_accessor(IDT id, std::shared_ptr<T>& ptr) :
         m_id(id),
         m_ptr(ptr)
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
      IDT id() const
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

      friend void swap(content_accessor<IDT>& first,
                       content_accessor<IDT>& second)
      {
         using std::swap;
         swap(first.m_id, second.m_id);
         swap(first.m_ptr, second.m_ptr);
      }

      content_accessor<IDT>& operator=(content_accessor<IDT> other)
      {
         swap(*this, other);
         return *this;
      }

      private:
      
      /*
       Content ID
       */
      IDT m_id;

      /*
       Shared pointer to content
       */
      std::shared_ptr<void> m_ptr;
   };
}