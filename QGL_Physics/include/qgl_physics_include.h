#pragma once
#include <cstdint>

#ifdef QGL_PHYSICS_EXPORTS
#define QGL_PHYSICS_API __declspec(dllexport)
#define QGL_PHYSICS_TEMPLATE
#else
#define QGL_PHYSICS_API __declspec(dllimport)
#define QGL_PHYSICS_TEMPLATE extern
#endif

namespace qgl::physics
{

   enum class physics_modes : uint8_t
   {
      dynamic = 0,
      phased = 1,
      fixed = 2,
   };
}