#pragma once
#include "include/Memory/qgl_flags.h"

namespace qgl
{
   enum class os_version_flags : uint8_t
   {
      unknown = 0,
      windows = 1,
      linux = 2,
      mac = 3,
   };

   enum class char_encodings : uint8_t
   {
      one_byte = 0,
      two_byte = 1,
      multibyte = 2,
   };

   static constexpr uint8_t QGL_VERSION_MAJOR_0 = 0;
   static constexpr uint8_t QGL_VERSION_MAJOR_1 = 1;
   static constexpr uint8_t QGL_VERSION_MINOR_0 = 0;
   static constexpr uint8_t QGL_VERSION_MINOR_1 = 1;

   static constexpr uint8_t QGL_VERSION_MAJOR_LATEST = QGL_VERSION_MAJOR_0;
   static constexpr uint8_t QGL_VERSION_MINOR_LATEST = QGL_VERSION_MINOR_1;

#pragma pack(push, 1)
   struct qgl_version
   {
      public:
      constexpr qgl_version()
      {

      }

      constexpr qgl_version(uint8_t versionMajor,
                            uint8_t versionMinor,
                            char_encodings fileEnc,
                            os_version_flags osVersion) :
         m_flags(
            (static_cast<uint32_t>(osVersion) << 30) |
            (static_cast<uint32_t>(fileEnc) << 28) |
            (static_cast<uint32_t>(versionMajor) << 8) |
            (static_cast<uint32_t>(versionMinor)))
      {

      }

      constexpr qgl_version(const qgl_version&) = default;

      constexpr qgl_version(qgl_version&&) noexcept = default;

      ~qgl_version() noexcept = default;

      friend void swap(qgl_version& l, qgl_version& r) noexcept
      {
         using std::swap;
         swap(l.m_flags, r.m_flags);
      }

      constexpr char_encodings path_encoding() const noexcept
      {
         return static_cast<char_encodings>(m_flags.range_shift<28, 29>());
      }

      constexpr os_version_flags os() const noexcept
      {
         return static_cast<os_version_flags>(m_flags.range_shift<30, 31>());
      }

      constexpr uint8_t major() const noexcept
      {
         return m_flags.range_shift<8, 15>();
      }

      constexpr uint8_t minor() const noexcept
      {
         return m_flags.range_shift<0, 7>();
      }

      private:
      /*
      Bit layout: {2 OS Flag} { 2 path encoding } { 12 flags} {8 Major} {8 Minor}
      */
      qgl::mem::flags<32, true> m_flags;
   };
#pragma pack(pop)

   static_assert(sizeof(qgl_version) == 4, "QGL versions must be 4 bytes.");

   static constexpr qgl_version QGL_VERSION_0_1_WIN {
      QGL_VERSION_MAJOR_0,
      QGL_VERSION_MINOR_1,
      char_encodings::two_byte,
      os_version_flags::windows };


   static constexpr qgl_version QGL_VERSION_0_2_WIN {
      QGL_VERSION_MAJOR_0,
      2,
      char_encodings::two_byte,
      os_version_flags::windows };

   static constexpr qgl_version QGL_VERSION_LATEST = QGL_VERSION_0_2_WIN;
}

namespace std
{
   /*
    The hash of a version is an 8-byte value where only the bottom 2 bytes are
    set. The lowest byte is a version's minor. The next byte is the version's
    major.
    */
   template<> struct hash<qgl::qgl_version>
   {
      constexpr hash()
      {

      }

      typedef qgl::qgl_version argument_type;
      typedef std::size_t result_type;
      constexpr result_type operator()(const argument_type& v) const noexcept
      {
         return (static_cast<result_type>(v.major()) << CHAR_BIT) |
            static_cast<result_type>(v.minor());
      }
   };
}

namespace qgl::hashes
{
   static constexpr auto VERSION_0_1_HASH =
      std::hash<qgl::qgl_version>{}(qgl::QGL_VERSION_0_1_WIN);

   static constexpr auto VERSION_0_2_HASH =
      std::hash<qgl::qgl_version>{}(qgl::QGL_VERSION_0_2_WIN);
}