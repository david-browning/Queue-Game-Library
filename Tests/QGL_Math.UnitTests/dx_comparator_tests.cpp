#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace qgl;
using namespace qgl::math;

namespace QGL_Math_Unit_Tests
{
   TEST_CLASS(DXComparatorTests)
   {
      public:
      TEST_METHOD(XMFloat2Equal)
      {
         DirectX::XMFLOAT2 v1{ 1.0f, 2.0f };
         DirectX::XMFLOAT2 v2{ -1.0f, 2.0001f };
         DirectX::XMFLOAT2 v3{ 1.0f, 0.0f };
         DirectX::XMFLOAT2 v4{ 0.0f, 2.0f };

         Assert::IsTrue(v1 == v1, L"The vectors should be equal.");
         Assert::IsFalse(v1 == v2, L"The vectors should not be equal.");

         Assert::IsFalse(v1 == v3, L"v1 and v3 should not be equal.");
         Assert::IsFalse(v1 == v4, L"v1 and v4 should not be equal.");
      }

      TEST_METHOD(XMFloat2NotEqual)
      {
         DirectX::XMFLOAT2 v1{ 1.0f, 2.0f };
         DirectX::XMFLOAT2 v2{ -1.0f, 2.0001f };
         DirectX::XMFLOAT2 v3{ 1.0f, 0.0f };
         DirectX::XMFLOAT2 v4{ 0.0f, 2.0f };

         Assert::IsFalse(v1 != v1, L"The vectors should be equal.");
         Assert::IsTrue(v1 != v2, L"The vectors should not be equal.");

         Assert::IsTrue(v1 != v3, L"v1 and v3 should not be equal.");
         Assert::IsTrue(v1 != v4, L"v1 and v4 should not be equal.");
      }

      TEST_METHOD(XMFloat3Equal)
      {
         DirectX::XMFLOAT3 v1{ 1.0f, 2.0f, 5.1f };
         DirectX::XMFLOAT3 v2{ 1.0f, 2.0001f,  5.1f };
         DirectX::XMFLOAT3 v3{ 1.0f, 2.0f, 4.0f };

         Assert::IsTrue(v1 == v1, L"The vectors should be equal.");
         Assert::IsFalse(v1 == v2, L"The vectors should not be equal.");
         Assert::IsFalse(v1 == v3, L"v1 and v3 should not be equal.");
      }

      TEST_METHOD(XMFloat3NotEqual)
      {
         DirectX::XMFLOAT3 v1{ 1.0f, 2.0f, 5.1f };
         DirectX::XMFLOAT3 v2{ 1.0f, 2.0001f,  5.1f };
         DirectX::XMFLOAT3 v3{ 1.0f, 2.0f, 4.0f };

         Assert::IsFalse(v1 != v1, L"The vectors should be equal.");
         Assert::IsTrue(v1 != v2, L"The vectors should not be equal.");
         Assert::IsTrue(v1 != v3, L"v1 and v3 should not be equal.");
      }

      TEST_METHOD(XMFloat4Equal)
      {
         DirectX::XMFLOAT4 v1{ 1.0f, 2.0f, 5.0f, -5.0f };
         DirectX::XMFLOAT4 v2{ 1.0f, 2.0001f,  5.0f, -5.0f };
         DirectX::XMFLOAT4 v3{ 1.0f, 2.0f, 9.0f, -5.0f };
         DirectX::XMFLOAT4 v4{ 1.0f, 2.0f, 5.0f, 1.0f };

         Assert::IsTrue(v1 == v1, L"The vectors should be equal.");
         Assert::IsFalse(v1 == v2, L"The vectors should not be equal.");
         Assert::IsFalse(v1 == v3, L"v1 and v3 should not be equal.");
         Assert::IsFalse(v1 == v4, L"v1 and v4 should not be equal.");
      }

      TEST_METHOD(XMFloat4NotEqual)
      {
         DirectX::XMFLOAT4 v1{ 1.0f, 2.0f, 5.0f, -5.0f };
         DirectX::XMFLOAT4 v2{ 1.0f, 2.0001f,  5.0f, -5.0f };
         DirectX::XMFLOAT4 v3{ 1.0f, 2.0f, 9.0f, -5.0f };
         DirectX::XMFLOAT4 v4{ 1.0f, 2.0f, 5.0f, 1.0f };

         Assert::IsFalse(v1 != v1, L"The vectors should be equal.");
         Assert::IsTrue(v1 != v2, L"The vectors should not be equal.");
         Assert::IsTrue(v1 != v3, L"v1 and v3 should not be equal.");
         Assert::IsTrue(v1 != v4, L"v1 and v4 should not be equal.");
      }

      TEST_METHOD(DXBoundingSphereEqual)
      {
         DirectX::XMFLOAT3 v1{ 1.0f, 2.0f, 5.1f };
         DirectX::XMFLOAT3 v2{ 1.0f, 2.0001f,  5.1f };

         DirectX::BoundingSphere s1{ v1, 3.0f };
         DirectX::BoundingSphere s2{ v1, 2.0f };
         DirectX::BoundingSphere s3{ v2, 3.0f };

         Assert::IsTrue(s1 == s1, L"The bounding spheres should be equal.");
         Assert::IsFalse(s1 == s2, L"s1 and s2 should not be equal.");
         Assert::IsFalse(s1 == s3, L"s1 and s3 should not be equal.");
      }

      TEST_METHOD(DXBoundingSphereNotEqual)
      {
         DirectX::XMFLOAT3 v1{ 1.0f, 2.0f, 5.1f };
         DirectX::XMFLOAT3 v2{ 1.0f, 2.0001f,  5.1f };

         DirectX::BoundingSphere s1{ v1, 3.0f };
         DirectX::BoundingSphere s2{ v1, 2.0f };
         DirectX::BoundingSphere s3{ v2, 3.0f };

         Assert::IsFalse(s1 != s1, L"The bounding spheres should be equal.");
         Assert::IsTrue(s1 != s2, L"s1 and s2 should not be equal.");
         Assert::IsTrue(s1 != s3, L"s1 and s3 should not be equal.");
      }
   };
}