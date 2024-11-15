#pragma once
#include "include/Components/qgl_icomponent_metadata.h"
#include "src/Components/qgl_dynamic_component_param_metadata.h"

namespace qgl::components
{
   /*
    This is a component metadata that can be constructed from static code.
    */
   class dynamic_component_metadata : public icomponent_metadata
   {
      public:
      /*
       Constructor
       name: Name of the component.
       desc: Component description.
       g: Component GUID.
       first: First input iterator. Dereferencing the iterator must yield a
         "dynamic_component_param_metadata".
       last: Last input iterator. Dereferencing the iterator must yield a
         "dynamic_component_param_metadata".
       */
      template<class InputIt>
      dynamic_component_metadata(const std::string& name,
                                 const std::string& desc,
                                 const qgl::guid& g,
                                 InputIt first,
                                 InputIt last) :
         m_name(name),
         m_description(desc),
         m_guid(g),
         m_params(first, last)
      {

      }

      /*
       Constructor
       name: Name of the component. This parameter is moved so the original
         variable is invalid after calling this constructor.
       desc: Component description. This parameter is moved so the original
         variable is invalid after calling this constructor.
       g: Component GUID.
       first: First input iterator. Dereferencing the iterator must yield a
         "dynamic_component_param_metadata".
       last: Last input iterator. Dereferencing the iterator must yield a
         "dynamic_component_param_metadata".
       */
      template<class InputIt>
      dynamic_component_metadata(std::string&& name,
                                 std::string&& desc,
                                 qgl::guid&& g,
                                 InputIt first,
                                 InputIt last) :
         m_name(std::forward<std::string>(name)),
         m_description(std::forward<std::string>(desc)),
         m_guid(std::forward<qgl::guid>(g)),
         m_params(first, last)
      {

      }

      /*
       Copy constructor.
       */
      dynamic_component_metadata(const dynamic_component_metadata&) = default;

      /*
       Move constructor.
       */
      dynamic_component_metadata(dynamic_component_metadata&&) noexcept = default;

      /*
       Destructor.
       */
      virtual ~dynamic_component_metadata() noexcept
      {

      }

      virtual void release() noexcept
      {
         delete this;
      }

      virtual iqgl* duplicate() const noexcept
      {
         return new (std::nothrow)dynamic_component_metadata(*this);
      }

      virtual uint32_t name(char* strz) const noexcept
      {
         if (strz)
         {
            memcpy(strz, m_name.c_str(), m_name.size());
            strz[m_name.size()] = '\0';
         }

         return static_cast<uint32_t>(m_name.size() + 1);
      }

      virtual uint32_t description(char* strz) const noexcept
      {
         if (strz)
         {
            memcpy(strz, m_description.c_str(), m_description.size());
            strz[m_description.size()] = '\0';
         }

         return static_cast<uint32_t>(m_description.size() + 1);
      }

      virtual void id(qgl::guid* g) const noexcept
      {
         memcpy(g, &m_guid, sizeof(qgl::guid));
      }

      virtual uint32_t size() const noexcept
      {
         return static_cast<uint32_t>(m_params.size());
      }

      virtual result_t param(
         uint32_t idx, icomponent_param_metadata** out_p) const noexcept
      {
         if (idx >= size() || !out_p)
         {
            return E_INVALIDARG;
         }

         *out_p = static_cast<dynamic_component_param_metadata*>(m_params[idx].duplicate());
         return S_OK;
      }

      private:
      std::string m_name;
      std::string m_description;
      qgl::guid m_guid;
      mutable std::vector<dynamic_component_param_metadata> m_params;
   };
}