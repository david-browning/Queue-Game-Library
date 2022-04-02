#include "pch.h"
#include "include/Interfaces/qgl_icontent_map.h"

namespace qgl
{
   using ctrl_map =
      typename std::unordered_map<content_param_types, content_map_data>;

   const ctrl_map make_default_map()
   {
      ctrl_map ret;
      using cpt = content_param_types;

      // Set the int related:
      for (auto i = (cpt)known_param_types::known_int8;
           i < (cpt)known_param_types::known_int_last;
           i++)
      {
         if (i % 2 != 0)
         {
            ret[i] = { 1, content_control_types::uspinner };
         }
         else
         {
            ret[i] = { 1, content_control_types::spinner };
         }
      }

      // Set the rational related:
      for (auto i = (cpt)known_param_types::known_rational8;
           i < (cpt)known_param_types::known_rational_last;
           i++)
      {
         if (i % 2 != 0)
         {
            ret[i] = { 2, content_control_types::uspinner };
         }
         else
         {
            ret[i] = { 2, content_control_types::spinner };
         }
      }

      // Set the x2 rationals:
      for (auto i = (cpt)known_param_types::known_rational8x2;
           i < (cpt)known_param_types::known_rationalx2_last;
           i++)
      {
         if (i % 2 != 0)
         {
            ret[i] = { 4, content_control_types::uspinner };
         }
         else
         {
            ret[i] = { 4, content_control_types::spinner };
         }
      }

      // Set the x3 rationals:
      for (auto i = (cpt)known_param_types::known_rational8x3;
           i < (cpt)known_param_types::known_rationalx3_last;
           i++)
      {
         if (i % 2 != 0)
         {
            ret[i] = { 6, content_control_types::uspinner };
         }
         else
         {
            ret[i] = { 6, content_control_types::spinner };
         }
      }

      // Set the x4 rationals:
      for (auto i = (cpt)known_param_types::known_rational8x4;
           i < (cpt)known_param_types::known_rationalx4_last;
           i++)
      {
         if (i % 2 != 0)
         {
            ret[i] = { 8, content_control_types::uspinner };
         }
         else
         {
            ret[i] = { 8, content_control_types::spinner };
         }
      }

      // Set the strings:
      for (auto i = (cpt)known_param_types::known_str8;
           i < (cpt)known_param_types::known_str_last;
           i++)
      {
         ret[i] = { 1, content_control_types::text_box };
      }

      return ret;
   }

   class content_map_0_1 : public icontent_map
   {
      public:
      content_map_0_1() :
         m_map(make_default_map())
      {

      }

      virtual ~content_map_0_1() noexcept = default;

      virtual void release()
      {
         delete this;
      }

      virtual bool contains(content_param_types t) const noexcept
      {
         return m_map.count(t) > 0;
      }

      virtual void insert(content_param_types t, const content_map_data* d)
      {
         m_map[t] = *d;
      }

      virtual size_t control_count(content_param_types t) const
      {
         return m_map.at(t).controls;
      }

      virtual content_control_types control_type(content_param_types t) const
      {
         return m_map.at(t).control_type;
      }

      private:
      ctrl_map m_map;
   };

   qgl_code QGL_CC make_content_map(qgl_version_t v, icontent_map** m) noexcept
   {
      if (!m)
      {
#ifdef DEBUG
         OutputDebugString(L"out_p cannot be nullptr.");
#endif
         return E_INVALIDARG;
      }

      icontent_map* ret = new (std::nothrow) content_map_0_1();

      if (!ret)
      {
#ifdef DEBUG
         OutputDebugString(L"Out of memory!");
#endif
         return E_OUTOFMEMORY;
      }

      *m = ret;
      return S_OK;
   }
}

