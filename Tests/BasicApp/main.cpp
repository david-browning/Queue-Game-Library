#include "pch.h"
#include "App.h"

int __stdcall wWinMain(HINSTANCE, HINSTANCE, PWSTR, int)
{
   CoreApplication::Run(make<App>());
}
