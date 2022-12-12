#pragma once
#include "include/qgl_model_include.h"
#include "include/Memory/qgl_mem_helpers.h"

namespace qgl::mem
{
#pragma pack(push, 1)
   /*
    A constexpr bitset. This allocates exactly Bit, rounded up to the nearest
    8th, number of bits.
    */
   template<size_t Bits, bool unsgnd = false>
   class flags final
   {
      public:
      using type = typename traits::make_type<
         traits::bit_size<Bits>::value, unsgnd>::type;

      /*
       Construct with all bits cleared.
       */
      constexpr flags()
      {

      }

      constexpr flags(type flgs) :
         data(flgs)
      {

      }

      /*
       Sets the idx'th bit to 1.
       */
      constexpr void set(size_t idx)
      {
         mem::set_bit(data, idx);
      }

      /*
       Sets the idx'th bit to 0.
       */
      constexpr void reset(size_t idx)
      {
         mem::clear_bit(data, idx);
      }

      /*
       returns true if the bit at idx is set.
       */
      constexpr bool at(size_t idx) const
      {
         return mem::is_bit_set(data, idx);
      }

      /*
       Returns the number of bits this encapsulates.
       */
      constexpr size_t size() const noexcept
      {
         return sizeof(type) * CHAR_BIT;
      }

      template<size_t start, size_t end>
      constexpr type range() const
      {
         static_assert(end > start,
                       "End parameter must be larger than start.");
         static_assert(end < Bits,
                       "End parameter must be less than Bits.");

         return data & mem::mask<type>(start, end);
      }

      template<size_t start, size_t end>
      constexpr typename mem::traits::make_type<mem::traits::bit_size<end - start>::value, unsgnd>::type range_shift() const
      {
         using retT = typename mem::traits::make_type<
            mem::traits::bit_size<end - start>::value, unsgnd>::type;
         static_assert(end > start,
                       "End parameter must be larger than start.");
         static_assert(end < Bits,
                       "End parameter must be less than Bits.");

         return static_cast<retT>(range<start, end>() >> start);
      }

      /*
       Interprets the raw data as another type.
       T must be the same number of bits as this.
       */
      template<typename T>
      constexpr T as() const noexcept
      {
         static_assert(sizeof(T) == sizeof(type),
                       "T does not have the same number of bits as this.");
         return static_cast<T>(data);
      }

      /*
       Returns true if the bit at idx is set.
       */
      constexpr bool operator[](size_t idx) const
      {
         return at(idx);
      }

      friend void swap(flags& l, flags& r) noexcept
      {
         using std::swap;
         swap(l.data, r.data);
      }

      flags& operator=(flags r)
      {
         swap(*this, r);
         return *this;
      }

      private:
      type data = 0;
   };
#pragma pack(pop)

   static_assert(sizeof(flags<7>) == 1,
                 "flags size is not correct (7)");

   static_assert(sizeof(flags<8>) == 1,
                 "flags size is not correct (8)");

   static_assert(sizeof(flags<11>) == 2,
                 "flags size is not correct (11)");

   static_assert(sizeof(flags<16>) == 2,
                 "flags size is not correct (16)");

   static_assert(sizeof(flags<17>) == 4,
                 "flags size is not correct (17)");

   static_assert(sizeof(flags<33>) == 8,
                 "flags size is not correct (33)");
}