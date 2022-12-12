#pragma once
#include "include/qgl_graphics_include.h"
#include "include/GPU/Root-Signature/qgl_ibindable.h"

namespace qgl::stagers
{
   /*
    Used to pass ibindable pointers to an object.
    */
   using ibindable_stager = typename std::list<const graphics::gpu::ibindable*>;

   /*
    Gets the address of an ibindable.
    */
   inline const graphics::gpu::ibindable* ibindable_address(
      const graphics::gpu::ibindable* p) noexcept
   {
      return p;
   }
   
   /*
    Gets the address of an ibindable.
    */
   inline const graphics::gpu::ibindable* ibindable_address(
      const graphics::gpu::ibindable& b) noexcept
   {
      return std::addressof(b);
   }
   
   /*
    Gets the address of an ibindable.
    */
   inline const graphics::gpu::ibindable* ibindable_address(
      const std::reference_wrapper<graphics::gpu::ibindable>& b) noexcept
   {
      return std::addressof(b.get());
   }
   
   /*
    Gets the address of an ibindable.
    */
   inline const graphics::gpu::ibindable* ibindable_address(
      const std::shared_ptr<graphics::gpu::ibindable>& p) noexcept
   {
      return p.get();
   }

   /*
    Gets the address of an ibindable.
    */
   inline const graphics::gpu::ibindable* ibindable_address(
      const std::unique_ptr<graphics::gpu::ibindable>& p) noexcept
   {
      return p.get();
   }

   /*
    Creates a collection of ibindable pointers that will be passed to another
    object. The ibindable stager returned by this contains only references to
    the ibindables. Freeing the ibindables that were passed to this will cause
    this to point to invalid memory.
    IBindableForwardIt must point to some kind of ibindable pointer or reference
    since ibindable is an abstract type.
    */
   template<class IBindableForwardIt>
   inline ibindable_stager make_ibindable_stager(
      IBindableForwardIt first, IBindableForwardIt last)
   {
      ibindable_stager ret;
      for (; first != last; first++)
      {
         ret.push_back(ibindable_address(*first));
      }

      return ret;
   }

   /*
    Creates a collection of ibindable pointers that will be passed to another
    object. The ibindable stager returned by this contains only references to
    the ibindables. Freeing the ibindables that were passed to this will cause
    this to point to invalid memory.
    IBindableT must be some kind of ibindable pointer or reference since 
    ibindable is an abstract type.
    */
   template<class IBindableT>
   inline ibindable_stager make_ibindable_stager(
      std::initializer_list<IBindableT> ibindables)
   {
      return make_ibindable_stager(ibindables.begin(), ibindables.end());
   }
}
