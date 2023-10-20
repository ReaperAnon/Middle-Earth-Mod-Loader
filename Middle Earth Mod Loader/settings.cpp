#include "settings.h"
#include "input.h"
#include "interface.h"
#include "dllmain.h"
#include "dxgi_hook.h"

CSimpleIni Settings::iniFile;
std::string Settings::iniFileName = ".\\modloader\\Mod Loader.ini";

float Settings::fontScale;
bool Settings::saveWhenClosing;
bool Settings::saveOnDefault;
bool Settings::showAllMods;
bool Settings::debugState;
int Settings::logLimit;

void Settings::ReadSettings()
{
	GameKey<ImGuiKey>* gameKey;

	iniFile.LoadFile(iniFileName.c_str());
	
	// Settings
	saveWhenClosing = iniFile.GetBoolValue(L"General", L"Autosave", false);
	saveOnDefault = iniFile.GetBoolValue(L"General", L"SaveOnDefault", false);
	showAllMods = iniFile.GetBoolValue(L"General", L"ShowAllMods", true);
	debugState = iniFile.GetBoolValue(L"General", L"DebugState", false);
	fontScale = iniFile.GetDoubleValue(L"General", L"FontScale", 1);
	logLimit = iniFile.GetLongValue(L"General", L"LogLimit", 5);
	ImGui::GetIO().FontGlobalScale = fontScale;

	gameKey = Input::MakeGameKey(iniFile.GetValue(L"General", L"MenuKey", L"581 0 0"));
	gameKey->PressAction = Interface::ToggleModConfigMenu;
	gameKey = Input::MakeGameKey(iniFile.GetValue(L"General", L"LogKey", L"594 0 0"));
	gameKey->PressAction = Interface::ToggleLogMenu;
	gameKey = Input::MakeGameKey(ImGuiKey_Escape);
	gameKey->PressAction = Interface::ExitMenus;

	// Plugins
	for (int i = 1; i < pluginEntries.size(); i++)
		pluginEntries[i]->isEnabled = iniFile.GetBoolValue(L"Plugins", pluginEntries[i]->fileNameUnicode.c_str(), true);
}

void Settings::SetDefaultSettings()
{
	saveWhenClosing = false;
	saveOnDefault = false;
	showAllMods = true;
	debugState = false;
	fontScale = 1.0f;
	logLimit = 5;
	ImGui::GetIO().FontGlobalScale = fontScale;

	Input::GetGameKey(0)->RemapKeys(ImGuiKey_F10, ImGuiKey_None, ImGuiKey_None);
	Input::GetGameKey(1)->RemapKeys(ImGuiKey_GraveAccent, ImGuiKey_None, ImGuiKey_None);
}

void Settings::SaveSettings()
{
	wchar_t buffer[16];
	GameKey<ImGuiKey>* gameKey;

	gameKey = Input::GetGameKey(0);
	swprintf(buffer, 16, L"%i %i %i", gameKey->FirstKey, gameKey->SecondKey, gameKey->ModKey);
	iniFile.SetValue(L"General", L"MenuKey", buffer);
	gameKey = Input::GetGameKey(1);
	swprintf(buffer, 16, L"%i %i %i", gameKey->FirstKey, gameKey->SecondKey, gameKey->ModKey);
	iniFile.SetValue(L"General", L"LogKey", buffer);

	iniFile.SetBoolValue(L"General", L"Autosave", saveWhenClosing);
	iniFile.SetBoolValue(L"General", L"SaveOnDefault", saveOnDefault);
	iniFile.SetBoolValue(L"General", L"ShowAllMods", showAllMods);
	iniFile.SetBoolValue(L"General", L"DebugState", debugState);
	iniFile.SetDoubleValue(L"General", L"FontScale", fontScale);
	iniFile.SetLongValue(L"General", L"LogLimit", logLimit);

	iniFile.SaveFile(iniFileName.c_str());
}

void Settings::SavePlugins()
{
	for(int i = 1; i < pluginEntries.size(); i++)
		iniFile.SetBoolValue(L"Plugins", pluginEntries[i]->fileNameUnicode.c_str(), pluginEntries[i]->isEnabled);

	iniFile.SaveFile(iniFileName.c_str());
}