#pragma once
#include "include/qgl_graphics_include.h"
#include "include/Shaders/qgl_shader.h"

namespace qgl::stagers
{
   /*
    Used to pass shader pointers to an object.
    */
   using shader_stager = typename std::list<
      const qgl::graphics::shaders::shader*>;

   /*
    Gets the address of a shader.
    */
   inline const qgl::graphics::shaders::shader* shader_address(
      const qgl::graphics::shaders::shader* p) noexcept
   {
      return p;
   }

   /*
    Gets the address of a shader.
    */
   inline const qgl::graphics::shaders::shader* shader_address(
      const qgl::graphics::shaders::shader& s) noexcept
   {
      return std::addressof(s);
   }
   
   /*
    Gets the address of a shader.
    */
   inline const qgl::graphics::shaders::shader* shader_address(
      const std::reference_wrapper<qgl::graphics::shaders::shader>& s) noexcept
   {
      return std::addressof(s.get());
   }

   /*
    Gets the address of a shader.
    */
   inline const qgl::graphics::shaders::shader* shader_address(
      const std::shared_ptr<qgl::graphics::shaders::shader>& p) noexcept
   {
      return p.get();
   }

   /*
    Gets the address of a shader.
    */
   inline const qgl::graphics::shaders::shader* shader_address(
      const std::unique_ptr<qgl::graphics::shaders::shader>& p) noexcept
   {
      return p.get();
   }

   /*
    Creates a collection of shaders that will be passed to another object.
    The shader stager returned by this contains only references to the shaders.
    Freeing the shaders that were passed to this will cause this to point to
    invalid memory.
    ShaderForwardIt must point to some kind of shader pointer or reference.
    Do not pass the entire frame object.
    */
   template<class ShaderForwardIt>
   inline shader_stager make_shader_stager(
      ShaderForwardIt first, ShaderForwardIt last)
   {
      shader_stager ret;
      for (; first != last; first++)
      {
         ret.push_back(shader_address(*first));
      }

      return ret;
   }

   /*
    Creates a collection of shaders that will be passed to another object.
    The shader stager returned by this contains only references to the shaders.
    Freeing the shaders that were passed to this will cause this to point to
    invalid memory.
    ShaderT must be some kind of shader pointer or reference. Do not pass the
    entire shader object.
    */
   template<class ShaderT>
   inline shader_stager make_shader_stager(
      std::initializer_list<ShaderT> shaders)
   {
      return make_shader_stager(shaders.begin(), shaders.end());
   }
}