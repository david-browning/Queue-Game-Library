#pragma once
#include "include/qgl_game_include.h"

namespace qgl::descriptors
{
   enum class screen_spaces : uint32_t
   {
      absolute = 0,
      relative = 1,
   };

#pragma pack(push, 1)
   struct screen_space_descriptor final
   {
      public:
      constexpr screen_space_descriptor()
      {

      }

      constexpr screen_space_descriptor(
         const screen_space_descriptor&) = default;

      constexpr screen_space_descriptor(
         screen_space_descriptor&&) noexcept = default;

      ~screen_space_descriptor() noexcept = default;

      constexpr screen_spaces space() const noexcept
      {
         return static_cast<screen_spaces>(flags.at(SPACE_MODE_IDX));
      }

      constexpr void space(screen_spaces s) noexcept
      {
         if (s == screen_spaces::absolute)
         {
            flags.reset(SPACE_MODE_IDX);
         }
         else
         {
            flags.set(SPACE_MODE_IDX);
         }
      }

      auto top() const noexcept
      {
         return rect[0];
      }

      auto left() const noexcept
      {
         return rect[1];
      }

      auto bottom() const noexcept
      {
         return rect[2];
      }

      auto right() const noexcept
      {
         return rect[3];
      }

      friend void swap(screen_space_descriptor& l,
                       screen_space_descriptor& r) noexcept
      {
         using std::swap;
         swap(l.flags, r.flags);
         swap(l.reserved, r.reserved);
         swap(l.rect, r.rect);
      }

      screen_space_descriptor& operator=(screen_space_descriptor r) noexcept
      {
         swap(*this, r);
         return *this;
      }

      /*
       Bit set of flags
       */
      mem::flags<32> flags = DEFAULT_FLAGS;

      uint32_t reserved = 0;

      /*
       The top left vertex is encoded in the x,y fields. The bottom right
       vertex is encoded in the z, w.
       */
      vector_descriptor rect;

      /*
       Returns this as a Direct2D float rectangle.
       */
      explicit inline operator D2D_RECT_F() const noexcept
      {
         D2D_RECT_F ret;
         ret.top = static_cast<float>(top());
         ret.left = static_cast<float>(left());
         ret.bottom = static_cast<float>(bottom());
         ret.right = static_cast<float>(right());
         return ret;
      }

      private:
      static constexpr uint32_t DEFAULT_FLAGS = 0x01;
      static constexpr size_t SPACE_MODE_IDX = 1;
      static constexpr size_t SPACE_MODE_END_IDX = SPACE_MODE_IDX;
   };
#pragma pack(pop)
}