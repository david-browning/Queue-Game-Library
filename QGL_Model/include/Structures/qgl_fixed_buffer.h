#pragma once

namespace qgl
{
   // Type safe, statically allocated buffer safe for serialization.
   template<typename T, size_t n>
   class fixed_buffer final
   {
      public:
      static_assert(std::is_default_constructible<T>::value,
                    "T must be default constructible.");

      static_assert(std::is_copy_constructible<T>::value,
                    "T must be copy constructible.");

      static_assert(std::is_move_constructible<T>::value,
                    "T must be move constructible.");

      class const_iterator;
      class iterator;

      constexpr fixed_buffer()
      {

      }

      constexpr fixed_buffer(T x)
      {
         mem::set_memory(m_data, x, n);
      }

      /*
       Copies 'count' elements from data to this.
       */
      void copy(const T* const data, size_t count)
      {
         if (count > n)
         {
            throw std::out_of_range{
               "Too many elements specified in fixed_buffer::copy()" };
         }

         mem::copy_elements(m_data, data, count);
      }

      /*
       Copies the buffer.
       */
      constexpr fixed_buffer(const T* buf, size_t len)
      {
         if (len > n)
         {
            throw std::out_of_range{
               "Too many elements specified in fixed_buffer::copy()" };
         }

         mem::copy_elements(m_data, buf, len);
      }

      constexpr fixed_buffer(const fixed_buffer& r)
      {
         for (size_t i = 0; i < n; i++)
         {
            m_data[i] = r[i];
         }
      }

      fixed_buffer(fixed_buffer&& r) noexcept
      {
         for (size_t i = 0; i < n; i++)
         {
            m_data[i] = r[i];
         }
      }

      constexpr size_t size() const noexcept
      {
         return n;
      }

      constexpr const T* data() const noexcept
      {
         return m_data;
      }

      constexpr T* data() noexcept
      {
         return m_data;
      }

      constexpr const T& at(size_t i) const
      {
         if (i > n - 1)
         {
            throw std::out_of_range{ "Index is greater than size." };
         }

         return m_data[i];
      }

      constexpr T& at(size_t i)
      {
         if (i > n - 1)
         {
            throw std::out_of_range{ "Index is greater than size." };
         }

         return m_data[i];
      }

      constexpr const T& operator[](size_t i) const
      {
         return at(i);
      }

      constexpr T& operator[](size_t i)
      {
         return at(i);
      }

      friend void swap(fixed_buffer& l, fixed_buffer& r) noexcept
      {
         using std::swap;
         mem::swap_elements(l.data(), r.data(), r.size());
      }

      fixed_buffer& operator=(fixed_buffer r)
      {
         swap(*this, r);
         return *this;
      }

      iterator begin() noexcept
      {
         return iterator{ (T*)m_data };
      }

      iterator end() noexcept
      {
         return iterator{};
      }

      const_iterator cbegin() const noexcept
      {
         return const_iterator{ (T*)m_data };
      }

      const_iterator cend() const noexcept
      {
         return const_iterator{};
      }

      class const_iterator
      {
         public:
         /*
          Constructs the end iterator.
          */
         const_iterator()
         {
         }

         const_iterator(T* data) :
            m_data(data),
            m_curIdx(0)
         {

         }

         const_iterator(const const_iterator&) = default;

         const_iterator(const_iterator&&) noexcept = default;

         virtual ~const_iterator() noexcept
         {
            m_data = nullptr;
            m_curIdx = -1;
         }

         const T* operator->() const
         {
            if (m_curIdx < 0 || m_curIdx >= n)
            {
               throw_out_of_range();
            }

            return m_data + m_curIdx;
         }

         const T& operator*() const
         {
            if (m_curIdx < 0 || m_curIdx >= n)
            {
               throw_out_of_range();
            }

            return m_data[m_curIdx];
         }

         friend void swap(const_iterator l, const_iterator r) noexcept
         {
            using std::swap;
            swap(l.m_curIdx, r.m_curIdx);
            swap(l.m_data, r.m_data);
         }

         const_iterator& operator=(const_iterator r) noexcept
         {
            swap(*this, r);
            return *this;
         }

         const_iterator& operator++()
         {
            if (m_curIdx < 0 || m_curIdx >= n - 1)
            {
               m_curIdx = -1;
            }
            else
            {
               m_curIdx++;
            }

            return *this;
         }

         bool operator==(const const_iterator& r) const noexcept
         {
            return m_curIdx == r.m_curIdx &&
               m_data == r.m_data;
         }

         bool operator!=(const const_iterator& r) const noexcept
         {
            return !(*this == r);
         }

         protected:
         void throw_out_of_range() const
         {
            throw std::out_of_range(
               "Dereferencing the iterator is out of range.");
         }

         int m_curIdx = -1;;
         T* m_data = nullptr;
      };

      class iterator : public const_iterator
      {
         public:
         /*
          Constructs the end iterator.
          */
         iterator() : 
            const_iterator()
         {
         }

         iterator(T* data) :
            const_iterator(data)
         {

         }

         iterator(const iterator&) = default;

         iterator(iterator&&) noexcept = default;

         virtual ~iterator() noexcept = default;

         T* operator->()
         {
            if (const_iterator::m_curIdx < 0 || const_iterator::m_curIdx >= n)
            {
               const_iterator::throw_out_of_range();
            }

            return const_iterator::m_data + const_iterator::m_curIdx;
         }

         T& operator*()
         {
            if (const_iterator::m_curIdx < 0 || const_iterator::m_curIdx >= n)
            {
               const_iterator::throw_out_of_range();
            }

            return const_iterator::m_data[const_iterator::m_curIdx];
         }
      };

      private:
      T m_data[n] = { T() };
   };
}