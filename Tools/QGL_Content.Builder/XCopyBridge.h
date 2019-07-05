#pragma once
#include "CommonIncludes.h"
using namespace winrt::Windows;

bool VerifyInvokeSupported()
{
   return Foundation::Metadata::ApiInformation::IsApiContractPresent(
      L"Windows.ApplicationModel.FullTrustAppContract", 
      1, 
      0);
}

void SetXCopyParameters(const std::wstring& params)
{
   Storage::ApplicationData::Current().LocalSettings().Values().Insert(
      L"params",
      Foundation::PropertyValue::CreateString(params));
}

void InvokeXCopy()
{
   ApplicationModel::FullTrustProcessLauncher::
      LaunchFullTrustProcessForCurrentAppAsync(L"Parameters").get();
}