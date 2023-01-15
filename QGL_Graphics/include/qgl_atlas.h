#pragma once
#include "include/qgl_graphics_include.h"
#include "include/Content/Content-Descriptors/qgl_atlas_descriptor.h"
#include "include/Content/qgl_image.h"

namespace qgl::graphics
{
   /*
    A texture atlas. Use "set" to specify which region of the atlas will be
    retrieved when calling "get".
    https://learn.microsoft.com/en-us/windows/win32/direct2d/atlas
    */
   class atlas
   {
      public:
      template<class AtlasEntryIt>
      atlas(const descriptors::atlas_descriptor& desc,
            const image& img,
            AtlasEntryIt first,
            AtlasEntryIt last,
            i2d_context* context_p) :
         m_desc(desc),
         m_image(img)
      {
         make_effect(context_p, desc, first, last);
      }

      template<class AtlasEntryIt>
      atlas(descriptors::atlas_descriptor&& desc,
            image&& img,
            AtlasEntryIt first,
            AtlasEntryIt last) :
         m_desc(std::forward<descriptors::atlas_descriptor>(desc)),
         m_image(std::forward<image>(img))
      {
         make_effect(context_p, desc, first, last);
      }

      /*
       Sets which area of the atlas will be retrieved when calling "get".
       */
      void set(const std::string& name)
      {
         if (m_entries.count(name) == 0)
         {
            std::stringstream error;
            error << name << " is not in the atlas.";
            throw error.str();
         }

         check_result(m_atlasEffect->SetValue(
            D2D1_ATLAS_PROP_INPUT_RECT, m_entries[name]));
      }

      atlas(const atlas&) = default;

      atlas(atlas&&) = default;

      virtual ~atlas() noexcept = default;

      /*
       Retrieves the image that was specified with "set".
       */
      ID2D1Effect* get() noexcept
      {
         return m_atlasEffect.get();
      }

      /*
       Retrieves the image that was specified with "set".
       */
      const ID2D1Effect* get() const noexcept
      {
         return m_atlasEffect.get();
      }

      private:
      template<class AtlasEntryIt>
      void make_effect(i2d_context* context_p,
                       const descriptors::atlas_descriptor& desc,
                       AtlasEntryIt first,
                       AtlasEntryIt last)
      {
         assert(std::distance(first, last) == desc.count);


         // Create the D2D effect
         check_result(context_p->CreateEffect(
            CLSID_D2D1Atlas, m_atlasEffect.put()));
         m_atlasEffect->SetInput(0, m_image.get());

         // Copy the entry descriptors to a hash table.
         for (auto it = first; it != last; it++)
         {
            m_entries[it->name.data()] = from_atlas_entry(*it);
         }

         set(first->name.data());
      }

      D2D1_VECTOR_4F from_atlas_entry(
         const descriptors::atlas_entry_descriptor& desc) const noexcept
      {
         auto width = desc.right() - desc.left();
         auto height = desc.bottom() - desc.top();;
         return D2D1::Vector4F(static_cast<float>(desc.left()),
                               static_cast<float>(desc.top()),
                               static_cast<float>(width),
                               static_cast<float>(height));
      }

      descriptors::atlas_descriptor m_desc;
      std::unordered_map<std::string, D2D1_VECTOR_4F> m_entries;
      pptr<ID2D1Effect> m_atlasEffect;
      image m_image;
   };
}