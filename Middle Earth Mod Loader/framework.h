#pragma once

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

#define IMGUI_DISABLE_OBSOLETE_FUNCTIONS

#include <windows.h>
#include <functional>
#include <vector>
#include <string>
#include <mutex>
#include <algorithm>
#include <chrono>
#include <format>
#include <fstream>

#include <input_processing.h>
#include <SimpleIni.h>
#include <MinHook.h>
#include <imgui_hotkey.h>
#include <imgui_fontscale.h>
#include <modloaderapi.h>
#include <memorytool.h>

#include <d3d11.h>
#include <dxgi1_2.h>
#include <wrl/client.h>

#include <imgui.h>
#include <imgui_impl_dx11.h>
#include "imgui_impl_win32.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "libMinHook.x64.lib")

typedef void(__cdecl* p_OnLoad)();
typedef void(__cdecl* p_PluginUnload)();
typedef void(__cdecl* p_PluginAPI)(ModLoader::PluginAPI*);
typedef void(__cdecl* p_LogAPI)(ModLoader::LogAPI*);
typedef void(__cdecl* p_MenuAPI)(ModLoader::MenuAPI*);
