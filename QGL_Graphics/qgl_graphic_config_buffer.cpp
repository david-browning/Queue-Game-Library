#include "pch.h"
#include "include/qgl_graphic_config_buffer.h"

qgl::graphics::GRAPHICS_CONFIG_BUFFER::GRAPHICS_CONFIG_BUFFER() :
   m_flags(0),
   m_preferedAdapterDevID(0),
   m_width(640),
   m_height(480),
   m_refresh(60),
   m_buffers(3),
   m_fullScreen(false),
   m_tearing(false),
   m_highResolution(true),
   m_3d(false),
   m_hdr(false),
   m_enableInterlacing(true)
{
}

qgl::graphics::GRAPHICS_CONFIG_BUFFER::GRAPHICS_CONFIG_BUFFER(const GRAPHICS_CONFIG_BUFFER& r) :
   m_flags(r.m_flags),
   m_preferedAdapterDevID(r.m_preferedAdapterDevID),
   m_width(r.m_width),
   m_height(r.m_height),
   m_refresh(r.m_refresh),
   m_buffers(r.m_buffers),
   m_fullScreen(r.m_fullScreen),
   m_tearing(r.m_tearing),
   m_highResolution(r.m_highResolution),
   m_3d(r.m_3d),
   m_hdr(r.m_hdr),
   m_enableInterlacing(r.m_enableInterlacing)
{
}

qgl::graphics::GRAPHICS_CONFIG_BUFFER::GRAPHICS_CONFIG_BUFFER(GRAPHICS_CONFIG_BUFFER&& r) :
   m_flags(r.m_flags),
   m_preferedAdapterDevID(r.m_preferedAdapterDevID),
   m_width(r.m_width),
   m_height(r.m_height),
   m_refresh(r.m_refresh),
   m_buffers(r.m_buffers),
   m_fullScreen(r.m_fullScreen),
   m_tearing(r.m_tearing),
   m_highResolution(r.m_highResolution),
   m_3d(r.m_3d),
   m_hdr(r.m_hdr),
   m_enableInterlacing(r.m_enableInterlacing)
{
   #ifdef DEBUG
   r.m_buffers = 0;
   r.m_flags = 0;
   r.m_fullScreen = false;
   r.m_height = 0;
   r.m_highResolution = false;
   r.m_refresh = 0;
   r.m_tearing = false;
   r.m_width = 0;
   r.m_3d = false;
   r.m_hdr = false;
   r.m_enableInterlacing = false;
   r.m_preferedAdapterDevID = 0;
   #endif
}
