#pragma once
#include "pch.h"
#include "include/Shaders/qgl_shader_helpers.h"

namespace qgl::graphics::shaders
{
   static const std::unordered_map<shader_types, std::string> SHADER_NAME_MAP{
      {shader_types::ds, "ds"},
      {shader_types::gs, "gs"},
      {shader_types::hs, "hs"},
      {shader_types::ps, "ps"},
      {shader_types::vs, "vs"},
      {shader_types::cs, "cs"},
      {shader_types::lib, "lib"},
   };

   static std::string make_target(const shader_compile_params* params_p)
   {
      std::stringstream ret;

      // Need to do special processing for int8 types because otherwise, the
      // compiler treats them as chars.
      // https://stackoverflow.com/questions/15047028/how-to-get-stringstream-to-treat-uint8-t-as-a-number-not-a-character
      ret << SHADER_NAME_MAP.at(params_p->type) << '_' <<
         std::to_string(params_p->shader_model) << '_' <<
         std::to_string(params_p->effect_type);
      return ret.str();
   }

   result_t QGL_CC compile_shader(
      void* source_p,
      size_t sourceSize,
      const shader_compile_params* params_p,
      ishader_blob** out_p,
      ishader_blob** errors_p) noexcept
   {
      auto target = make_target(params_p);
      auto ret = D3DCompile(source_p, sourceSize, params_p->name.data(),
                            nullptr, nullptr,
                            params_p->entry.data(), target.c_str(),
                            params_p->flags.range_shift<0, 31>(),
                            params_p->flags.range_shift<32, 63>(),
                            out_p,
                            errors_p);

      return ret;
   }

   result_t QGL_CC compile_shader_file(
      const qgl::sys_char* filePath,
      const shader_compile_params* params_p,
      ishader_blob** out_p,
      ishader_blob** errors_p) noexcept
   {
      auto target = make_target(params_p);
      auto ret = D3DCompileFromFile(filePath,
                                    nullptr, nullptr,
                                    params_p->entry.data(), target.c_str(),
                                    params_p->flags.range_shift<0, 31>(),
                                    params_p->flags.range_shift<32, 63>(),
                                    out_p,
                                    errors_p);
      return ret;
   }

   result_t QGL_CC qgl::graphics::shaders::compile_shader_library(
      void* source_p,
      size_t sourceSize,
      const shader_compile_params* params_p,
      ishader_blob** out_p,
      ishader_blob** errors_p) noexcept
   {
      auto target = make_target(params_p);
      auto ret = D3DCompile(source_p, sourceSize, "ShaderModule",
                            nullptr, nullptr,
                            nullptr,
                            target.c_str(),
                            params_p->flags.range_shift<0, 31>(),
                            params_p->flags.range_shift<32, 63>(),
                            out_p,
                            errors_p);

      return ret;
   }

   result_t QGL_CC compile_shader_library_file(
      const qgl::sys_char* filePath,
      const shader_compile_params* params_p,
      ishader_blob** out_p,
      ishader_blob** errors_p) noexcept
   {
      auto target = make_target(params_p);

      // The below function complains if the shader entry point is null.
      // This is OK for shader libraries so disable the warning here.
#pragma warning(push)
#pragma warning(disable : 6387)
      auto ret = D3DCompileFromFile(filePath,
                                    nullptr, nullptr,
                                    nullptr,
                                    target.c_str(),
                                    params_p->flags.range_shift<0, 31>(),
                                    params_p->flags.range_shift<32, 63>(),
                                    out_p,
                                    errors_p);
#pragma warning(pop)
      return ret;
   }
}