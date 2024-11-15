#pragma once
#include "pch.h"
#include "include/Components/qgl_icomponent_param_metadata.h"
#include "include/Components/qgl_component_params.h"

namespace qgl::components
{
   /*
    This is a component param metadata that can be constructored from static
    code.
    */
   class dynamic_component_param_metadata : public icomponent_param_metadata
   {
      public:
      dynamic_component_param_metadata(content_param_types t,
                                       uint8_t count,
                                       const std::string& name,
                                       const std::string& desc) :
         m_name(name),
         m_description(desc),
         m_type(t),
         m_size(count)
      {

      }

      dynamic_component_param_metadata(content_param_types t,
                                       uint8_t count,
                                       std::string&& name,
                                       std::string&& desc) :
         m_name(std::forward<std::string>(name)),
         m_description(std::forward<std::string>(desc)),
         m_type(t),
         m_size(count)
      {

      }

      template<class InputIt>
      dynamic_component_param_metadata(const std::string& name,
                                       const std::string& desc,
                                       InputIt first,
                                       InputIt last) :
         m_name(name),
         m_description(desc),
         m_type(impl::known_param_types_impl::known_compound),
         m_params(first, last)
      {
         if (m_params.size() > UINT8_MAX)
         {
            throw std::length_error{ "Too many parameters." };
         }

         m_size = static_cast<uint8_t>(m_params.size());
      }

      template<class InputIt>
      dynamic_component_param_metadata(std::string&& name,
                                       std::string&& desc,
                                       InputIt first,
                                       InputIt last) :
         m_name(std::forward<std::string>(name)),
         m_description(std::forward<std::string>(desc)),
         m_type(impl::known_param_types_impl::known_compound),
         m_params(first, last)
      {
         if (m_params.size() > UINT8_MAX)
         {
            throw std::length_error{ "Too many parameters." };
         }

         m_size = static_cast<uint8_t>(m_params.size());
      }

      dynamic_component_param_metadata(
         const dynamic_component_param_metadata&) = default;

      dynamic_component_param_metadata(
         dynamic_component_param_metadata&&) noexcept = default;

      virtual ~dynamic_component_param_metadata() noexcept
      {
         
      }

      virtual void release() noexcept
      {
         delete this;
      }

      virtual iqgl* duplicate() const noexcept
      {
         return new (std::nothrow)dynamic_component_param_metadata(*this);
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

      virtual content_param_types type() const noexcept
      {
         return m_type;
      }

      virtual uint8_t size() const noexcept
      {
         return m_size;
      }

      virtual result_t parameter(
         uint32_t idx, icomponent_param_metadata** out_p) const noexcept
      {
         if (!compound_param(m_type))
         {
            return DISP_E_TYPEMISMATCH;
         }

         if (idx >= size() || !out_p)
         {
            return E_INVALIDARG;
         }

         *out_p = static_cast<icomponent_param_metadata*>(m_params[idx].duplicate());
         return S_OK;
      }

      private:
      std::string m_name;
      std::string m_description;
      content_param_types m_type;

      uint8_t m_size;
      std::vector<dynamic_component_param_metadata> m_params;
   };
}