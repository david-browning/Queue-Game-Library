#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace qgl;
using namespace qgl::math;
using namespace qgl::math::tiles;

namespace QGL_Math_Unit_Tests
{
   TEST_CLASS(TileHelperTests)
   {
      TEST_METHOD(ToTiles1D)
      {
         // A tile is made of 2 physical units.
         using float2Helper = typename tile_helpers<int, float, 2>;

         Assert::AreEqual(1, float2Helper::to_tiles(2.0f),
                          L"2 meters did not scale to 1 tile.");

         // 1/2 physical unit per tile.
         using floatHalfHelper = typename tile_helpers<int, float, 1, 2>;
         Assert::AreEqual(1, floatHalfHelper::to_tiles(0.5),
                          L"1/2 meters did not scale to 1 tile.");

         Assert::AreEqual(4, floatHalfHelper::to_tiles(2.0f),
                          L"2 meters did not scale to 4 tiles.");
      }

      TEST_METHOD(ToTilesVector)
      {
         // A tile is made of 2 physical units.
         using float2Helper = typename tile_helpers<int, float, 2>;

         auto x1 = DirectX::XMVectorSet(1.0f, 2.0f, 3.0f, 4.0f);
         auto x1X = DirectX::XMVectorSet(1.0f, 1.0f, 2.0f, 2.0f);


         using helper = typename tile_helpers<int, float, 2>;

         Assert::IsTrue(x1X == helper::to_tiles(x1),
                        L"X1 was not transformed properly.");
      }

      TEST_METHOD(ScaleXForm)
      {
         // 2 tiles per physical unit
         using floatHalfHelper = typename tile_helpers<int, float, 1, 2>;
         auto xForm = floatHalfHelper::tile_scale();

         auto expected = DirectX::XMMatrixSet(
            2.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 2.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 2.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
         );

         Assert::AreEqual(expected, xForm);
      }
   };
}