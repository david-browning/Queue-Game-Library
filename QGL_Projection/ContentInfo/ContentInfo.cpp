#include "pch.h"
#include "ContentInfo.h"
#include "ContentLoader.h"
#include "ContentLoader.g.h"
#include "ResourceType.h"
#include "ResourceType.g.h"
#include "CompilerVersion.h"
#include "CompilerVersion.g.h"

using namespace qgl;
using namespace qgl::content;
using namespace winrt::Windows::Storage;
using namespace winrt::Windows::Foundation;
using namespace winrt::Windows::UI::Xaml::Data;
using namespace winrt::Windows::Foundation::Collections;

namespace winrt::QGL_Projection::implementation
{
   QGL_Projection::ResourceType FindModel(uint16_t resourceID,
                                          IVector<QGL_Projection::ResourceType> const& a)
   {
      for (auto r : a)
      {
         if (r.Value() == resourceID)
         {
            return r;
         }
      }

      return nullptr;
   }

   QGL_Projection::ContentLoader FindModel(uint16_t loaderID,
                                           const QGL_Projection::ResourceType& r)
   {
      for (auto l : r.Loaders())
      {
         if (l.Value() == loaderID)
         {
            return l;
         }
      }

      return nullptr;
   }

   void ContentInfo::WriteContentInfoToBuffer(QGL_Projection::ContentInfo const& info,
                                              Windows::Storage::Streams::DataWriter const& r)
   {
       //Write the compiler version as a 32-bit UINT:
      r.WriteUInt32(info.CompilerVersion().Value());

      //Write the GUID
      r.WriteGuid(info.Guid());

      //Write the resource Type
      r.WriteUInt16(info.ResourceType().Value());

      //Write the loader ID
      r.WriteUInt16(info.ContentLoader().Value());

      //Write the flags
      //IsShared
      //Char Length
      r.WriteBoolean(info.ContentVisible());
      r.WriteBoolean(info.ContentObeyPhysics());

      //Write the length of the content name.
      r.WriteUInt32(info.ContentName().size());

      //Write the content name
      r.WriteString(info.ContentName());
   }

   QGL_Projection::ContentInfo ContentInfo::ReadContentInfoFromBuffer(Streams::DataReader const& r,
                                                                      IVector<QGL_Projection::ResourceType> const& supportedResourceTypes)
   {
      QGL_Projection::ContentInfo ret = winrt::make<QGL_Projection::implementation::ContentInfo>();

      //Read the compiler version.
      auto v = winrt::make<QGL_Projection::implementation::CompilerVersion>(r.ReadUInt32());
      ret.CompilerVersion(v);

      //Read the GUID
      ret.Guid(r.ReadGuid());

      //Read the resource type
      auto resId = r.ReadUInt16();
      auto rType = FindModel(resId, supportedResourceTypes);
      if (rType == nullptr)
      {
         return nullptr;
      }

      ret.ResourceType(rType);

      //Read the loader ID
      auto ldrID = r.ReadUInt16();
      auto ldr = FindModel(ldrID, rType);
      if (ldr == nullptr)
      {
         return nullptr;
      }
      ret.ContentLoader(ldr);

      //Read the flags.
      //IsShared.
      //CharLength
      //Content Visible
      ret.ContentVisible(r.ReadBoolean());
      //Content Obey Physics
      ret.ContentObeyPhysics(r.ReadBoolean());

      //Length of the content name.
      auto nameLen = r.ReadUInt32();

      //Content name.
      ret.ContentName(r.ReadString(nameLen));

      return ret;
   }


   static auto loaderUnknown = winrt::make<QGL_Projection::implementation::ContentLoader>(CONTENT_LOADER_IDS::CONTENT_LOADER_ID_UNKNOWN,
                                                                                          L"Unknown");

   std::vector<QGL_Projection::ContentLoader> unknownLoaders{ loaderUnknown };

   static auto resourceUnknown = winrt::make<QGL_Projection::implementation::ResourceType>(RESOURCE_TYPES::RESOURCE_TYPE_UNKNOWN,
                                                                                           L"Unknown",
                                                                                           unknownLoaders);

   ContentInfo::ContentInfo() :
      m_buffer(),
      m_loaderModel(nullptr),
      m_resourceModel(nullptr),
      m_compilerVersion(nullptr)
   {
      CompilerVersion(QGL_Projection::CompilerVersion::Version_0_1());
      ResourceType(resourceUnknown);
      ContentLoader(loaderUnknown);
      ContentVisible(true);
      ContentObeyPhysics(false);
      ContentName(L"New Content");
   }

   bool ContentInfo::ContentVisible()
   {
      return m_buffer.content_visible();
   }

   void ContentInfo::ContentVisible(bool value)
   {
      if (m_buffer.content_visible() != value)
      {
         m_buffer.content_visible(value);
         m_propertyChanged(*this, PropertyChangedEventArgs{ L"ContentVisible" });
      }
   }

   bool ContentInfo::ContentObeyPhysics()
   {
      return m_buffer.obey_physics();
   }

   void ContentInfo::ContentObeyPhysics(bool value)
   {
      if (m_buffer.obey_physics() != value)
      {
         m_buffer.obey_physics(value);
         m_propertyChanged(*this, PropertyChangedEventArgs{ L"ContentObeyPhysics" });
      }
   }

   hstring ContentInfo::ContentName()
   {
      return m_buffer.name();
   }

   void ContentInfo::ContentName(hstring const& value)
   {
      if (m_buffer.name() != value)
      {
         m_buffer.name(value);
         m_propertyChanged(*this, PropertyChangedEventArgs{ L"ContentName" });
      }
   }

   QGL_Projection::ResourceType ContentInfo::ResourceType()
   {
      return m_resourceModel;
   }

   void ContentInfo::ResourceType(QGL_Projection::ResourceType const& value)
   {
      //assert(value != nullptr);
      if (value != nullptr && m_resourceModel != value)
      {
         m_resourceModel = value;
         m_buffer.resource_type(static_cast<RESOURCE_TYPES>(m_resourceModel.Value()));
         m_propertyChanged(*this, PropertyChangedEventArgs{ L"ResourceType" });
      }
   }

   QGL_Projection::ContentLoader ContentInfo::ContentLoader()
   {
      return m_loaderModel;
   }

   void ContentInfo::ContentLoader(QGL_Projection::ContentLoader const& value)
   {
      if (m_loaderModel != value)
      {
         m_loaderModel = value;
         if (m_loaderModel != nullptr)
         {
            m_buffer.loader_id(static_cast<qgl::content::CONTENT_LOADER_IDS>(m_loaderModel.Value()));
         }

         m_propertyChanged(*this, PropertyChangedEventArgs{ L"ContentLoader" });
      }
   }

   QGL_Projection::CompilerVersion ContentInfo::CompilerVersion()
   {
      return m_compilerVersion;
   }

   void ContentInfo::CompilerVersion(QGL_Projection::CompilerVersion const& value)
   {
      assert(value != nullptr);
      if (m_compilerVersion != value)
      {
         m_compilerVersion = value;
         m_propertyChanged(*this, PropertyChangedEventArgs{ L"CompilerVersion" });
      }
   }

   winrt::guid ContentInfo::Guid()
   {
      return winrt::guid(m_buffer.guid());
   }

   void ContentInfo::Guid(winrt::guid const & g)
   {
      m_buffer.guid(g);
   }

   winrt::event_token ContentInfo::PropertyChanged(Windows::UI::Xaml::Data::PropertyChangedEventHandler const& handler)
   {
      return m_propertyChanged.add(handler);
   }

   void ContentInfo::PropertyChanged(winrt::event_token const& token) noexcept
   {
      m_propertyChanged.remove(token);
   }
}
