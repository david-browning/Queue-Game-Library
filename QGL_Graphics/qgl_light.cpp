#include "pch.h"
#include "include/qgl_light.h"

qgl::graphics::light::light(const LIGHT_BUFFER& lBuffer,
                            winrt::com_ptr<d3d_device>& dev_p,
                            const content::wcontent_item::str_t& name,
                            const content::wcontent_item::id_t& id) :
   m_lightBuffer(dev_p),
   content_item(name, id,
                qgl::content::RESOURCE_TYPES::RESOURCE_TYPE_LIGHT,
                qgl::content::CONTENT_LOADER_IDS::CONTENT_LOADER_ID_LIGHT)
{
   intensity(lBuffer.intensity());

   color(DirectX::XMVectorSet(lBuffer.color()[0], lBuffer.color()[1],
                              lBuffer.color()[2], lBuffer.color()[3]));

   look_at(DirectX::XMVectorSet(lBuffer.look_at()[0], lBuffer.look_at()[1],
                                lBuffer.look_at()[2], lBuffer.look_at()[3]));

   position(DirectX::XMVectorSet(lBuffer.position()[0], lBuffer.position()[1],
                                 lBuffer.position()[2], lBuffer.position()[3]));
}

qgl::graphics::light::~light()
{
   
}
