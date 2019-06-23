#pragma once
#include "include/qgl_content_include.h"

namespace qgl::content
{
   using id_t = uint64_t;
   static constexpr id_t INVALID_CONTENT_ID = static_cast<id_t>(-1);

   static constexpr uint64_t QGL_CONTENT_PROJECT_MAGIC_NUMBER =
      0x8A473F4DB11D8CE7;

   static constexpr uint64_t QGL_CONTENT_PROJECT_ENTRY_SEPERATOR_MAGIC_NUMBER =
      0xF6C3A57BD87C798B;

   static constexpr uint64_t QGL_CONTENT_FILE_MAGIC_NUMBER = 
      0x0E72B375C1C649159;

   enum RESOURCE_TYPES : uint16_t
   {
      RESOURCE_TYPE_UNKNOWN = 0,

      #pragma region Basic Types
      RESOURCE_TYPE_STRING = 0x0101,
      RESOURCE_TYPE_INTEGER = 0x0102,
      RESOURCE_TYPE_FLOAT = 0x0103,
      RESOURCE_TYPE_DESCRIPTION = 0x0104,
      RESOURCE_TYPE_BOOL = 0x0105,
      RESOURCE_TYPE_GUID = 0x0106,
      #pragma endregion

      #pragma region Render Resources
      RESOURCE_TYPE_GEOMETRY = 0x0201,
      RESOURCE_TYPE_TEXTURE = 0x0202,
      RESOURCE_TYPE_SHAPE = 0x0203,

      RESOURCE_TYPE_LIGHT = 0x0211,
      RESOURCE_TYPE_CAMERA = 0x0212,
      RESOURCE_TYPE_BRUSH = 0x0213,
      RESOURCE_TYPE_TEXT_FORMAT = 0x0214,
      #pragma endregion

      #pragma region GPU Configuration Types
      RESOURCE_TYPE_GRAPHICS_CONFIG = 0x0401,

      RESOURCE_TYPE_SHADER = 0x0411,
      RESOURCE_TYPE_SAMPLER = 0x0412,

      RESOURCE_TYPE_DEPTH_STENCIL = 0x0421,
      RESOURCE_TYPE_RASTERIZER = 0x0422,
      RESOURCE_TYPE_BLENDER = 0x0423,
      
      RESOURCE_TYPE_PSO = 0x0431,
      #pragma endregion


      //Physics Types


      //Sound Types

      //AI Types

      //Other Types

   };

   enum CONTENT_LOADER_IDS : uint16_t
   {
      CONTENT_LOADER_ID_UNKNOWN = 0,

      #pragma region Basic datatypes
      CONTENT_LOADER_ID_STRING = 0x1101,
      CONTENT_LOADER_ID_WSTRING = 0x1102,
      CONTENT_LOADER_ID_BOOL = 0x1103,
      CONTENT_LOADER_ID_STRUCT = 0x1104,
      CONTENT_LOADER_ID_HEADER = 0x1105,
      #pragma endregion

      #pragma region Numeric datatypes.
      CONTENT_LOADER_ID_INT8 = 0x1201,
      CONTENT_LOADER_ID_INT16 = 0x1202,
      CONTENT_LOADER_ID_INT32 = 0x1203,
      CONTENT_LOADER_ID_INT64 = 0x1204,

      CONTENT_LOADER_ID_UINT8 = 0x1281,
      CONTENT_LOADER_ID_UINT16 = 0x1282,
      CONTENT_LOADER_ID_UINT32 = 0x1283,
      CONTENT_LOADER_ID_UINT64 = 0x1284,

      CONTENT_LOADER_ID_RATIONAL = 0x1301,
      CONTENT_LOADER_ID_FLOAT32 = 0x1302,
      CONTENT_LOADER_ID_FLOAT64 = 0x1303,
      #pragma endregion

      #pragma region Texture Loaders
      CONTENT_LOADER_ID_DDS = 0x2101,
      #pragma endregion

      #pragma region Geometry Loaders
      CONTENT_LOADER_ID_OBJ = 0x2201,

      CONTENT_LOADER_ID_SPHERE = 0x2211,
      CONTENT_LOADER_ID_AABB = 0x2212,

      CONTENT_LOADER_ID_RECTANGLE = 0x2221,
      #pragma endregion

      #pragma region Render Resources
      CONTENT_LOADER_ID_LIGHT = 0x2301,
      CONTENT_LOADER_ID_CAMERA = 0x2311,
      CONTENT_LOADER_ID_BRUSH = 0x2321,
      CONTENT_LOADER_ID_TEXT_FORMAT = 0x2331,
      #pragma endregion

      #pragma region Text Rendering
      CONTENT_LOADER_ID_WTEXT = 0x2401,
      #pragma endregion

      #pragma region Graphics Configuration Loaders
      CONTENT_LOADER_ID_GRAPHICS_CONFIG = 0x2501,

      CONTENT_LOADER_ID_SHADER_SOURCE = 0x2511,
      CONTENT_LOADER_ID_SHADER_COMPILED = 0x2512,
      CONTENT_LOADER_ID_SAMPLER = 0x2521,
      CONTENT_LOADER_ID_VERTEX_ELEMENT = 0x2531,
      CONTENT_LOADER_ID_VERTEX_DESCRIPTION = 0x2532,

      CONTENT_LOADER_ID_DEPTH_STENCIL = 0x2541,
      CONTENT_LOADER_ID_RASTERIZER = 0x2542,
      CONTENT_LOADER_ID_BLENDER = 0x2543,

      CONTENT_LOADER_ID_PSO = 0x2551,
      #pragma endregion

      CONTENT_LOADER_ID_MULTISAMPLE_DESCRIPTION,

      //Components
      CONTENT_LOADER_ID_RENDER_COMPONENT,
   };
}