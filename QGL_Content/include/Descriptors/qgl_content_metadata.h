#pragma once
#include "include/qgl_content_include.h"
#include "include/Compression/qgl_compression.h"
#include <QGLPhysics.h>

namespace qgl::descriptors
{
   class metadata_flags
   {
      public:
      static constexpr size_t SHARED_FLAG_IDX = 0;

      static constexpr size_t SHARE_MODE_FLAG_IDX = SHARED_FLAG_IDX + 1;

      static constexpr size_t SHARE_MODE_FLAG_END = SHARE_MODE_FLAG_IDX + 3;

      static constexpr size_t VISIBLE_FLAG_IDX = SHARE_MODE_FLAG_END + 1;

      static constexpr size_t PHYSICS_MODE_FLAG_IDX = VISIBLE_FLAG_IDX + 1;

      static constexpr size_t PHYSICS_MODE_FLAG_END = PHYSICS_MODE_FLAG_IDX + 3;

      static constexpr size_t COMPRESSION_FLAG_IDX = PHYSICS_MODE_FLAG_END + 1;

      static constexpr size_t COMPRESSION_FLAG_END = COMPRESSION_FLAG_IDX + 3;

      static constexpr size_t COMPRESSION_TYPE_IDX = COMPRESSION_FLAG_END + 1;

      static constexpr size_t COMPRESSION_TYPE_END = COMPRESSION_TYPE_IDX + 5;
   };

   static constexpr size_t MAX_METADATA_NAME_LEN = 64;

   /*
    Describes the metadata for a piece of content.
    */
#pragma pack(push, 1)
   struct content_metadata final
   {
      constexpr content_metadata()
      {

      }

      content_metadata(const content_metadata&) = default;

      content_metadata(content_metadata&&) noexcept = default;

      ~content_metadata() noexcept = default;

      friend void swap(content_metadata& l, content_metadata& r) noexcept
      {
         using std::swap;
         swap(l.flags, r.flags);
         swap(l.id, r.id);
         swap(l.loader, r.loader);
         swap(l.name, r.name);
         swap(l.version, r.version);
      }

      content_metadata& operator=(content_metadata r) noexcept
      {
         swap(*this, r);
         return *this;
      }

      constexpr bool shared() const noexcept
      {
         return flags.at(metadata_flags::SHARED_FLAG_IDX);
      }

      void shared(bool enable) noexcept
      {
         flags.set(metadata_flags::SHARED_FLAG_IDX, enable);
      }

      constexpr bool visible() const noexcept
      {
         return flags.at(metadata_flags::VISIBLE_FLAG_IDX);
      }

      void visible(bool enable) noexcept
      {
         flags.set(metadata_flags::VISIBLE_FLAG_IDX, enable);
      }

      constexpr physics::physics_modes physics_mode() const noexcept
      {
         return static_cast<physics::physics_modes>(
            flags.range_shift<metadata_flags::PHYSICS_MODE_FLAG_IDX,
            metadata_flags::PHYSICS_MODE_FLAG_END>());
      }

      void physics_mode(physics::physics_modes f) noexcept
      {
         flags.reset(metadata_flags::PHYSICS_MODE_FLAG_IDX,
                     metadata_flags::PHYSICS_MODE_FLAG_END);

         flags |= (static_cast<decltype(flags)::type>(f) <<
                   metadata_flags::PHYSICS_MODE_FLAG_IDX);
      }

      constexpr auto compression_flags() const noexcept
      {
         return static_cast<content::compression::compression_flags>(
            flags.range_shift<metadata_flags::COMPRESSION_FLAG_IDX,
                              metadata_flags::COMPRESSION_FLAG_END>());
      }

      void compression_flags(
         content::compression::compression_flags f) noexcept
      {
         // Zero out the compression flags and OR the argument in.
         flags.reset(metadata_flags::COMPRESSION_FLAG_IDX,
                     metadata_flags::COMPRESSION_FLAG_END);

         flags |= (static_cast<decltype(flags)::type>(f) <<
                   metadata_flags::COMPRESSION_FLAG_IDX);
      }

      constexpr auto compression_type() const noexcept
      {
         return static_cast<content::compression::compression_types>(
            flags.range_shift<metadata_flags::COMPRESSION_TYPE_IDX,
                              metadata_flags::COMPRESSION_TYPE_END>());
      }

      void compression_type(
         content::compression::compression_types f) noexcept
      {
         // Zero out the compression type and OR the argument in.
         flags.reset(metadata_flags::COMPRESSION_TYPE_IDX,
                     metadata_flags::COMPRESSION_TYPE_END);

         flags |= (static_cast<decltype(flags)::type>(f) <<
                   metadata_flags::COMPRESSION_TYPE_IDX);
      }

      /*
       Version the content was compiled.
       */
      qgl_version version = QGL_VERSION_0_1_WIN;

      /*
       { 1 is shared } { 3 share mode } { 1 is visible } { 3 physics mode }
       { 3 compression flags. 0 means no compression } { 5 compression type }
       */
      mem::flags<32, true> flags = 0;

      /*
       Content ID.
       */
      guid id;

      /*
       ID of the loader to use for this content.
       */
      guid loader;

      /*
       Name of the content.
       */
      fixed_buffer<char, MAX_METADATA_NAME_LEN> name;
   };
#pragma pack(pop)
}