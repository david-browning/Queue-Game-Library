#include "pch.h"
#include "include/qgl_dds.h"

using namespace qgl::graphics::low;
using namespace qgl::graphics;

#pragma pack(push,1)

const uint32_t DDS_MAGIC = 0x20534444; // "DDS "

enum DDS_LOADER_FLAGS
{
   DDS_LOADER_DEFAULT = 0,
   DDS_LOADER_FORCE_SRGB = 0x1,
   DDS_LOADER_MIP_RESERVE = 0x8,
};

#define DDS_FOURCC      0x00000004  // DDPF_FOURCC
#define DDS_RGB         0x00000040  // DDPF_RGB
#define DDS_LUMINANCE   0x00020000  // DDPF_LUMINANCE
#define DDS_ALPHA       0x00000002  // DDPF_ALPHA
#define DDS_BUMPDUDV    0x00080000  // DDPF_BUMPDUDV

#define DDS_HEADER_FLAGS_VOLUME         0x00800000  // DDSD_DEPTH

#define DDS_HEIGHT 0x00000002 // DDSD_HEIGHT
#define DDS_WIDTH  0x00000004 // DDSD_WIDTH

#define DDS_CUBEMAP_POSITIVEX 0x00000600 // DDSCAPS2_CUBEMAP | DDSCAPS2_CUBEMAP_POSITIVEX
#define DDS_CUBEMAP_NEGATIVEX 0x00000a00 // DDSCAPS2_CUBEMAP | DDSCAPS2_CUBEMAP_NEGATIVEX
#define DDS_CUBEMAP_POSITIVEY 0x00001200 // DDSCAPS2_CUBEMAP | DDSCAPS2_CUBEMAP_POSITIVEY
#define DDS_CUBEMAP_NEGATIVEY 0x00002200 // DDSCAPS2_CUBEMAP | DDSCAPS2_CUBEMAP_NEGATIVEY
#define DDS_CUBEMAP_POSITIVEZ 0x00004200 // DDSCAPS2_CUBEMAP | DDSCAPS2_CUBEMAP_POSITIVEZ
#define DDS_CUBEMAP_NEGATIVEZ 0x00008200 // DDSCAPS2_CUBEMAP | DDSCAPS2_CUBEMAP_NEGATIVEZ

#define DDS_CUBEMAP_ALLFACES ( DDS_CUBEMAP_POSITIVEX | DDS_CUBEMAP_NEGATIVEX |\
                               DDS_CUBEMAP_POSITIVEY | DDS_CUBEMAP_NEGATIVEY |\
                               DDS_CUBEMAP_POSITIVEZ | DDS_CUBEMAP_NEGATIVEZ )

#define DDS_CUBEMAP 0x00000200 // DDSCAPS2_CUBEMAP

enum DDS_MISC_FLAGS2
{
   DDS_MISC_FLAGS2_ALPHA_MODE_MASK = 0x7L,
};

struct DDS_PIXELFORMAT
{
   uint32_t    size;
   uint32_t    flags;
   uint32_t    fourCC;
   uint32_t    RGBBitCount;
   uint32_t    RBitMask;
   uint32_t    GBitMask;
   uint32_t    BBitMask;
   uint32_t    ABitMask;
};

struct DDS_HEADER
{
   uint32_t        size;
   uint32_t        flags;
   uint32_t        height;
   uint32_t        width;
   uint32_t        pitchOrLinearSize;
   uint32_t        depth; // only if DDS_HEADER_FLAGS_VOLUME is set in flags
   uint32_t        mipMapCount;
   uint32_t        reserved1[11];
   DDS_PIXELFORMAT ddspf;
   uint32_t        caps;
   uint32_t        caps2;
   uint32_t        caps3;
   uint32_t        caps4;
   uint32_t        reserved2;
};

struct DDS_HEADER_DXT10
{
   DXGI_FORMAT     dxgiFormat;
   uint32_t        resourceDimension;
   uint32_t        miscFlag; // see D3D11_RESOURCE_MISC_FLAG
   uint32_t        arraySize;
   uint32_t        miscFlags2;
};

#pragma pack(pop)

constexpr uint32_t make_4_cc(uint32_t ch0, uint32_t ch1, uint32_t ch2, uint32_t ch3)
{
   return ((uint32_t)(uint8_t)(ch0) | ((uint32_t)(uint8_t)(ch1) << 8) |
      ((uint32_t)(uint8_t)(ch2) << 16) | ((uint32_t)(uint8_t)(ch3) << 24));
}

inline bool is_bit_mask(const DDS_PIXELFORMAT& ddpf, UINT r, UINT g, UINT b, UINT a)
{
   return (ddpf.RBitMask == r && ddpf.GBitMask == g && ddpf.BBitMask == b && ddpf.ABitMask == a);
}

inline uint32_t CountMips(uint32_t width, uint32_t height)
{
   if (width == 0 || height == 0)
      return 0;

   uint32_t count = 1;
   while (width > 1 || height > 1)
   {
      width >>= 1;
      height >>= 1;
      count++;
   }
   return count;
}

size_t BitsPerPixel(_In_ DXGI_FORMAT fmt)
{
   switch (fmt)
   {
      case DXGI_FORMAT_R32G32B32A32_TYPELESS:
      case DXGI_FORMAT_R32G32B32A32_FLOAT:
      case DXGI_FORMAT_R32G32B32A32_UINT:
      case DXGI_FORMAT_R32G32B32A32_SINT:
         return 128;

      case DXGI_FORMAT_R32G32B32_TYPELESS:
      case DXGI_FORMAT_R32G32B32_FLOAT:
      case DXGI_FORMAT_R32G32B32_UINT:
      case DXGI_FORMAT_R32G32B32_SINT:
         return 96;

      case DXGI_FORMAT_R16G16B16A16_TYPELESS:
      case DXGI_FORMAT_R16G16B16A16_FLOAT:
      case DXGI_FORMAT_R16G16B16A16_UNORM:
      case DXGI_FORMAT_R16G16B16A16_UINT:
      case DXGI_FORMAT_R16G16B16A16_SNORM:
      case DXGI_FORMAT_R16G16B16A16_SINT:
      case DXGI_FORMAT_R32G32_TYPELESS:
      case DXGI_FORMAT_R32G32_FLOAT:
      case DXGI_FORMAT_R32G32_UINT:
      case DXGI_FORMAT_R32G32_SINT:
      case DXGI_FORMAT_R32G8X24_TYPELESS:
      case DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
      case DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS:
      case DXGI_FORMAT_X32_TYPELESS_G8X24_UINT:
      case DXGI_FORMAT_Y416:
      case DXGI_FORMAT_Y210:
      case DXGI_FORMAT_Y216:
         return 64;

      case DXGI_FORMAT_R10G10B10A2_TYPELESS:
      case DXGI_FORMAT_R10G10B10A2_UNORM:
      case DXGI_FORMAT_R10G10B10A2_UINT:
      case DXGI_FORMAT_R11G11B10_FLOAT:
      case DXGI_FORMAT_R8G8B8A8_TYPELESS:
      case DXGI_FORMAT_R8G8B8A8_UNORM:
      case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
      case DXGI_FORMAT_R8G8B8A8_UINT:
      case DXGI_FORMAT_R8G8B8A8_SNORM:
      case DXGI_FORMAT_R8G8B8A8_SINT:
      case DXGI_FORMAT_R16G16_TYPELESS:
      case DXGI_FORMAT_R16G16_FLOAT:
      case DXGI_FORMAT_R16G16_UNORM:
      case DXGI_FORMAT_R16G16_UINT:
      case DXGI_FORMAT_R16G16_SNORM:
      case DXGI_FORMAT_R16G16_SINT:
      case DXGI_FORMAT_R32_TYPELESS:
      case DXGI_FORMAT_D32_FLOAT:
      case DXGI_FORMAT_R32_FLOAT:
      case DXGI_FORMAT_R32_UINT:
      case DXGI_FORMAT_R32_SINT:
      case DXGI_FORMAT_R24G8_TYPELESS:
      case DXGI_FORMAT_D24_UNORM_S8_UINT:
      case DXGI_FORMAT_R24_UNORM_X8_TYPELESS:
      case DXGI_FORMAT_X24_TYPELESS_G8_UINT:
      case DXGI_FORMAT_R9G9B9E5_SHAREDEXP:
      case DXGI_FORMAT_R8G8_B8G8_UNORM:
      case DXGI_FORMAT_G8R8_G8B8_UNORM:
      case DXGI_FORMAT_B8G8R8A8_UNORM:
      case DXGI_FORMAT_B8G8R8X8_UNORM:
      case DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM:
      case DXGI_FORMAT_B8G8R8A8_TYPELESS:
      case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:
      case DXGI_FORMAT_B8G8R8X8_TYPELESS:
      case DXGI_FORMAT_B8G8R8X8_UNORM_SRGB:
      case DXGI_FORMAT_AYUV:
      case DXGI_FORMAT_Y410:
      case DXGI_FORMAT_YUY2:
         return 32;

      case DXGI_FORMAT_P010:
      case DXGI_FORMAT_P016:
      case DXGI_FORMAT_V408:
         return 24;

      case DXGI_FORMAT_R8G8_TYPELESS:
      case DXGI_FORMAT_R8G8_UNORM:
      case DXGI_FORMAT_R8G8_UINT:
      case DXGI_FORMAT_R8G8_SNORM:
      case DXGI_FORMAT_R8G8_SINT:
      case DXGI_FORMAT_R16_TYPELESS:
      case DXGI_FORMAT_R16_FLOAT:
      case DXGI_FORMAT_D16_UNORM:
      case DXGI_FORMAT_R16_UNORM:
      case DXGI_FORMAT_R16_UINT:
      case DXGI_FORMAT_R16_SNORM:
      case DXGI_FORMAT_R16_SINT:
      case DXGI_FORMAT_B5G6R5_UNORM:
      case DXGI_FORMAT_B5G5R5A1_UNORM:
      case DXGI_FORMAT_A8P8:
      case DXGI_FORMAT_B4G4R4A4_UNORM:
      case DXGI_FORMAT_P208:
      case DXGI_FORMAT_V208:
         return 16;

      case DXGI_FORMAT_NV12:
      case DXGI_FORMAT_420_OPAQUE:
      case DXGI_FORMAT_NV11:
         return 12;

      case DXGI_FORMAT_R8_TYPELESS:
      case DXGI_FORMAT_R8_UNORM:
      case DXGI_FORMAT_R8_UINT:
      case DXGI_FORMAT_R8_SNORM:
      case DXGI_FORMAT_R8_SINT:
      case DXGI_FORMAT_A8_UNORM:
      case DXGI_FORMAT_AI44:
      case DXGI_FORMAT_IA44:
      case DXGI_FORMAT_P8:
         return 8;

      case DXGI_FORMAT_R1_UNORM:
         return 1;

      case DXGI_FORMAT_BC1_TYPELESS:
      case DXGI_FORMAT_BC1_UNORM:
      case DXGI_FORMAT_BC1_UNORM_SRGB:
      case DXGI_FORMAT_BC4_TYPELESS:
      case DXGI_FORMAT_BC4_UNORM:
      case DXGI_FORMAT_BC4_SNORM:
         return 4;

      case DXGI_FORMAT_BC2_TYPELESS:
      case DXGI_FORMAT_BC2_UNORM:
      case DXGI_FORMAT_BC2_UNORM_SRGB:
      case DXGI_FORMAT_BC3_TYPELESS:
      case DXGI_FORMAT_BC3_UNORM:
      case DXGI_FORMAT_BC3_UNORM_SRGB:
      case DXGI_FORMAT_BC5_TYPELESS:
      case DXGI_FORMAT_BC5_UNORM:
      case DXGI_FORMAT_BC5_SNORM:
      case DXGI_FORMAT_BC6H_TYPELESS:
      case DXGI_FORMAT_BC6H_UF16:
      case DXGI_FORMAT_BC6H_SF16:
      case DXGI_FORMAT_BC7_TYPELESS:
      case DXGI_FORMAT_BC7_UNORM:
      case DXGI_FORMAT_BC7_UNORM_SRGB:
         return 8;

      default:
         return 0;
   }
}

HRESULT GetSurfaceInfo(_In_ size_t width,
                       _In_ size_t height,
                       _In_ DXGI_FORMAT fmt,
                       size_t* outNumBytes,
                       _Out_opt_ size_t* outRowBytes,
                       _Out_opt_ size_t* outNumRows)
{
   uint64_t numBytes = 0;
   uint64_t rowBytes = 0;
   uint64_t numRows = 0;

   bool bc = false;
   bool packed = false;
   bool planar = false;
   size_t bpe = 0;
   switch (fmt)
   {
      case DXGI_FORMAT_BC1_TYPELESS:
      case DXGI_FORMAT_BC1_UNORM:
      case DXGI_FORMAT_BC1_UNORM_SRGB:
      case DXGI_FORMAT_BC4_TYPELESS:
      case DXGI_FORMAT_BC4_UNORM:
      case DXGI_FORMAT_BC4_SNORM:
         bc = true;
         bpe = 8;
         break;

      case DXGI_FORMAT_BC2_TYPELESS:
      case DXGI_FORMAT_BC2_UNORM:
      case DXGI_FORMAT_BC2_UNORM_SRGB:
      case DXGI_FORMAT_BC3_TYPELESS:
      case DXGI_FORMAT_BC3_UNORM:
      case DXGI_FORMAT_BC3_UNORM_SRGB:
      case DXGI_FORMAT_BC5_TYPELESS:
      case DXGI_FORMAT_BC5_UNORM:
      case DXGI_FORMAT_BC5_SNORM:
      case DXGI_FORMAT_BC6H_TYPELESS:
      case DXGI_FORMAT_BC6H_UF16:
      case DXGI_FORMAT_BC6H_SF16:
      case DXGI_FORMAT_BC7_TYPELESS:
      case DXGI_FORMAT_BC7_UNORM:
      case DXGI_FORMAT_BC7_UNORM_SRGB:
         bc = true;
         bpe = 16;
         break;

      case DXGI_FORMAT_R8G8_B8G8_UNORM:
      case DXGI_FORMAT_G8R8_G8B8_UNORM:
      case DXGI_FORMAT_YUY2:
         packed = true;
         bpe = 4;
         break;

      case DXGI_FORMAT_Y210:
      case DXGI_FORMAT_Y216:
         packed = true;
         bpe = 8;
         break;

      case DXGI_FORMAT_NV12:
      case DXGI_FORMAT_420_OPAQUE:
      case DXGI_FORMAT_P208:
         planar = true;
         bpe = 2;
         break;

      case DXGI_FORMAT_P010:
      case DXGI_FORMAT_P016:
         planar = true;
         bpe = 4;
         break;

      default:
         break;
   }

   if (bc)
   {
      uint64_t numBlocksWide = 0;
      if (width > 0)
      {
         numBlocksWide = std::max<uint64_t>(1u, (uint64_t(width) + 3u) / 4u);
      }
      uint64_t numBlocksHigh = 0;
      if (height > 0)
      {
         numBlocksHigh = std::max<uint64_t>(1u, (uint64_t(height) + 3u) / 4u);
      }
      rowBytes = numBlocksWide * bpe;
      numRows = numBlocksHigh;
      numBytes = rowBytes * numBlocksHigh;
   }
   else if (packed)
   {
      rowBytes = ((uint64_t(width) + 1u) >> 1) * bpe;
      numRows = uint64_t(height);
      numBytes = rowBytes * height;
   }
   else if (fmt == DXGI_FORMAT_NV11)
   {
      rowBytes = ((uint64_t(width) + 3u) >> 2) * 4u;
      numRows = uint64_t(height) * 2u; // Direct3D makes this simplifying assumption, although it is larger than the 4:1:1 data
      numBytes = rowBytes * numRows;
   }
   else if (planar)
   {
      rowBytes = ((uint64_t(width) + 1u) >> 1) * bpe;
      numBytes = (rowBytes * uint64_t(height)) + ((rowBytes * uint64_t(height) + 1u) >> 1);
      numRows = height + ((uint64_t(height) + 1u) >> 1);
   }
   else
   {
      size_t bpp = BitsPerPixel(fmt);
      if (!bpp)
         return E_INVALIDARG;

      rowBytes = (uint64_t(width) * bpp + 7u) / 8u; // round up to nearest byte
      numRows = uint64_t(height);
      numBytes = rowBytes * height;
   }

   #if defined(_M_IX86) || defined(_M_ARM) || defined(_M_HYBRID_X86_ARM64)
   static_assert(sizeof(size_t) == 4, "Not a 32-bit platform!");
   if (numBytes > UINT32_MAX || rowBytes > UINT32_MAX || numRows > UINT32_MAX)
      return HRESULT_FROM_WIN32(ERROR_ARITHMETIC_OVERFLOW);
   #else
   static_assert(sizeof(size_t) == 8, "Not a 64-bit platform!");
   #endif

   if (outNumBytes)
   {
      *outNumBytes = static_cast<size_t>(numBytes);
   }
   if (outRowBytes)
   {
      *outRowBytes = static_cast<size_t>(rowBytes);
   }
   if (outNumRows)
   {
      *outNumRows = static_cast<size_t>(numRows);
   }

   return S_OK;
}

DXGI_FORMAT GetDXGIFormat(const DDS_PIXELFORMAT& ddpf)
{
   if (ddpf.flags & DDS_RGB)
   {
       // Note that sRGB formats are written using the "DX10" extended header

      switch (ddpf.RGBBitCount)
      {
         case 32:
            if (is_bit_mask(ddpf, 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000))
            {
               return DXGI_FORMAT_R8G8B8A8_UNORM;
            }

            if (is_bit_mask(ddpf, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000))
            {
               return DXGI_FORMAT_B8G8R8A8_UNORM;
            }

            if (is_bit_mask(ddpf, 0x00ff0000, 0x0000ff00, 0x000000ff, 0x00000000))
            {
               return DXGI_FORMAT_B8G8R8X8_UNORM;
            }

            // No DXGI format maps to ISBITMASK(0x000000ff,0x0000ff00,0x00ff0000,0x00000000) aka D3DFMT_X8B8G8R8

            // Note that many common DDS reader/writers (including D3DX) swap the
            // the RED/BLUE masks for 10:10:10:2 formats. We assume
            // below that the 'backwards' header mask is being used since it is most
            // likely written by D3DX. The more robust solution is to use the 'DX10'
            // header extension and specify the DXGI_FORMAT_R10G10B10A2_UNORM format directly

            // For 'correct' writers, this should be 0x000003ff,0x000ffc00,0x3ff00000 for RGB data
            if (is_bit_mask(ddpf, 0x3ff00000, 0x000ffc00, 0x000003ff, 0xc0000000))
            {
               return DXGI_FORMAT_R10G10B10A2_UNORM;
            }

            // No DXGI format maps to ISBITMASK(0x000003ff,0x000ffc00,0x3ff00000,0xc0000000) aka D3DFMT_A2R10G10B10

            if (is_bit_mask(ddpf, 0x0000ffff, 0xffff0000, 0x00000000, 0x00000000))
            {
               return DXGI_FORMAT_R16G16_UNORM;
            }

            if (is_bit_mask(ddpf, 0xffffffff, 0x00000000, 0x00000000, 0x00000000))
            {
                // Only 32-bit color channel format in D3D9 was R32F
               return DXGI_FORMAT_R32_FLOAT; // D3DX writes this out as a FourCC of 114
            }
            break;

         case 24:
             // No 24bpp DXGI formats aka D3DFMT_R8G8B8
            break;

         case 16:
            if (is_bit_mask(ddpf, 0x7c00, 0x03e0, 0x001f, 0x8000))
            {
               return DXGI_FORMAT_B5G5R5A1_UNORM;
            }
            if (is_bit_mask(ddpf, 0xf800, 0x07e0, 0x001f, 0x0000))
            {
               return DXGI_FORMAT_B5G6R5_UNORM;
            }

            // No DXGI format maps to ISBITMASK(0x7c00,0x03e0,0x001f,0x0000) aka D3DFMT_X1R5G5B5

            if (is_bit_mask(ddpf, 0x0f00, 0x00f0, 0x000f, 0xf000))
            {
               return DXGI_FORMAT_B4G4R4A4_UNORM;
            }

            // No DXGI format maps to ISBITMASK(0x0f00,0x00f0,0x000f,0x0000) aka D3DFMT_X4R4G4B4

            // No 3:3:2, 3:3:2:8, or paletted DXGI formats aka D3DFMT_A8R3G3B2, D3DFMT_R3G3B2, D3DFMT_P8, D3DFMT_A8P8, etc.
            break;
      }
   }
   else if (ddpf.flags & DDS_LUMINANCE)
   {
      if (8 == ddpf.RGBBitCount)
      {
         if (is_bit_mask(ddpf, 0x000000ff, 0x00000000, 0x00000000, 0x00000000))
         {
            return DXGI_FORMAT_R8_UNORM; // D3DX10/11 writes this out as DX10 extension
         }

         // No DXGI format maps to ISBITMASK(0x0f,0x00,0x00,0xf0) aka D3DFMT_A4L4

         if (is_bit_mask(ddpf, 0x000000ff, 0x00000000, 0x00000000, 0x0000ff00))
         {
            return DXGI_FORMAT_R8G8_UNORM; // Some DDS writers assume the bitcount should be 8 instead of 16
         }
      }

      if (16 == ddpf.RGBBitCount)
      {
         if (is_bit_mask(ddpf, 0x0000ffff, 0x00000000, 0x00000000, 0x00000000))
         {
            return DXGI_FORMAT_R16_UNORM; // D3DX10/11 writes this out as DX10 extension
         }
         if (is_bit_mask(ddpf, 0x000000ff, 0x00000000, 0x00000000, 0x0000ff00))
         {
            return DXGI_FORMAT_R8G8_UNORM; // D3DX10/11 writes this out as DX10 extension
         }
      }
   }
   else if (ddpf.flags & DDS_ALPHA)
   {
      if (8 == ddpf.RGBBitCount)
      {
         return DXGI_FORMAT_A8_UNORM;
      }
   }
   else if (ddpf.flags & DDS_BUMPDUDV)
   {
      if (16 == ddpf.RGBBitCount)
      {
         if (is_bit_mask(ddpf, 0x00ff, 0xff00, 0x0000, 0x0000))
         {
            return DXGI_FORMAT_R8G8_SNORM; // D3DX10/11 writes this out as DX10 extension
         }
      }

      if (32 == ddpf.RGBBitCount)
      {
         if (is_bit_mask(ddpf, 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000))
         {
            return DXGI_FORMAT_R8G8B8A8_SNORM; // D3DX10/11 writes this out as DX10 extension
         }
         if (is_bit_mask(ddpf, 0x0000ffff, 0xffff0000, 0x00000000, 0x00000000))
         {
            return DXGI_FORMAT_R16G16_SNORM; // D3DX10/11 writes this out as DX10 extension
         }

         // No DXGI format maps to ISBITMASK(0x3ff00000, 0x000ffc00, 0x000003ff, 0xc0000000) aka D3DFMT_A2W10V10U10
      }
   }
   else if (ddpf.flags & DDS_FOURCC)
   {
      if (make_4_cc('D', 'X', 'T', '1') == ddpf.fourCC)
      {
         return DXGI_FORMAT_BC1_UNORM;
      }
      if (make_4_cc('D', 'X', 'T', '3') == ddpf.fourCC)
      {
         return DXGI_FORMAT_BC2_UNORM;
      }
      if (make_4_cc('D', 'X', 'T', '5') == ddpf.fourCC)
      {
         return DXGI_FORMAT_BC3_UNORM;
      }

      // While pre-multiplied alpha isn't directly supported by the DXGI formats,
      // they are basically the same as these BC formats so they can be mapped
      if (make_4_cc('D', 'X', 'T', '2') == ddpf.fourCC)
      {
         return DXGI_FORMAT_BC2_UNORM;
      }
      if (make_4_cc('D', 'X', 'T', '4') == ddpf.fourCC)
      {
         return DXGI_FORMAT_BC3_UNORM;
      }

      if (make_4_cc('A', 'T', 'I', '1') == ddpf.fourCC)
      {
         return DXGI_FORMAT_BC4_UNORM;
      }
      if (make_4_cc('B', 'C', '4', 'U') == ddpf.fourCC)
      {
         return DXGI_FORMAT_BC4_UNORM;
      }
      if (make_4_cc('B', 'C', '4', 'S') == ddpf.fourCC)
      {
         return DXGI_FORMAT_BC4_SNORM;
      }

      if (make_4_cc('A', 'T', 'I', '2') == ddpf.fourCC)
      {
         return DXGI_FORMAT_BC5_UNORM;
      }
      if (make_4_cc('B', 'C', '5', 'U') == ddpf.fourCC)
      {
         return DXGI_FORMAT_BC5_UNORM;
      }
      if (make_4_cc('B', 'C', '5', 'S') == ddpf.fourCC)
      {
         return DXGI_FORMAT_BC5_SNORM;
      }

      // BC6H and BC7 are written using the "DX10" extended header

      if (make_4_cc('R', 'G', 'B', 'G') == ddpf.fourCC)
      {
         return DXGI_FORMAT_R8G8_B8G8_UNORM;
      }
      if (make_4_cc('G', 'R', 'G', 'B') == ddpf.fourCC)
      {
         return DXGI_FORMAT_G8R8_G8B8_UNORM;
      }

      if (make_4_cc('Y', 'U', 'Y', '2') == ddpf.fourCC)
      {
         return DXGI_FORMAT_YUY2;
      }

      // Check for D3DFORMAT enums being set here
      switch (ddpf.fourCC)
      {
         case 36: // D3DFMT_A16B16G16R16
            return DXGI_FORMAT_R16G16B16A16_UNORM;

         case 110: // D3DFMT_Q16W16V16U16
            return DXGI_FORMAT_R16G16B16A16_SNORM;

         case 111: // D3DFMT_R16F
            return DXGI_FORMAT_R16_FLOAT;

         case 112: // D3DFMT_G16R16F
            return DXGI_FORMAT_R16G16_FLOAT;

         case 113: // D3DFMT_A16B16G16R16F
            return DXGI_FORMAT_R16G16B16A16_FLOAT;

         case 114: // D3DFMT_R32F
            return DXGI_FORMAT_R32_FLOAT;

         case 115: // D3DFMT_G32R32F
            return DXGI_FORMAT_R32G32_FLOAT;

         case 116: // D3DFMT_A32B32G32R32F
            return DXGI_FORMAT_R32G32B32A32_FLOAT;
      }
   }

   return DXGI_FORMAT_UNKNOWN;
}

DXGI_FORMAT MakeSRGB(_In_ DXGI_FORMAT format)
{
   switch (format)
   {
      case DXGI_FORMAT_R8G8B8A8_UNORM:
         return DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;

      case DXGI_FORMAT_BC1_UNORM:
         return DXGI_FORMAT_BC1_UNORM_SRGB;

      case DXGI_FORMAT_BC2_UNORM:
         return DXGI_FORMAT_BC2_UNORM_SRGB;

      case DXGI_FORMAT_BC3_UNORM:
         return DXGI_FORMAT_BC3_UNORM_SRGB;

      case DXGI_FORMAT_B8G8R8A8_UNORM:
         return DXGI_FORMAT_B8G8R8A8_UNORM_SRGB;

      case DXGI_FORMAT_B8G8R8X8_UNORM:
         return DXGI_FORMAT_B8G8R8X8_UNORM_SRGB;

      case DXGI_FORMAT_BC7_UNORM:
         return DXGI_FORMAT_BC7_UNORM_SRGB;

      default:
         return format;
   }
}

inline bool IsDepthStencil(DXGI_FORMAT fmt)
{
   switch (fmt)
   {
      case DXGI_FORMAT_R32G8X24_TYPELESS:
      case DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
      case DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS:
      case DXGI_FORMAT_X32_TYPELESS_G8X24_UINT:
      case DXGI_FORMAT_D32_FLOAT:
      case DXGI_FORMAT_R24G8_TYPELESS:
      case DXGI_FORMAT_D24_UNORM_S8_UINT:
      case DXGI_FORMAT_R24_UNORM_X8_TYPELESS:
      case DXGI_FORMAT_X24_TYPELESS_G8_UINT:
      case DXGI_FORMAT_D16_UNORM:
         return true;

      default:
         return false;
   }
}

inline void AdjustPlaneResource(_In_ DXGI_FORMAT fmt,
                                _In_ size_t height,
                                _In_ size_t slicePlane,
                                _Inout_ D3D12_SUBRESOURCE_DATA& res)
{
   switch (fmt)
   {
      case DXGI_FORMAT_NV12:
      case DXGI_FORMAT_P010:
      case DXGI_FORMAT_P016:
         if (!slicePlane)
         {
             // Plane 0
            res.SlicePitch = res.RowPitch * height;
         }
         else
         {
             // Plane 1
            res.pData = reinterpret_cast<const uint8_t*>(res.pData) + res.RowPitch * height;
            res.SlicePitch = res.RowPitch * ((height + 1) >> 1);
         }
         break;

      case DXGI_FORMAT_NV11:
         if (!slicePlane)
         {
             // Plane 0
            res.SlicePitch = res.RowPitch * height;
         }
         else
         {
             // Plane 1
            res.pData = reinterpret_cast<const uint8_t*>(res.pData) + res.RowPitch * height;
            res.RowPitch = (res.RowPitch >> 1);
            res.SlicePitch = res.RowPitch * height;
         }
         break;
   }
}

HRESULT FillInitData(_In_ UINT width,
                     _In_ UINT height,
                     _In_ UINT depth,
                     _In_ UINT16 mipCount,
                     _In_ size_t arraySize,
                     _In_ size_t numberOfPlanes,
                     _In_ DXGI_FORMAT format,
                     _In_ size_t maxsize,
                     _In_ size_t bitSize,
                     _In_reads_bytes_(bitSize) const uint8_t* bitData,
                     _Out_ UINT& twidth,
                     _Out_ UINT& theight,
                     _Out_ UINT& tdepth,
                     _Out_ UINT16& skipMip,
                     std::vector<D3D12_SUBRESOURCE_DATA>& initData)
{
   if (!bitData)
   {
      return E_POINTER;
   }

   skipMip = 0;
   twidth = 0;
   theight = 0;
   tdepth = 0;

   size_t NumBytes = 0;
   size_t RowBytes = 0;
   const uint8_t* pEndBits = bitData + bitSize;

   initData.clear();

   for (size_t p = 0; p < numberOfPlanes; ++p)
   {
      const uint8_t* pSrcBits = bitData;

      for (size_t j = 0; j < arraySize; j++)
      {
         UINT w = width;
         UINT h = height;
         UINT d = depth;
         for (UINT16 i = 0; i < mipCount; i++)
         {
            HRESULT hr = GetSurfaceInfo(w, h, format, &NumBytes, &RowBytes, nullptr);
            if (FAILED(hr))
               return hr;

            if (NumBytes > UINT32_MAX || RowBytes > UINT32_MAX)
               return HRESULT_FROM_WIN32(ERROR_ARITHMETIC_OVERFLOW);

            if ((mipCount <= 1) || !maxsize || (w <= maxsize && h <= maxsize && d <= maxsize))
            {
               if (!twidth)
               {
                  twidth = w;
                  theight = h;
                  tdepth = d;
               }

               D3D12_SUBRESOURCE_DATA res =
               {
                   reinterpret_cast<const void*>(pSrcBits),
                   static_cast<LONG_PTR>(RowBytes),
                   static_cast<LONG_PTR>(NumBytes)
               };

               AdjustPlaneResource(format, h, p, res);

               initData.emplace_back(res);
            }
            else if (!j)
            {
                // Count number of skipped mipmaps (first item only)
               ++skipMip;
            }

            if (pSrcBits + (NumBytes*d) > pEndBits)
            {
               return HRESULT_FROM_WIN32(ERROR_HANDLE_EOF);
            }

            pSrcBits += NumBytes * d;

            w = w >> 1;
            h = h >> 1;
            d = d >> 1;
            if (w == 0)
            {
               w = 1;
            }
            if (h == 0)
            {
               h = 1;
            }
            if (d == 0)
            {
               d = 1;
            }
         }
      }
   }

   return initData.empty() ? E_FAIL : S_OK;
}

DDS_ALPHA_MODE GetAlphaMode(_In_ const DDS_HEADER* header)
{
   if (header->ddspf.flags & DDS_FOURCC)
   {
      if (make_4_cc('D', 'X', '1', '0') == header->ddspf.fourCC)
      {
         auto d3d10ext = reinterpret_cast<const DDS_HEADER_DXT10*>((const char*)header + sizeof(DDS_HEADER));
         auto mode = static_cast<DDS_ALPHA_MODE>(d3d10ext->miscFlags2 & DDS_MISC_FLAGS2_ALPHA_MODE_MASK);
         switch (mode)
         {
            case DDS_ALPHA_MODE_STRAIGHT:
            case DDS_ALPHA_MODE_PREMULTIPLIED:
            case DDS_ALPHA_MODE_OPAQUE:
            case DDS_ALPHA_MODE_CUSTOM:
               return mode;
         }
      }
      else if ((make_4_cc('D', 'X', 'T', '2') == header->ddspf.fourCC)
               || (make_4_cc('D', 'X', 'T', '4') == header->ddspf.fourCC))
      {
         return DDS_ALPHA_MODE_PREMULTIPLIED;
      }
   }

   return DDS_ALPHA_MODE_UNKNOWN;
}

HRESULT CreateTextureFromDDS(_In_ ID3D12Device* d3dDevice,
                             _In_ const DDS_HEADER* header,
                             _In_reads_bytes_(bitSize) const uint8_t* bitData,
                             size_t bitSize,
                             size_t maxsize,
                             D3D12_RESOURCE_FLAGS resFlags,
                             unsigned int loadFlags,
                             TEXTURE_BUFFER* toPopulate_p)
{
   HRESULT hr = S_OK;

   toPopulate_p->width() = header->width;
   toPopulate_p->height() = header->height;
   toPopulate_p->depth() = header->depth;
   toPopulate_p->format() = DXGI_FORMAT_UNKNOWN;
   toPopulate_p->cube() = false;
   toPopulate_p->dimmension() = D3D12_RESOURCE_DIMENSION_UNKNOWN;
   toPopulate_p->array_size() = 1;

   toPopulate_p->mip_length() = static_cast<UINT16>(header->mipMapCount);
   if (0 == toPopulate_p->mip_length())
   {
      toPopulate_p->mip_length() = 1;
   }

   if ((header->ddspf.flags & DDS_FOURCC) &&
      (make_4_cc('D', 'X', '1', '0') == header->ddspf.fourCC))
   {
      auto d3d10ext = reinterpret_cast<const DDS_HEADER_DXT10*>((const char*)header + sizeof(DDS_HEADER));

      toPopulate_p->array_size() = d3d10ext->arraySize;
      if (toPopulate_p->array_size() == 0)
      {
         return HRESULT_FROM_WIN32(ERROR_INVALID_DATA);
      }

      switch (d3d10ext->dxgiFormat)
      {
         case DXGI_FORMAT_AI44:
         case DXGI_FORMAT_IA44:
         case DXGI_FORMAT_P8:
         case DXGI_FORMAT_A8P8:
            return HRESULT_FROM_WIN32(ERROR_NOT_SUPPORTED);

         default:
            if (BitsPerPixel(d3d10ext->dxgiFormat) == 0)
            {
               return HRESULT_FROM_WIN32(ERROR_NOT_SUPPORTED);
            }
      }

      toPopulate_p->format() = d3d10ext->dxgiFormat;

      switch (d3d10ext->resourceDimension)
      {
         case D3D12_RESOURCE_DIMENSION_TEXTURE1D:
             // D3DX writes 1D textures with a fixed Height of 1
            if ((header->flags & DDS_HEIGHT) && toPopulate_p->height() != 1)
            {
               return HRESULT_FROM_WIN32(ERROR_INVALID_DATA);
            }
            toPopulate_p->height() = toPopulate_p->depth() = 1;
            break;

         case D3D12_RESOURCE_DIMENSION_TEXTURE2D:
            if (d3d10ext->miscFlag & 0x4 /* RESOURCE_MISC_TEXTURECUBE */)
            {
               toPopulate_p->array_size() *= 6;
               toPopulate_p->cube() = true;
            }
            toPopulate_p->depth() = 1;
            break;

         case D3D12_RESOURCE_DIMENSION_TEXTURE3D:
            if (!(header->flags & DDS_HEADER_FLAGS_VOLUME))
            {
               return HRESULT_FROM_WIN32(ERROR_INVALID_DATA);
            }

            if (toPopulate_p->array_size() > 1)
            {
               return HRESULT_FROM_WIN32(ERROR_NOT_SUPPORTED);
            }
            break;

         default:
            return HRESULT_FROM_WIN32(ERROR_NOT_SUPPORTED);
      }

      toPopulate_p->dimmension() = static_cast<D3D12_RESOURCE_DIMENSION>(d3d10ext->resourceDimension);
   }
   else
   {
      toPopulate_p->format() = GetDXGIFormat(header->ddspf);

      if (toPopulate_p->format() == DXGI_FORMAT_UNKNOWN)
      {
         return HRESULT_FROM_WIN32(ERROR_NOT_SUPPORTED);
      }

      if (header->flags & DDS_HEADER_FLAGS_VOLUME)
      {
         toPopulate_p->dimmension() = D3D12_RESOURCE_DIMENSION_TEXTURE3D;
      }
      else
      {
         if (header->caps2 & DDS_CUBEMAP)
         {
             // We require all six faces to be defined
            if ((header->caps2 & DDS_CUBEMAP_ALLFACES) != DDS_CUBEMAP_ALLFACES)
            {
               return HRESULT_FROM_WIN32(ERROR_NOT_SUPPORTED);
            }

            toPopulate_p->array_size() = 6;
            toPopulate_p->cube() = true;
         }

         toPopulate_p->depth() = 1;
         toPopulate_p->dimmension() = D3D12_RESOURCE_DIMENSION_TEXTURE2D;

         // Note there's no way for a legacy Direct3D 9 DDS to express a '1D' texture
      }

      assert(BitsPerPixel(toPopulate_p->format()) != 0);
   }

   // Bound sizes (for security purposes we don't trust DDS file metadata larger than the Direct3D hardware requirements)
   if (toPopulate_p->mip_length() > D3D12_REQ_MIP_LEVELS)
   {
      return HRESULT_FROM_WIN32(ERROR_NOT_SUPPORTED);
   }

   switch (toPopulate_p->dimmension())
   {
      case D3D12_RESOURCE_DIMENSION_TEXTURE1D:
         if ((toPopulate_p->array_size() > D3D12_REQ_TEXTURE1D_ARRAY_AXIS_DIMENSION) ||
            (toPopulate_p->width() > D3D12_REQ_TEXTURE1D_U_DIMENSION))
         {
            return HRESULT_FROM_WIN32(ERROR_NOT_SUPPORTED);
         }
         break;

      case D3D12_RESOURCE_DIMENSION_TEXTURE2D:
         if (toPopulate_p->cube())
         {
             // This is the right bound because we set arraySize to (NumCubes*6) above
            if ((toPopulate_p->array_size() > D3D12_REQ_TEXTURE2D_ARRAY_AXIS_DIMENSION) ||
               (toPopulate_p->width() > D3D12_REQ_TEXTURECUBE_DIMENSION) ||
                (toPopulate_p->height() > D3D12_REQ_TEXTURECUBE_DIMENSION))
            {
               return HRESULT_FROM_WIN32(ERROR_NOT_SUPPORTED);
            }
         }
         else if ((toPopulate_p->array_size() > D3D12_REQ_TEXTURE2D_ARRAY_AXIS_DIMENSION) ||
            (toPopulate_p->width() > D3D12_REQ_TEXTURE2D_U_OR_V_DIMENSION) ||
                  (toPopulate_p->height() > D3D12_REQ_TEXTURE2D_U_OR_V_DIMENSION))
         {
            return HRESULT_FROM_WIN32(ERROR_NOT_SUPPORTED);
         }
         break;

      case D3D12_RESOURCE_DIMENSION_TEXTURE3D:
         if ((toPopulate_p->array_size() > 1) ||
            (toPopulate_p->width() > D3D12_REQ_TEXTURE3D_U_V_OR_W_DIMENSION) ||
             (toPopulate_p->height() > D3D12_REQ_TEXTURE3D_U_V_OR_W_DIMENSION) ||
             (toPopulate_p->depth() > D3D12_REQ_TEXTURE3D_U_V_OR_W_DIMENSION))
         {
            return HRESULT_FROM_WIN32(ERROR_NOT_SUPPORTED);
         }
         break;

      default:
         return HRESULT_FROM_WIN32(ERROR_NOT_SUPPORTED);
   }

   UINT numberOfPlanes = D3D12GetFormatPlaneCount(d3dDevice, toPopulate_p->format());
   if (!numberOfPlanes)
      return E_INVALIDARG;

   if ((numberOfPlanes > 1) && IsDepthStencil(toPopulate_p->format()))
   {
       // DirectX 12 uses planes for stencil, DirectX 11 does not
      return HRESULT_FROM_WIN32(ERROR_NOT_SUPPORTED);
   }

   // Create the texture
   size_t numberOfResources = (toPopulate_p->dimmension() == D3D12_RESOURCE_DIMENSION_TEXTURE3D)
      ? 1 : toPopulate_p->array_size();
   numberOfResources *= toPopulate_p->mip_length();
   numberOfResources *= numberOfPlanes;

   if (numberOfResources > D3D12_REQ_SUBRESOURCES)
      return E_INVALIDARG;

   toPopulate_p->subresources().resize(numberOfResources);


   UINT16 skipMip = 0;
   UINT twidth = 0;
   UINT theight = 0;
   UINT tdepth = 0;
   hr = FillInitData(toPopulate_p->width(), toPopulate_p->height(), toPopulate_p->depth(),
                     toPopulate_p->mip_length(), toPopulate_p->array_size(),
                     numberOfPlanes, toPopulate_p->format(),
                     maxsize, bitSize, bitData,
                     twidth, theight, tdepth, skipMip, toPopulate_p->subresources());

   if (SUCCEEDED(hr))
   {
      size_t reservedMips = toPopulate_p->mip_length();
      if (loadFlags & DDS_LOADER_MIP_RESERVE)
      {
         reservedMips = std::min<size_t>(D3D12_REQ_MIP_LEVELS, CountMips(toPopulate_p->width(),
                                                                         toPopulate_p->height()));
      }

      if (FAILED(hr) && !maxsize && (toPopulate_p->mip_length() > 1))
      {
         toPopulate_p->subresources().clear();

         maxsize = (toPopulate_p->dimmension() == D3D12_RESOURCE_DIMENSION_TEXTURE3D)
            ? D3D12_REQ_TEXTURE3D_U_V_OR_W_DIMENSION
            : D3D12_REQ_TEXTURE2D_U_OR_V_DIMENSION;

         hr = FillInitData(toPopulate_p->width(), toPopulate_p->height(), toPopulate_p->depth(),
                           toPopulate_p->mip_length(), toPopulate_p->array_size(),
                           numberOfPlanes, toPopulate_p->format(),
                           maxsize, bitSize, bitData,
                           twidth, theight, tdepth, skipMip, toPopulate_p->subresources());
      }
   }

   if (FAILED(hr))
   {
      toPopulate_p->subresources().clear();
   }

   toPopulate_p->height() = theight;
   toPopulate_p->width() = twidth;
   toPopulate_p->depth() = tdepth;
   toPopulate_p->mip_length() -= skipMip;

   return hr;
}

D3D12_SRV_DIMENSION qgl::graphics::low::srv_dim_from_tbuffer(const TEXTURE_BUFFER & buff)

{
   switch (buff.dimmension())
   {
      case D3D12_RESOURCE_DIMENSION::D3D12_RESOURCE_DIMENSION_TEXTURE1D:
      {
         if (buff.array_size() > 1)
         {
            return D3D12_SRV_DIMENSION_TEXTURE1DARRAY;
         }

         return D3D12_SRV_DIMENSION_TEXTURE1D;
      }
      case D3D12_RESOURCE_DIMENSION::D3D12_RESOURCE_DIMENSION_TEXTURE2D:
      {
         if (buff.cube())
         {
            if (buff.array_size() > 6)
            {
               if ((buff.array_size() % 6) != 0)
               {
                  throw;
               }
               return D3D12_SRV_DIMENSION_TEXTURECUBEARRAY;
            }
            else if (buff.array_size() > 1)
            {
               return D3D12_SRV_DIMENSION_TEXTURE2DARRAY;
            }
            else
            {
               return D3D12_SRV_DIMENSION_TEXTURE2D;
            }
         }
      }
      case D3D12_RESOURCE_DIMENSION::D3D12_RESOURCE_DIMENSION_TEXTURE3D:
      {
         return D3D12_SRV_DIMENSION_TEXTURE3D;
      }
      case D3D12_RESOURCE_DIMENSION::D3D12_RESOURCE_DIMENSION_BUFFER:
      {
         return D3D12_SRV_DIMENSION::D3D12_SRV_DIMENSION_BUFFER;
      }
      default:
      {
         return D3D12_SRV_DIMENSION::D3D12_SRV_DIMENSION_UNKNOWN;
      }
   }
}

void qgl::graphics::low::load_texture_data(const winrt::com_ptr<d3d_device>& dev_p,
                                           TEXTURE_BUFFER* const data_p)
{
   auto first4Bytes_p = reinterpret_cast<const uint32_t*>(data_p->data());
   if (*first4Bytes_p != DDS_MAGIC)
   {
      throw std::domain_error("The magic number is incorrect.");
   }

   auto header_p = reinterpret_cast<const DDS_HEADER*>(data_p->data() + sizeof(uint32_t));

   // Verify header to validate DDS file
   if (header_p->size != sizeof(DDS_HEADER) ||
       header_p->ddspf.size != sizeof(DDS_PIXELFORMAT))
   {
      throw std::domain_error("The header size is not correct.");
   }

    // Check for DX10 extension
   bool bDXT10Header = false;
   if ((header_p->ddspf.flags & DDS_FOURCC) &&
      (make_4_cc('D', 'X', '1', '0') == header_p->ddspf.fourCC))
   {
      bDXT10Header = true;
   }

   size_t ptrOffset = sizeof(uint32_t) + sizeof(DDS_HEADER) + (bDXT10Header ? sizeof(DDS_HEADER_DXT10) : 0);
   auto rawData_p = data_p->data() + ptrOffset;
   size_t rawDataSize = data_p->size() - ptrOffset;

   auto hr = CreateTextureFromDDS(dev_p.get(), header_p, rawData_p, rawDataSize,
                                  0, D3D12_RESOURCE_FLAG_NONE, DDS_LOADER_DEFAULT,
                                  data_p);
   winrt::check_hresult(hr);
}
