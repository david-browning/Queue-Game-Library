#include "pch.h"
#include "include/qgl_graphic_options.h"
#include "include/qgl_gdev_helpers.h"
#include "include/qgl_d3d_device.h"

qgl::graphics::graphic_options::graphic_options(const graphics_device& dev)
{
   p_populate(dev);
}

qgl::graphics::graphic_options::graphic_options(const graphic_options& r) :
   m_desc(r.m_desc),
   m_modes(r.m_modes),
   m_3d(r.m_3d),
   m_tearing(r.m_tearing),
   m_hdr(r.m_hdr)
{
}

qgl::graphics::graphic_options::graphic_options(graphic_options&& r) :
   m_desc(std::move(r.m_desc)),
   m_modes(std::move(r.m_modes)),
   m_3d(r.m_3d),
   m_tearing(r.m_tearing),
   m_hdr(r.m_hdr)
{
}

bool qgl::graphics::graphic_options::valid(const GRAPHICS_CONFIG_BUFFER& config) const
{
   bool isValid = true;

   //Tearing enabled implies support. (t -> s) = (!t V s)
   auto tearingValid = (!config.tearing() || m_tearing);
   isValid = isValid && tearingValid;
   #ifdef DEBUG
   if (!isValid)
   {
      throw std::invalid_argument("Tearing is enabled but not supported.");
   }
   #endif
      
   isValid = isValid && (!config.stereo() || m_3d);
   #ifdef DEBUG
   if (!isValid)
   {
      throw std::invalid_argument("Stereo is enabled but not supported.");
   }
   #endif
   

   isValid = isValid && (!config.hdr() || m_hdr);
   #ifdef DEBUG
   if (!isValid)
   {
      throw std::invalid_argument("HDR is enabled but not supported.");
   }
   #endif

   //There must be at least two buffers for the swap chain.
   isValid = isValid && (config.buffers() > 1);
   #ifdef DEBUG
   if (!isValid)
   {
      throw std::invalid_argument("There must be at least two buffers..");
   }
   #endif

   //Enumerate all the display modes.
   bool foundMode = false;
   for (auto& mode : m_modes)
   {
      //Check if the resolution and refresh rate are supported.
      float aproxRefresh = static_cast<float>(mode.RefreshRate.Numerator) /
         static_cast<float>(mode.RefreshRate.Denominator);
      if (mode.Height == config.height() &&
          mode.Width == config.width() &&
          ApproxEq<float>(aproxRefresh, static_cast<float>(config.refresh()), 0.00001f))
      {
         foundMode = true;
         break;
      }
   }

   isValid = isValid && foundMode;
   #ifdef DEBUG
   if (!isValid)
   {
      throw std::invalid_argument("There is no matching resolution + refresh rate.");
   }
   #endif

   return isValid;
}

void qgl::graphics::graphic_options::p_populate(const graphics_device& dev)
{
   //Check for tearing support.
   m_tearing = qgl::graphics::support_tearing(dev.com_dx_factory());

   //Check for 3D support
   m_3d = qgl::graphics::support_stereo();

   //Check HDR support.
   winrt::com_ptr<IDXGIOutput> bestOutput = nullptr;
   winrt::check_hresult(dev.com_swap_chain()->GetContainingOutput(bestOutput.put()));
   auto output6 = bestOutput.as<IDXGIOutput6>();
   m_hdr = qgl::graphics::support_hdr(output6, m_desc);

   //Enumerate the display modes.
   m_modes = qgl::graphics::enum_display_modes(output6, 
                                               dev.format(), 
                                               dev.config().interlacing(),
                                               m_3d);

   assert(0 == 0);
}
