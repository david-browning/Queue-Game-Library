#pragma once
#include "include/qgl_graphics_include.h"
#include "include/Shaders/qgl_shader_descriptor.h"

namespace qgl::graphics::shaders
{
   enum class shader_var_types
   {
      unknown = D3D_REGISTER_COMPONENT_TYPE::D3D_REGISTER_COMPONENT_UNKNOWN,
      uint32 = D3D_REGISTER_COMPONENT_TYPE::D3D_REGISTER_COMPONENT_UINT32,
      sint32 = D3D_REGISTER_COMPONENT_TYPE::D3D_REGISTER_COMPONENT_SINT32,
      float32 = D3D_REGISTER_COMPONENT_TYPE::D3D_REGISTER_COMPONENT_FLOAT32,
   };

   enum class shader_buffer_types
   {
      const_buffer = D3D_CBUFFER_TYPE::D3D_CT_CBUFFER,
      texture_buffer = D3D_CBUFFER_TYPE::D3D_CT_TBUFFER,
      ipointers = D3D_CBUFFER_TYPE::D3D_CT_INTERFACE_POINTERS,
      bind_info = D3D_CBUFFER_TYPE::D3D_CT_RESOURCE_BIND_INFO,
   };

   /*
    Computes the size of a shader variable.
    */
   constexpr size_t shader_var_size(shader_var_types type) noexcept
   {
      switch (type)
      {
         case shader_var_types::uint32:
         case shader_var_types::sint32:
         case shader_var_types::float32:
         {
            return sizeof(int32_t);
         }
         default:
         {
            return 0;
         }
      }
   }

   /*
    Calculates the total size of a shader variable where the mask represents
    which components are set.
    */
   constexpr size_t shader_var_size(shader_var_types type,
                                    uint8_t mask) noexcept
   {
      return shader_var_size(type) * mem::bits_set(mask);
   }

   /*
    Gets a DXGI_FORMAT for a float shader variable with the given component
    mask.
    */
   constexpr DXGI_FORMAT vformat_from_float(uint8_t mask) noexcept
   {
      switch (mask)
      {
         case 0b1111:
         {
            return DXGI_FORMAT_R32G32B32A32_FLOAT;
         }
         case 0b0111:
         {
            return DXGI_FORMAT_R32G32B32_FLOAT;
         }
         case 0b0011:
         {
            return DXGI_FORMAT_R32G32_FLOAT;
         }
         case 0b0001:
         {
            return DXGI_FORMAT_R32_FLOAT;
         }
         default:
         {
            return DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
         }
      }
   }

   /*
    Gets a DXGI_FORMAT for a signed int shader variable with the given component
    mask.
    */
   constexpr DXGI_FORMAT vformat_from_sint(uint8_t mask) noexcept
   {
      switch (mask)
      {
         case 0b1111:
         {
            return DXGI_FORMAT_R32G32B32A32_SINT;
         }
         case 0b0111:
         {
            return DXGI_FORMAT_R32G32B32_SINT;
         }
         case 0b0011:
         {
            return DXGI_FORMAT_R32G32_SINT;
         }
         case 0b0001:
         {
            return DXGI_FORMAT_R32_SINT;
         }
         default:
         {
            return DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
         }
      }
   }

   /*
    Gets a DXGI_FORMAT for an unsigned int shader variable with the given 
    component mask.
    */
   constexpr DXGI_FORMAT vformat_from_uint(uint8_t mask) noexcept
   {
      switch (mask)
      {
         case 0b1111:
         {
            return DXGI_FORMAT_R32G32B32A32_UINT;
         }
         case 0b0111:
         {
            return DXGI_FORMAT_R32G32B32_UINT;
         }
         case 0b0011:
         {
            return DXGI_FORMAT_R32G32_UINT;
         }
         case 0b0001:
         {
            return DXGI_FORMAT_R32_UINT;
         }
         default:
         {
            return DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
         }
      }
   }

   /*
    Returns an appropriate DXGI_FORMAT for a vertex element with the given 
    component mask.
    */
   constexpr DXGI_FORMAT vformat(shader_var_types type,
                                 uint8_t mask) noexcept
   {
      switch (type)
      {
         case shader_var_types::float32:
         {
            return vformat_from_float(mask);
         }
         case shader_var_types::sint32:
         {
            return vformat_from_sint(mask);
         }
         case shader_var_types::uint32:
         {
            return vformat_from_uint(mask);
         }
         default:
         {
            return DXGI_FORMAT::DXGI_FORMAT_UNKNOWN;
         }
      }
   }

   /*
    Describes a parameter that is passed to a shader entry point.
    */
   struct shader_in_param final
   {
      shader_in_param(const D3D12_SIGNATURE_PARAMETER_DESC& d) :
         semantic_name(d.SemanticName),
         semantic_idx(d.SemanticIndex),
         register_idx(d.Register),
         component_mask(d.Mask),
         rw_mask(d.ReadWriteMask),
         component_type(static_cast<shader_var_types>(d.ComponentType))
      {
         param_size = shader_var_size(component_type, component_mask);
      }

      shader_in_param(const shader_in_param&) = default;

      shader_in_param(shader_in_param&&) noexcept = default;

      ~shader_in_param() noexcept = default;

      /*
       A per-parameter string that identifies how the data will be used.
       */
      std::string semantic_name;

      /*
       Semantic index that modifies the semantic. Used to differentiate
       different parameters that use the same semantic.
       */
      size_t semantic_idx;

      /*
       The register that will contain this variable's data.
       */
      size_t register_idx;

      /*
       The type of the parameter or the type of each element if this is a 
       multi-component data type.
       */
      shader_var_types component_type;

      /*
       Mask which indicates which components of a register are used.
       If the bit is set, then the component is present.
       */
      uint8_t component_mask;

      /*
       Mask which indicates whether a given component is never written
       (if the signature is an output signature) or always read
       (if the signature is an input signature).
       */
      uint8_t rw_mask;

      /*
       Size (in bytes) of the shader parameter.
       */
      size_t param_size;
   };

   /*
    Describes a parameter that is returned from a shader entry point.
    */
   using shader_out_param = typename shader_in_param;

   /*
    Describes a parameter that is passed to a shader function. This is not the 
    same as the shader entry point.
    */
   struct shader_param_info final
   {
      shader_param_info(const D3D12_PARAMETER_DESC& d) :
         name(d.Name),
         semantic_name(d.SemanticName),
         rows(d.Rows),
         columns(d.Columns)
      {

      }

      shader_param_info(const shader_param_info&) = default;

      shader_param_info(shader_param_info&&) noexcept = default;

      ~shader_param_info() noexcept = default;

      /*
       The name of the function parameter.
       */
      std::string name;

      /*
       The HLSL semantic that is associated with this function parameter.
       This name includes the index, for example, SV_Target[n].
       */
      std::string semantic_name;

      /*
       The number of rows for a matrix parameter.
       */
      size_t rows;

      /*
       The number of columns for a matrix parameter.
       */
      size_t columns;
   };

   using shader_param_stager = typename std::vector<shader_param_info>;

   /*
    Describes a shader variable.
    */
   struct shader_var_info final
   {
      shader_var_info(const D3D12_SHADER_VARIABLE_DESC& d,
                      const D3D12_SHADER_TYPE_DESC& t) :
         name(d.Name),
         size(d.Size),
         offset(d.StartOffset),
         t_offset(d.StartTexture),
         t_size(d.TextureSize),
         s_offset(d.StartSampler),
         s_size(d.SamplerSize),
         type(t.Name),
         rows(t.Rows),
         columns(t.Columns),
         elements(t.Elements),
         members(t.Members)
      {

      }

      shader_var_info(const shader_var_info&) = default;

      shader_var_info(shader_var_info&&) noexcept = default;

      ~shader_var_info() noexcept = default;

      /*
       The variable name.
       */
      std::string name;

      /*
       Name of the shader-variable type. This member can be NULL if it isn't
       used. This member supports dynamic shader linkage interface types,
       which have names.
       */
      std::string type;

      /*
       Size of the variable (in bytes).
       */
      size_t size;

      /*
       Offset from the start of the parent structure to the beginning of the
       variable.
       */
      size_t offset;

      /*
       Offset from the start of the variable to the beginning of the texture.
       */
      size_t t_offset;

      /*
       The size of the texture, in bytes.
       */
      size_t t_size;

      /*
       Offset from the start of the variable to the beginning of the sampler.
       */
      size_t s_offset;

      /*
       The size of the sampler, in bytes.
       */
      size_t s_size;

      /*
       Number of rows in a matrix. Otherwise a numeric type returns 1, any
       other type returns 0.
       */
      size_t rows;

      /*
       Number of columns in a matrix. Otherwise a numeric type returns 1, any
       other type returns 0.
       */
      size_t columns;

      /*
       Number of elements in an array; otherwise 0.
       */
      size_t elements;

      /*
       Number of members in the structure; otherwise 0.
       */
      size_t members;
   };

   using shader_var_stager = typename std::vector<shader_var_info>;

   /*
    Describes a shader function, including it's parameters.
    */
   struct shader_func_info final
   {
      shader_func_info(const D3D12_FUNCTION_DESC& d,
                       const shader_param_stager& p) :
         name(d.Name),
         type(to_shader_type(static_cast<D3D12_SHADER_VERSION_TYPE>(d.Version))),
         is_void(d.HasReturn),
         params(p)
      {

      }

      shader_func_info(const shader_func_info&) = default;

      shader_func_info(shader_func_info&&) noexcept = default;

      ~shader_func_info() noexcept = default;

      /*
       The name of the function.
       */
      std::string name;

      /*
       Shader type.
       */
      shader_types type;

      /*
       Indicates whether the function returns a value. TRUE indicates it
       returns a value; otherwise, FALSE (it is a subroutine).
       */
      bool is_void;

      /*
       The parameters the function uses.
       */
      shader_param_stager params;
   };

   /*
    Describes a shader buffer.
    */
   struct shader_buffer_info final
   {
      shader_buffer_info(const D3D12_SHADER_BUFFER_DESC& d,
                         const shader_var_stager& v) :
         name(d.Name),
         variables(d.Variables),
         size(d.Size),
         type(static_cast<shader_buffer_types>(d.Type)),
         vars(v)
      {

      }

      shader_buffer_info(const shader_buffer_info&) = default;

      shader_buffer_info(shader_buffer_info&&) noexcept = default;

      ~shader_buffer_info() noexcept = default;

      /*
       The name of the buffer.
       */
      std::string name;

      /*
       The number of unique variables.
       */
      size_t variables;

      /*
       The size of the buffer, in bytes.
       */
      size_t size;

      /*
       Type of buffer.
       */
      shader_buffer_types type;

      /*
       Variables in the buffer.
       */
      shader_var_stager vars;
   };
}