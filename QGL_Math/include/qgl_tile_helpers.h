#pragma once
#include "include/qgl_math_includes.h"
#include "include/qgl_math_funcs.h"
#include <DirectXMath.h>
#include <ratio>
#include <type_traits>

/*
 This namespace provides helper functions to convert between physical units,
 such as meters, and tile units which are found in isometric games.
 Geometry that is encoded in physical units like models or collision boundaries,
 can be scaled to a tile based unit system with these helpers.

 The helper functions are encapsulated in a class because they depend so much
 on templates. Making them all free functions would create bloated code.
 */
namespace qgl::math::tiles
{
   /*
    Converts physical units (like meters) to tile units based on a conversion
    factor. The conversion factor is a compile-time constant.

    TileT: The data type of the tile units. Must be an integer-like type.

    PhysicalT: The data type of the physical units. This should probably be
      a floating point type.

    Physical: The number of physical units in a tile unit.
      Ex: If a tile is 2 meters wide, then this value should be "2".

    Tile: The denominator in the physical to tile ratio. By default this is 1,
      but a the ratio can be inverted by specifying a number larger than
      "Physical".

    */
   template<
      typename TileT,
      typename PhysicalT,
      std::intmax_t Physical,
      std::intmax_t Tile = 1>
      class tile_helpers
   {
      private:
      /*
       Assert that the template parameters are correct.
       */
      static_assert(std::is_integral<TileT>::value,
                    "TileT must be integral.");

      static_assert(std::is_floating_point<PhysicalT>::value,
                    "PhysicalT must be a floating point data type.");

      typedef std::integral_constant<std::intmax_t, Tile> DenomConstant;
      typedef std::integral_constant <std::intmax_t, 0> ZeroConstant;
      static_assert(DenomConstant::value != ZeroConstant::value,
                    "Denominator cannot be zero.");

      public:

      /*
       Returns the physical to tile ratio.
       */
      static constexpr std::ratio<Physical, Tile> tile_ratio()
      {
         return std::ratio<Physical, Tile> {};
      }

      /*
       Returns the physical to tile ratio as a real number.
       */
      static constexpr PhysicalT real() noexcept
      {
         return static_cast<PhysicalT>(Physical) / static_cast<PhysicalT>(Tile);
      }

      /*
       Returns the tile to physical ratio as a real number.
       */
      static constexpr PhysicalT ireal() noexcept
      {
         return static_cast<PhysicalT>(Tile) / static_cast<PhysicalT>(Physical);
      }

      /*
       Converts a physical unit to tile space. The result is rounded up to the
       nearest tile.
       */
      static TileT to_tiles(PhysicalT p) noexcept
      {
         return math::ceiling<TileT>(ireal() * p);
      }

      /*
       Converts a vector of physical units to a vector in tile space.
       Each element is rounded up to the nearest tile.
       */
      static DirectX::XMVECTOR XM_CALLCONV to_tiles(DirectX::FXMVECTOR p) noexcept
      {
         return math::ceiling<DirectX::XMVECTOR>(DirectX::operator*(
            ireal(), p));
      }

      /*
       Creates a scale transform matrix that when multiplied by a physical
       scaled vector, scales the vector to tile space.
       */
      static DirectX::XMMATRIX XM_CALLCONV tile_scale() noexcept
      {
         return DirectX::XMMatrixScaling(ireal(),
                                         ireal(),
                                         ireal());
      }
   };
}