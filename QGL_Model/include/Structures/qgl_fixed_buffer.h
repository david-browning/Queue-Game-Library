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
            throw std::out_of_range{"Index is greater than size."};
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

      private:
      T m_data[n] = { T() };
   };
}