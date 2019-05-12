#pragma once

namespace qgl::content
{
   template<typename IDT, class InputIterator>
   class content_iterator : public std::iterator<std::forward_iterator_tag,
      content_accessor<IDT>>
   {
      public:
      content_iterator(InputIterator first, InputIterator last) :
         m_first(first),
         m_last(last)
      {

      }

      content_iterator(const content_iterator<IDT, InputIterator>&) = default;

      content_iterator<IDT, InputIterator>& operator++()
      {
         ++m_first;
         return *this;
      }

      /*
       Returns true if the two iterators are equal.
       */
      bool operator==(content_iterator<IDT, InputIterator> other) const
      {
         return m_first == other.m_first;
      }

      /*
       Returns true if the two iterators are not equal.
       */
      bool operator!=(content_iterator<IDT, InputIterator> other) const
      {
         return m_first != other.m_first;
      }

      content_accessor<IDT>& operator*() const
      {
         return m_first->second;
      }

      private:
      InputIterator m_first;
      InputIterator m_last;
   };
}