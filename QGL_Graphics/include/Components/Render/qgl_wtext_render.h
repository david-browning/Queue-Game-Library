#pragma once
#include "include/qgl_graphics_include.h"
#include "include/Components/qgl_render_context.h"
#include "include/Components/Content/qgl_wtext.h"

namespace qgl::components
{
   static constexpr guid WTEXTRENDER_GUID{ "3491363A8BC44C05B851035BC0099624" };

   qgl::result_t render_wtext(wtext& txt, render_context& cxt)
   {
      return S_OK;
   }

   class wtext_render_component : public render_comonent<wtext>
   {
      public:
      wtext_render_component(
         decltype(render_wtext) renderFunctor) :
         component(WTEXTRENDER_GUID, renderFunctor)
      {

      }

      wtext_render_component()  :
         component(WTEXTRENDER_GUID, render_wtext)
      {

      }

      wtext_render_component(const wtext_render_component&) = default;

      wtext_render_component(wtext_render_component&&) noexcept = default;

      virtual ~wtext_render_component() noexcept = default;
   };
}