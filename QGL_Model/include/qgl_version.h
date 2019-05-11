#pragma once
#include "include/qgl_model_include.h"

namespace qgl
{
   static constexpr uint8_t QGL_VERSION_MAJOR_0 = 0;
   static constexpr uint8_t QGL_VERSION_MAJOR_1 = 1;
   static constexpr uint8_t QGL_VERSION_MINOR_0 = 0;
   static constexpr uint8_t QGL_VERSION_MINOR_1 = 1;

   static constexpr uint8_t QGL_VERSION_MAJOR_LATEST = QGL_VERSION_MAJOR_0;
   static constexpr uint8_t QGL_VERSION_MINOR_LATEST = QGL_VERSION_MINOR_1;

   /*
    Bit layout: {2 OS Flag} { 14 Flags} {8 Major} {8 Minor}
    */
   typedef uint32_t qgl_version_t;

   enum class QGL_OS_VERSION_FLAGS : uint8_t
   {
      OS_UNKNOWN = 0,
      OS_WINDOWS = 1,
      OS_LINUX = 2,
      OS_MAC = 3
   };

   //////////Masks for reading from a QGL version//////////
   static constexpr uint8_t QGL_VERSION_OS_FLAGS_MASK = 0xC;
   static constexpr uint16_t QGL_VERSION_FLAGS_MASK = 0x3FFF;
   static constexpr uint8_t QGL_VERSION_8_BIT_MASK = 0xFF;
   ///////////////////////////////////////////////////////

   inline uint8_t version_major(_IN_ qgl_version_t version)
   {
      return (version >> 8) & QGL_VERSION_8_BIT_MASK;
   }

   inline uint8_t version_minor(_IN_ qgl_version_t version)
   {
      return version & QGL_VERSION_8_BIT_MASK;
   }

   inline uint16_t version_flags(_IN_ qgl_version_t version)
   {
      return (version >> 16) & QGL_VERSION_FLAGS_MASK;
   }

   inline QGL_OS_VERSION_FLAGS version_os(_IN_ qgl_version_t version)
   {
      return (QGL_OS_VERSION_FLAGS)((version >> 30) & QGL_VERSION_OS_FLAGS_MASK);
   }

   constexpr qgl_version_t make_win_version(const uint8_t versionMajor,
                                            const uint8_t versionMinor)
   {
      return (uint32_t(QGL_OS_VERSION_FLAGS::OS_WINDOWS) << 31) |
         (0 << 15) |
         (uint32_t(versionMajor) << 7) |
         uint32_t(versionMinor);
   }

   static constexpr qgl_version_t QGL_VERSION_0_1_WIN =
      make_win_version(QGL_VERSION_MAJOR_0,
                       QGL_VERSION_MINOR_1);

   static constexpr qgl_version_t QGL_VERSION_0_2_WIN =
      make_win_version(QGL_VERSION_MAJOR_0,
                       2);

   static constexpr qgl_version_t QGL_VERSION_LATEST = QGL_VERSION_0_2_WIN;
}