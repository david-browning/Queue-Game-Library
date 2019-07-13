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

   /*
    Bit layout: {2 OS Flag} { 14 Flags} {8 Major} {8 Minor}
    */
   using qgl_version_t = uint32_t;

   static_assert(sizeof(qgl_version_t) == 4,
                 "QGL versions must be 4 bytes.");

   /*
    Returns the OS flags from version.
    */
   inline QGL_OS_VERSION_FLAGS version_os(qgl_version_t version)
   {
      return static_cast<QGL_OS_VERSION_FLAGS>((version & 0xC000'0000) >> 30);
   }

   /*
    Returns the major version stored in a qgl_version_t.
    */
   inline constexpr uint8_t version_major(qgl_version_t v)
   {
      return (v >> 8) & 0x0000'00FF;
   }

   /*
    Returns the minor version stored in a qgl_version_t.
    */
   inline constexpr uint8_t version_minor(qgl_version_t v)
   {
      return v & 0x0000'00FF;
   }

   /*
    Creates a qgl_version_t.
    */
   inline constexpr qgl_version_t make_version(uint8_t versionMajor,
                                               uint8_t versionMinor,
                                               QGL_OS_VERSION_FLAGS osVersion)
   {
      return
         (static_cast<qgl_version_t>(osVersion) << 30) |
         (versionMajor << 8) |
         (versionMinor);
   }

   /*
    Creates a qgl version where the OS flag is Windows.
    */
   inline constexpr qgl_version_t make_win_version(uint8_t versionMajor,
                                                   uint8_t versionMinor)
   {
      return make_version(versionMajor, 
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
   /*
    The hash of a version is an 8-byte value where only the bottom 2 bytes are
    set. The lowest byte is a version's minor. The next byte is the version's
    major.
    */
   template<> struct hash<qgl::qgl_version_t>
   {
      constexpr hash()
      {

      }

      typedef qgl::qgl_version_t argument_type;
      typedef std::size_t result_type;
      constexpr result_type operator()(const argument_type& v) const noexcept
      {
         return (static_cast<result_type>(qgl::version_major(v)) << CHAR_BIT) |
            static_cast<result_type>(qgl::version_minor(v));
      }
   };
}

namespace qgl::hashes
{
   static constexpr auto VERSION_0_1_HASH =
      std::hash<qgl_version_t>{}(QGL_VERSION_0_1_WIN);

   static constexpr auto VERSION_0_2_HASH =
      std::hash<qgl_version_t>{}(QGL_VERSION_0_2_WIN);
}