#include "pch.h"
#include <winrt/base.h>
#include "ContentProject.h"
#include "ContentProject/ContentProjectEntry.h"
#include "ContentProjectEntry.g.h"
#include "ContentInfo/ContentInfo.h"
#include "ContentInfo.g.h"
#include <winrt/Windows.Security.Cryptography.h>
#include <winrt/Windows.Storage.h>
#include <winrt/Windows.Storage.Streams.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <QGLContent.h>

using namespace winrt;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::Storage;
using namespace winrt::Windows::Storage::Streams;
using namespace winrt::Windows::Security::Cryptography;

namespace winrt::QGL_Projection::implementation
{
   ContentProject::ContentProject() :
      m_info()
   {
      ObservableEntries(winrt::single_threaded_observable_vector<IInspectable>());
   }

   QGL_Projection::ContentInfo ContentProject::ContentInfo()
   {
      assert(m_info != nullptr);
      return m_info;
   }

   void ContentProject::ContentInfo(QGL_Projection::ContentInfo const& value)
   {
      if (m_info != value)
      {
         m_info = value;
         m_propertyChanged(*this, PropertyChangedEventArgs{ L"ContentInfo" });
      }
   }

   IObservableVector<IInspectable> ContentProject::ObservableEntries()
   {
      assert(m_observableEntries != nullptr);
      return m_observableEntries;
   }

   void ContentProject::ObservableEntries(IObservableVector<IInspectable> const& value)
   {
      if (m_observableEntries != value)
      {
         m_observableEntries = value;
         m_propertyChanged(*this, PropertyChangedEventArgs{ L"ObservableEntries" });
      }
   }

   IAsyncOperation<bool> ContentProject::LoadFromFileAsync(StorageFile const f,
                                                           IVector<ResourceType> const& supported)
   {
      auto copySupported = supported;
      //Get a buffer for the file.
      IRandomAccessStream stream{ co_await f.OpenAsync(FileAccessMode::Read) };

      if (stream.Size() == 0)
      {
         return true;
      }

      IInputStream inputStream{ stream.GetInputStreamAt(0) };
      DataReader dataReader{ inputStream };
      dataReader.UnicodeEncoding(UnicodeEncoding::Utf16LE);
      dataReader.ByteOrder(ByteOrder::LittleEndian);

      co_await dataReader.LoadAsync(stream.Size());

      //Read the magic number:
      auto projectMagicNumber = dataReader.ReadUInt64();
      if (projectMagicNumber != qgl::content::QGL_CONTENT_PROJECT_MAGIC_NUMBER)
      {
         return false;
      }

      //Read the content info:
      ContentInfo(ContentInfo::ReadContentInfoFromBuffer(dataReader, copySupported));

      //Read the number of dictionary entries.
      auto numEntries = dataReader.ReadUInt64();

      m_observableEntries = winrt::single_threaded_observable_vector<IInspectable>();
      for (uint64_t entryIndex = 0; entryIndex < numEntries; entryIndex++)
      {
         //Check the entry's magic number.
         uint64_t magic = dataReader.ReadUInt64();
         if (magic != qgl::content::QGL_CONTENT_PROJECT_ENTRY_SEPERATOR_MAGIC_NUMBER)
         {
            return false;
         }

         //Read the content info.
         auto info = ContentInfo::ReadContentInfoFromBuffer(dataReader, copySupported);

         //Check the next magic number.
         magic = dataReader.ReadUInt64();
         if (magic != qgl::content::QGL_CONTENT_PROJECT_ENTRY_SEPERATOR_MAGIC_NUMBER)
         {
            return false;
         }

         //Read the number of characters in the file path.
         uint32_t filePathLen = dataReader.ReadUInt32();

         //Read the file path.
         auto entryPath = dataReader.ReadString(filePathLen);
         auto newEntry = winrt::make<QGL_Projection::implementation::ContentProjectEntry>(entryPath, info);
         m_observableEntries.Append(newEntry);
      }

      dataReader.Close();

      //m_propertyChanged(*this, PropertyChangedEventArgs{ L"ObservableEntries" });
      return true;
   }

   IAsyncOperation<bool> ContentProject::SaveToFileAsync(StorageFile const f)
   {
      //Get a buffer for the file.
      IRandomAccessStream stream{ co_await f.OpenAsync(FileAccessMode::ReadWrite) };
      auto outputStream = stream.GetOutputStreamAt(0);

      DataWriter dataWriter{ outputStream };
      dataWriter.UnicodeEncoding(UnicodeEncoding::Utf16LE);
      dataWriter.ByteOrder(ByteOrder::LittleEndian);

      //Write a 64-bit magic number.
      dataWriter.WriteUInt64(qgl::content::QGL_CONTENT_PROJECT_MAGIC_NUMBER);

      //Write the content info:
      ContentInfo::WriteContentInfoToBuffer(m_info, dataWriter);

      //Write the number of entries in the dictionary:
      dataWriter.WriteUInt64(m_observableEntries.Size());

      //Write the dictionary. Each entry is a magic number, the content info,
      //another magic number, length of the path, and the path. 
      for (auto obEntry : m_observableEntries)
      {
         auto entry = winrt::unbox_value<QGL_Projection::ContentProjectEntry>(obEntry);
         auto entryInfo = entry.ContentInfo();

         dataWriter.WriteUInt64(qgl::content::QGL_CONTENT_PROJECT_ENTRY_SEPERATOR_MAGIC_NUMBER);
         ContentInfo::WriteContentInfoToBuffer(entryInfo, dataWriter);
         dataWriter.WriteUInt64(qgl::content::QGL_CONTENT_PROJECT_ENTRY_SEPERATOR_MAGIC_NUMBER);
         dataWriter.WriteUInt32(entry.FilePath().size());
         dataWriter.WriteString(entry.FilePath());
      }

      auto bytes = co_await dataWriter.StoreAsync();
      co_await dataWriter.FlushAsync();

      dataWriter.DetachStream();
      return true;
   }

   winrt::event_token ContentProject::PropertyChanged(PropertyChangedEventHandler const& handler)
   {
      return m_propertyChanged.add(handler);
   }

   void ContentProject::PropertyChanged(winrt::event_token const& token) noexcept
   {
      m_propertyChanged.remove(token);
   }
}
