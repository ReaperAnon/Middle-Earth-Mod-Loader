#pragma once

#include "framework.h"
#include "interface.h"

extern std::vector<ModLoader::PluginInterface*> pluginEntries;
extern std::vector<HINSTANCE> loadedDlls;
extern std::vector<std::wstring> pluginNames;
extern std::vector<bool> loadMask;

void LoadPlugin(ModLoader::PluginInterface* pluginInterface);

void DisablePlugin(ModLoader::PluginInterface* pluginEntry);