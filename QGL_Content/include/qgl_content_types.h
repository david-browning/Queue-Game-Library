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

      //Basic Type Related
      RESOURCE_TYPE_STRING = 0x0011,
      RESOURCE_TYPE_INTEGER = 0x0012,
      RESOURCE_TYPE_FLOAT = 0x0013,
      RESOURCE_TYPE_DESCRIPTION = 0x0014,

      //Graphics Types
      RESOURCE_TYPE_GEOMETRY = 0x0021,
      RESOURCE_TYPE_TEXTURE = 0x0022,
      RESOURCE_TYPE_SHADER = 0x0023,

      RESOURCE_TYPE_LIGHT = 0x0031,
      RESOURCE_TYPE_CAMERA = 0x0032,
      RESOURCE_TYPE_BRUSH = 0x0033,

      RESOURCE_TYPE_SAMPLER = 0x0034,
      RESOURCE_TYPE_TEXT_FORMAT = 0x0035,
      RESOURCE_TYPE_DEPTH_STENCIL = 0x0036,
      RESOURCE_TYPE_RASTERIZER = 0x0037,


      //Physics Types


      //Sound Types

      //AI Types

      //Other Types
      RESOURCE_TYPE_GRAPHICS_CONFIG,

   };

   enum CONTENT_LOADER_IDS : uint16_t
   {
      CONTENT_LOADER_ID_UNKNOWN = 0,

      //Basic Types
      CONTENT_LOADER_ID_STRING = 0x1101,
      CONTENT_LOADER_ID_WSTRING = 0x1102,

      CONTENT_LOADER_ID_INT8 = 0x1201,
      CONTENT_LOADER_ID_INT16 = 0x1202,
      CONTENT_LOADER_ID_INT32 = 0x1203,
      CONTENT_LOADER_ID_INT64 = 0x1204,

      CONTENT_LOADER_ID_UINT8 = 0x1281,
      CONTENT_LOADER_ID_UINT16 = 0x1282,
      CONTENT_LOADER_ID_UINT32 = 0x1283,
      CONTENT_LOADER_ID_UINT64 = 0x1284,

      CONTENT_LOADER_ID_FLOAT32 = 0x1301,
      CONTENT_LOADER_ID_FLOAT64 = 0x1302,

      //CONTENT_LOADER_ID_FLOAT2,
      //CONTENT_LOADER_ID_FLOAT3,
      //CONTENT_LOADER_ID_FLOAT4,
      //CONTENT_LOADER_ID_FLOAT2_LIST,
      //CONTENT_LOADER_ID_FLOAT3_LIST,
      //CONTENT_LOADER_ID_FLOAT4_LIST,


      //Graphics Types
      CONTENT_LOADER_ID_DDS = 0x2201,
      //PNG?


      //CONTENT_LOADER_ID_OBJ,
      //CONTENT_LOADER_ID_FBX,
      CONTENT_LOADER_ID_LIGHT,
      CONTENT_LOADER_ID_CAMERA,
      CONTENT_LOADER_ID_WTEXT,
      CONTENT_LOADER_ID_DEPTH_STENCIL,
      CONTENT_LOADER_ID_RASTERIZER,
      CONTENT_LOADER_ID_GRAPHICS_CONFIG,




      CONTENT_LOADER_ID_TEXT_FORMAT,
      CONTENT_LOADER_ID_BRUSH,
      CONTENT_LOADER_ID_SAMPLER,

      CONTENT_LOADER_ID_SHADER_SOURCE,
      CONTENT_LOADER_ID_SHADER_COMPILED,



      //Physics Types
      //CONTENT_LOADER_ID_AABB,
      //CONTENT_LOADER_ID_SPHERE,

      //Sound Types

      //AI Types

      //Other Types
      CONTENT_LOADER_ID_STRUCT,
   };
}