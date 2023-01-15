#pragma once
#include "include/qgl_model_include.h"
#include "include/Memory/qgl_mem_helpers.h"
#include "include/qgl_misc_helpers.h"
#include "include/Errors/qgl_e_checkers.h"

namespace qgl
{
   /*
    GUID is stored in bit-endian form.
    */
#pragma pack(push, 1)
   struct guid final
   {
      public:
      static constexpr size_t UUID_BYTES = 16;

      /*
       Creates a guid with all 0s.
       */
      constexpr guid() noexcept :
         m_low(0),
         m_high(0)
      {

      }

      constexpr guid(const std::array<uint8_t, UUID_BYTES>& bytes) noexcept :
         guid(bytes.data())
      {

      }

      constexpr guid(const uint8_t* const data) noexcept :
         m_low(low(data)),
         m_high(high(data))
      {

      }

      constexpr guid(const char s[33]) noexcept :
         m_low(mem::from_hex<17>(s)),
         m_high(mem::from_hex<17>(s + 16))
      {

      }

      guid(const guid&) noexcept = default;

      guid(guid&&) noexcept = default;

      ~guid() noexcept = default;

      const uint8_t* data() const noexcept
      {
         return reinterpret_cast<const uint8_t*>(this);
      }

      constexpr uint32_t time_low() const noexcept
      {
         return (m_low & 0xFFFF'FFFF'0000'0000) >>
            (CHAR_BIT * sizeof(uint32_t));
      }

      constexpr uint16_t time_mid() const noexcept
      {
         return (m_low & 0x0000'0000'FFFF'0000) >>
            (CHAR_BIT * sizeof(UINT16));
      }

      constexpr uint16_t time_high() const noexcept
      {
         return m_low & 0x0000'0000'0000'FFFF;
      }

      constexpr uint16_t clock() const noexcept
      {
         return (m_high & 0xFFFF'0000'0000'0000) >>
            (CHAR_BIT * 6);
      }

      /*
       The node is actually 6 bytes. The top two bytes of this property will
       always be 0.
       */
      constexpr uint64_t node() const noexcept
      {
         return m_high & 0x0000'FFFF'FFFF'FFFF;
      }

      template<typename CharT>
      std::basic_string<CharT> str()  const noexcept
      {
         std::basic_stringstream<CharT> ret;
         ret << std::uppercase << std::hex << std::setfill(CharT('0')) <<
            std::setw(8) << time_low() <<
            '-' << std::setw(4) << time_mid() <<
            '-' << std::setw(4) << time_high() <<
            '-' << std::setw(4) << clock() <<
            '-' << std::setw(12) << node();
         return ret.str();
      }

      /*
       Swaps the content of l and r.
       */
      friend void swap(guid& l, guid& r) noexcept
      {
         using std::swap;
         swap(l.m_high, r.m_high);
         swap(l.m_low, r.m_low);
      }

      /*
       Assignment operator.
       */
      guid& operator=(guid& r) noexcept
      {
         swap(*this, r);
         return *this;
      }

      /*
       Move assign operator.
       */
      guid& operator=(guid&& r) = default;

      /*
       * Returns true if each byte in the guids are equal.
       */
      friend bool operator==(const guid& l, const guid& r) noexcept
      {
         return l.m_low == r.m_low && l.m_high == r.m_high;
      }

      friend bool operator!=(const guid& l, const guid& r)
      {
         return !(l == r);
      }

      friend bool operator<(const guid& l, const guid& r)
      {
         return l.m_low < r.m_low || l.m_high < r.m_high;
      }

      friend bool operator>(const guid& l, const guid& r)
      {
         return l.m_low > r.m_low || l.m_high > r.m_high;
      }

      friend std::ostream& operator<<(std::ostream& os, const guid& r)
      {
         os << r.str<char>();
         return os;
      }

      private:
      constexpr uint64_t low(const uint8_t* const g)
      {
         return qgl::mem::bswap(*(const uint64_t* const)g);
      }

      constexpr uint64_t high(const uint8_t* const g)
      {
         auto ptr = (const uint64_t* const)(g + sizeof(uint64_t));
         return qgl::mem::bswap(*ptr);
      }

      uint64_t m_low = 0;
      uint64_t m_high = 0;
   };
#pragma pack(pop)

   inline guid random_guid()
   {
      GUID g;
      check_result(CoCreateGuid(&g));
      return guid{ reinterpret_cast<const uint8_t*>(&g) };
   }
}

namespace std
{
   template<> struct hash<qgl::guid>
   {
      constexpr hash()
      {

      }

      typedef qgl::guid argument_type;
      typedef std::size_t result_type;
      constexpr result_type operator()(const argument_type& t) const noexcept
      {
         return qgl::fast_hash(&t, sizeof(argument_type), 0x4AC0E82519BA9478);
      }
   };
}
