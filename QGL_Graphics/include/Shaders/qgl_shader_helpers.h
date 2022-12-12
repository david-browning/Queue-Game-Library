#pragma once
#include "include/qgl_graphics_include.h"
#include "include/Shaders/qgl_shader_compile_params.h"

namespace qgl::graphics::shaders
{
   /*
    Stores shader source code or compiled shader code.
    */
   using shader_data = typename std::vector<std::byte>;

   /*
    Platform specific way of storing loaded shader data.
    */
   using ishader_blob = typename ID3DBlob;

   /*
    Compiles "source_p" which is "sourceSize" bytes long using "params_p"
    and stores the result in "out_p".
    */
   extern "C" QGL_GRAPHICS_API result_t QGL_CC compile_shader(
      void* source_p,
      size_t sourceSize,
      const shader_compile_params* params_p,
      ishader_blob** out_p,
      ishader_blob** errors_p) noexcept;

   /*
    Compiles the shader specified by the file using "params_p" and stores the 
    result in "out_p".
    */
   extern "C" QGL_GRAPHICS_API result_t QGL_CC compile_shader_file(
      const qgl::sys_char* filePath,
      const shader_compile_params* params_p,
      ishader_blob** out_p,
      ishader_blob** errors_p) noexcept;

   /*
    Compiles "source_p" which is "sourceSize" bytes long into a shader library
    and stores the result in "out_p".
    */
   extern "C" QGL_GRAPHICS_API result_t QGL_CC compile_shader_library(
      void* source_p,
      size_t sourceSize,
      const shader_compile_params* params_p,
      ishader_blob** out_p,
      ishader_blob** errors_p) noexcept;

   /*
    Compiles the shader library specified by the file and stores the result in
    "out_p".
    */
   extern "C" QGL_GRAPHICS_API result_t QGL_CC compile_shader_library_file(
      const qgl::sys_char* filePath,
      const shader_compile_params * params_p,
      ishader_blob** out_p,
      ishader_blob** errors_p) noexcept;

   /* 
    Checks the error blob and and prints the error using the reporter.
    Both b_p and e_p can be nullptr.
    */
   template<typename CharT>
   void check_and_print(ishader_blob* b_p, error_reporter<CharT>* e_p) noexcept
   {
#ifdef DEBUG
      if (b_p)
      {
         if (e_p)
         {
            e_p->print((const CharT*)b_p->GetBufferPointer());
         }
         else
         {
            if constexpr (std::is_same<CharT, char>::value)
            {
               OutputDebugStringA((const char*)b_p->GetBufferPointer());
            }
            else if constexpr(std::is_same<CharT, wchar_t>::value)
            {
               OutputDebugStringW((const wchar_t*)b_p->GetBufferPointer());
            }
            else
            {
            }
         }
      }
#else
      if (b_p && e_p)
      {
         e_p->print((const CharT*)b_p->GetBufferPointer());
      }
#endif
      
   }

   /*
    Compiles the source data using "params" and returns a buffer of the 
    compiled code.
    */
   template<typename CharT = char>
   inline shader_data compile_shader(
      const shader_data& source,
      const shader_compile_params& params,
      error_reporter<CharT>* e = nullptr)
   {
      winrt::com_ptr<ishader_blob> blob = nullptr;
      winrt::com_ptr<ishader_blob> error = nullptr;
      auto result = compile_shader((void*)source.data(), 
                                   source.size(),
                                   &params, 
                                   blob.put(),
                                   error.put());
      check_and_print<CharT>(error.get(), e);
      winrt::check_hresult(result);
      shader_data ret;
      ret.resize(blob->GetBufferSize());
      memcpy(ret.data(), blob->GetBufferPointer(), ret.size());
      return ret;
   }

   /*
    Compiles the shader file using "params" and returns a buffer of the
    compiled code.
    */
   template<typename CharT = char>
   inline shader_data compile_shader(
      const qgl::sys_str& file,
      const shader_compile_params& params,
      error_reporter<CharT>* e = nullptr)
   {
      winrt::com_ptr<ishader_blob> blob = nullptr;
      winrt::com_ptr<ishader_blob> error = nullptr;
      auto result = compile_shader_file(file.c_str(),
                                        &params,
                                        blob.put(),
                                        error.put());
      check_and_print<CharT>(error.get(), e);
      winrt::check_hresult(result);
      shader_data ret;
      ret.resize(blob->GetBufferSize());
      memcpy(ret.data(), blob->GetBufferPointer(), ret.size());
      return ret;
   }

   /*
    Compiles the shader library source code and returns a pointer to the 
    compiled code.
    */
   template<typename CharT = char>
   inline shader_data compile_shader_library(
      const shader_data& source,
      const shader_compile_params& params,
      error_reporter<CharT>* e = nullptr)
   {
      winrt::com_ptr<ishader_blob> blob = nullptr;
      winrt::com_ptr<ishader_blob> error = nullptr;
      auto result = compile_shader_library((void*)source.data(),
                                           source.size(),
                                           &params,
                                           blob.put(),
                                           error.put());
      check_and_print<CharT>(error.get(), e);
      winrt::check_hresult(result);
      shader_data ret;
      ret.resize(blob->GetBufferSize());
      memcpy(ret.data(), blob->GetBufferPointer(), ret.size());
      return ret;
   }

   /*
    Compiles the shader library file and return a buffer to the compiled code.
    */
   template<typename CharT = char>
   inline shader_data compile_shader_library(
      const qgl::sys_str& file,
      const shader_compile_params& params,
      error_reporter<CharT>* e = nullptr)
   {
      winrt::com_ptr<ishader_blob> blob = nullptr;
      winrt::com_ptr<ishader_blob> error = nullptr;
      auto result = compile_shader_library_file(file.c_str(),
                                                &params,
                                                blob.put(),
                                                error.put());
      check_and_print<CharT>(error.get(), e);
      winrt::check_hresult(result);
      shader_data ret;
      ret.resize(blob->GetBufferSize());
      memcpy(ret.data(), blob->GetBufferPointer(), ret.size());
      return ret;
   }
}