#pragma once
#include <Unknwn.h>

#include <cstdio>
#include <cstdint>
#include <cstring>
#include <cmath>
#include <algorithm>
#include <string>
#include <iostream>
#include <fstream>
#include <list>
#include <queue>
#include <forward_list>
#include <vector>
#include <iomanip>
#include <bitset>
#include <memory>
#include <mutex>
#include <stdexcept>
#include <sstream>
#include <atomic>
#include <QGLModel.h>

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <d3d12.h>
#include <d2d1_3.h>
#include <dxgi1_6.h>
#include <d3d11on12.h>
#include <dwrite_3.h>
#include <DirectXMath.h>
#include <DirectXCollision.h>
#include <DirectXTex.h>
#include <d2d1_1helper.h>
#include <d2d1_3helper.h>
#include <d3d12shader.h>
#include <d3dcompiler.h>

#include <windows.h>
#include <winrt/base.h>
#include "winrt/Windows.ApplicationModel.Core.h"
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.UI.Core.h>
#include <winrt/Windows.UI.ViewManagement.Core.h>
#include <winrt/Windows.Graphics.Display.h>
#ifdef _DEBUG
#include <D3d12SDKLayers.h>
#endif

#include <QGLModel.h>
#include <QGLMemory.h>
#include <QGLMath.h>
