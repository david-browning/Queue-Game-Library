#pragma once
#include "include/qgl_model_include.h"

namespace qgl
{
   enum class QGL_OS_VERSION_FLAGS : uint8_t
   {
      OS_UNKNOWN = 0,
      OS_WINDOWS = 1,
      OS_LINUX = 2,
      OS_MAC = 3
   };

   static constexpr uint8_t QGL_VERSION_MAJOR_0 = 0;
   static constexpr uint8_t QGL_VERSION_MAJOR_1 = 1;
   static constexpr uint8_t QGL_VERSION_MINOR_0 = 0;
   static constexpr uint8_t QGL_VERSION_MINOR_1 = 1;

   static constexpr uint8_t QGL_VERSION_MAJOR_LATEST = QGL_VERSION_MAJOR_0;
   static constexpr uint8_t QGL_VERSION_MINOR_LATEST = QGL_VERSION_MINOR_1;

   static constexpr uint8_t QGL_VERSION_NUM_FLAGS_BITS = 14;

   /*
    Bit layout: {2 OS Flag} { 14 Flags} {8 Major} {8 Minor}
    */
   #pragma pack(push, 1)
   struct QGL_MODEL_API qgl_version_t
   {
      constexpr qgl_version_t(uint8_t major, uint8_t minor,
                              QGL_OS_VERSION_FLAGS os) :
         Flags(static_cast<uint16_t>(os) << QGL_VERSION_NUM_FLAGS_BITS),
         Major(major),
         Minor(minor)
      {

      }

      qgl_version_t(const qgl_version_t&) = default;

      qgl_version_t(qgl_version_t&&) = default;

      uint16_t Flags;
      uint8_t Major;
      uint8_t Minor;

      friend void swap(qgl_version_t& l, qgl_version_t& r)
      {
         using std::swap;
         swap(l.Flags, r.Flags);
         swap(l.Major, r.Major);
         swap(l.Minor, r.Minor);
      }

      qgl_version_t& operator=(qgl_version_t r);

      friend bool operator==(const qgl_version_t& l, 
                             const qgl_version_t& r) noexcept
      {
         return l.Flags == r.Flags &&
            l.Major == r.Major &&
            l.Minor == r.Minor;
      }
   };
   #pragma pack(pop)

   constexpr uint8_t version_major(qgl_version_t version)
   {
      return version.Major;
   }

   constexpr uint8_t version_minor(qgl_version_t version)
   {
      return version.Minor;
   }

   constexpr uint16_t version_flags(qgl_version_t version)
   {
      return version.Flags;
   }

   constexpr QGL_OS_VERSION_FLAGS version_os(qgl_version_t version)
   {
      return static_cast<QGL_OS_VERSION_FLAGS>((version.Flags & 0xC000) >>
                                               QGL_VERSION_NUM_FLAGS_BITS);
   }

   constexpr qgl_version_t make_win_version(const uint8_t versionMajor,
                                            const uint8_t versionMinor)
   {
      return qgl_version_t(versionMajor,
                           versionMinor,
                           QGL_OS_VERSION_FLAGS::OS_WINDOWS);
   }

   static constexpr qgl_version_t QGL_VERSION_0_1_WIN =
      make_win_version(QGL_VERSION_MAJOR_0,
                       QGL_VERSION_MINOR_1);

   static constexpr qgl_version_t QGL_VERSION_0_2_WIN =
      make_win_version(QGL_VERSION_MAJOR_0,
                       2);

   static constexpr qgl_version_t QGL_VERSION_LATEST = QGL_VERSION_0_2_WIN;
}

namespace std
{
   template<> struct hash<qgl::qgl_version_t>
   {
      constexpr hash()
      {

      }

      typedef qgl::qgl_version_t argument_type;
      typedef std::size_t result_type;
      constexpr  result_type operator()(argument_type const& v) const noexcept
      {
         return (static_cast<result_type>(v.Major) << CHAR_BIT) |
            static_cast<result_type>(v.Minor);
      }
   };
}

namespace qgl::hashes
{
   constexpr auto VERSION_0_1_HASH =
      std::hash<qgl_version_t>{}(QGL_VERSION_0_1_WIN);

   constexpr auto VERSION_0_2_HASH =
      std::hash<qgl_version_t>{}(QGL_VERSION_0_2_WIN);
}