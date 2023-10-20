#include "dllmain.h"
#include "interface.h"
#include "dxgi_hook.h"
#include "settings.h"
#include "modmenu.h"
#include "input.h"
#include "log.h"
#include "gamehook.h"

extern "C"
{
	void* functionPointer0;
	void* functionPointer1;
	void* functionPointer2;
	void* functionPointer3;
	void* functionPointer4;
	void* functionPointer5;
	void* functionPointer6;
	void* functionPointer7;
	void* functionPointer8;
	void* functionPointer9;
	void* functionPointer10;
	void* functionPointer11;
	void* functionPointer12;
	void* functionPointer13;
	void* functionPointer14;
	void* functionPointer15;
	void* functionPointer16;
	void* functionPointer17;
	void* functionPointer18;
	void* functionPointer19;
	void* functionPointer20;
	void* functionPointer21;
	void* functionPointer22;
	void* functionPointer23;
	void* functionPointer24;
	void* functionPointer25;
	void* functionPointer26;
	void* functionPointer27;
	void* functionPointer28;
	void* functionPointer29;
	void* functionPointer30;
	void* functionPointer31;
	void* functionPointer32;
	void* functionPointer33;
	void* functionPointer34;
	void* functionPointer35;
	void* functionPointer36;
	void* functionPointer37;
	void* functionPointer38;
	void* functionPointer39;
	void* functionPointer40;
	void* functionPointer41;
	void* functionPointer42;
	void* functionPointer43;
	void* functionPointer44;
	void* functionPointer45;
	void* functionPointer46;
	void* functionPointer47;
	void* functionPointer48;
	void* functionPointer49;
	void* functionPointer50;
	void* functionPointer51;
	void* functionPointer52;
	void* functionPointer53;
	void* functionPointer54;
	void* functionPointer55;
	void* functionPointer56;
	void* functionPointer57;
	void* functionPointer58;
	void* functionPointer59;
	void* functionPointer60;
	void* functionPointer61;
	void* functionPointer62;
	void* functionPointer63;
	void* functionPointer64;
	void* functionPointer65;
	void* functionPointer66;
	void* functionPointer67;
	void* functionPointer68;
	void* functionPointer69;
	void* functionPointer70;
	void* functionPointer71;
	void* functionPointer72;
}
void** functionPointers[] = { &functionPointer0, &functionPointer1, &functionPointer2, &functionPointer3, &functionPointer4, &functionPointer5, &functionPointer6, &functionPointer7, &functionPointer8, &functionPointer9, &functionPointer10, &functionPointer11, &functionPointer12, &functionPointer13, &functionPointer14, &functionPointer15, &functionPointer16, &functionPointer17, &functionPointer18, &functionPointer19, &functionPointer20, &functionPointer21, &functionPointer22, &functionPointer23, &functionPointer24, &functionPointer25, &functionPointer26, &functionPointer27, &functionPointer28, &functionPointer29, &functionPointer30, &functionPointer31, &functionPointer32, &functionPointer33, &functionPointer34, &functionPointer35, &functionPointer36, &functionPointer37, &functionPointer38, &functionPointer39, &functionPointer40, &functionPointer41, &functionPointer42, &functionPointer43, &functionPointer44, &functionPointer45, &functionPointer46, &functionPointer47, &functionPointer48, &functionPointer49, &functionPointer50, &functionPointer51, &functionPointer52, &functionPointer53, &functionPointer54, &functionPointer55, &functionPointer56, &functionPointer57, &functionPointer58, &functionPointer59, &functionPointer60, &functionPointer61, &functionPointer62, &functionPointer63, &functionPointer64, &functionPointer65, &functionPointer66, &functionPointer67, &functionPointer68, &functionPointer69, &functionPointer70, &functionPointer71, &functionPointer72 };
LPCSTR functionNames[] = { "BinkAllocateFrameBuffers", "BinkClose", "BinkCloseTrack", "BinkControlBackgroundIO", "BinkCopyToBuffer", "BinkCopyToBufferRect", "BinkDoFrame", "BinkDoFrameAsync", "BinkDoFrameAsyncMulti", "BinkDoFrameAsyncWait", "BinkDoFramePlane", "BinkFreeGlobals", "BinkGetError", "BinkGetFrameBuffersInfo", "BinkGetGPUDataBuffersInfo", "BinkGetKeyFrame", "BinkGetPlatformInfo", "BinkGetRealtime", "BinkGetRects", "BinkGetSummary", "BinkGetTrackData", "BinkGetTrackID", "BinkGetTrackMaxSize", "BinkGetTrackType", "BinkGoto", "BinkLogoAddress", "BinkNextFrame", "BinkOpen", "BinkOpenDirectSound", "BinkOpenMiles", "BinkOpenTrack", "BinkOpenWaveOut", "BinkOpenWithOptions", "BinkOpenXAudio2", "BinkOpenXAudio27", "BinkOpenXAudio28", "BinkPause", "BinkRegisterFrameBuffers", "BinkRegisterGPUDataBuffers", "BinkRequestStopAsyncThread", "BinkRequestStopAsyncThreadsMulti", "BinkService", "BinkSetError", "BinkSetFileOffset", "BinkSetFrameRate", "BinkSetIO", "BinkSetIOSize", "BinkSetMemory", "BinkSetOSFileCallbacks", "BinkSetPan", "BinkSetSimulate", "BinkSetSoundOnOff", "BinkSetSoundSystem", "BinkSetSoundSystem2", "BinkSetSoundTrack", "BinkSetSpeakerVolumes", "BinkSetVideoOnOff", "BinkSetVolume", "BinkSetWillLoop", "BinkShouldSkip", "BinkStartAsyncThread", "BinkUtilCPUs", "BinkUtilFree", "BinkUtilMalloc", "BinkUtilMutexCreate", "BinkUtilMutexDestroy", "BinkUtilMutexLock", "BinkUtilMutexLockTimeOut", "BinkUtilMutexUnlock", "BinkWait", "BinkWaitStopAsyncThread", "BinkWaitStopAsyncThreadsMulti", "RADTimerRead" };


const std::wstring pluginDir = L".\\plugins\\";

std::vector<ModLoader::PluginInterface*> pluginEntries;
HINSTANCE binkInstance = nullptr;
std::vector<bool> loadMask;

void FindPlugins()
{
	WIN32_FIND_DATA findData;
	HANDLE foundHandle = FindFirstFile((pluginDir + L"*.dll").c_str(), &findData);

	if (foundHandle != INVALID_HANDLE_VALUE) do
	{
		if ((findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0)
			continue;

		#pragma warning(push)
		#pragma warning(disable: 4244)
		auto convert = [](const std::wstring& s) {
			std::string temp(s.length(), ' ');
			std::copy(s.begin(), s.end(), temp.begin());
			return temp;
		};
		#pragma warning(pop)

		std::wstring fileName(findData.cFileName);
		pluginEntries.push_back(new ModLoader::PluginInterface(fileName, convert(fileName), nullptr));

	} while (FindNextFile(foundHandle, &findData));

	FindClose(foundHandle);
}

int TryLoadPlugin(ModLoader::PluginInterface* pluginInterface)
{
	try
	{
		HINSTANCE moduleHandle = LoadLibrary((pluginDir + pluginInterface->fileNameUnicode).c_str());
		if (moduleHandle == NULL)
			return 2;

		// just for compatibility
		p_OnLoad onLoad = reinterpret_cast<p_OnLoad>(GetProcAddress(moduleHandle, "OnLoad"));
		if (onLoad)
			onLoad();

		pluginInterface->pluginHandle = moduleHandle;
		p_LogAPI onRegisterLogAPI = reinterpret_cast<p_LogAPI>(GetProcAddress(moduleHandle, "OnRegisterLogAPI"));
		if (onRegisterLogAPI)
			onRegisterLogAPI(&pluginInterface->logInterface);

		p_MenuAPI onRegisterMenuAPI = reinterpret_cast<p_MenuAPI>(GetProcAddress(moduleHandle, "OnRegisterMenuAPI"));
		if (onRegisterMenuAPI)
			onRegisterMenuAPI(&pluginInterface->menuInterface);

		p_PluginAPI onPluginLoad = reinterpret_cast<p_PluginAPI>(GetProcAddress(moduleHandle, "OnPluginLoad"));
		if (onPluginLoad)
			onPluginLoad(pluginInterface);

		return 0;
	}
	#pragma warning(disable: 4101)
	catch (std::exception& _) {}

	return 1;
}

void LoadPlugin(ModLoader::PluginInterface* pluginInterface)
{
	std::wstring errorMsg;
	int errorCode = TryLoadPlugin(pluginInterface);
	switch (errorCode)
	{
		case 1:
			errorMsg = L"LoadLibrary crashed on: " + pluginInterface->fileNameUnicode;
			MessageBox(NULL, errorMsg.c_str(), L"ShadowOfWar.exe", 0);
			throw std::exception();
		break;

		case 2:
			errorMsg = L"Can't load file: " + pluginInterface->fileNameUnicode;
			MessageBox(NULL, errorMsg.c_str(), L"ShadowOfWar.exe", 0);
			throw std::exception();
		break;

		default:
			break;
	}
}

void LoadPlugins()
{
	for(int i = 1; i < pluginEntries.size(); i++) {
		if (pluginEntries[i]->isEnabled)
			LoadPlugin(pluginEntries[i]);
	}
}

void ModLoaderThread()
{
	Sleep(10000);

	MH_Initialize();
	std::thread(GameHook::InitalizeMod).detach();
	ModMenu::RegisterModLoaderMenu();
	FindPlugins();

	DXGI::HookDXGI();
	Settings::ReadSettings();
	Input::StartInputProcessing();

	LoadPlugins();
}

void DisablePlugin(ModLoader::PluginInterface* pluginInterface)
{
	p_PluginUnload OnPluginUnload = reinterpret_cast<p_PluginUnload>(GetProcAddress(pluginInterface->pluginHandle, "OnPluginUnload"));
	if (OnPluginUnload)
		OnPluginUnload();
	
	pluginInterface->menuInterface.ClearEntries();
	FreeLibrary(pluginInterface->pluginHandle);
	pluginInterface->pluginHandle = nullptr;
}

void DisableModLoader()
{
	Input::StopInputProcessing();
	DXGI::UnhookDXGI();
	MH_Uninitialize();

	for (size_t i = pluginEntries.size() - 1; i >= 0; i--) {
		if (pluginEntries[i]->pluginHandle)
			DisablePlugin(pluginEntries[i]);

		delete pluginEntries[i];
	}

	FreeLibrary(binkInstance);
	delete binkInstance;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD callReason, LPVOID lpReserved)
{
	if (callReason == DLL_PROCESS_ATTACH)
	{
		binkInstance = LoadLibrary(L"bink2w64_.dll");
		if (!binkInstance)
		{
			MessageBox(NULL, L"The bink2w64_.dll failed to load. Make sure you renamed the original according to the instructions.", L"ShadowOfWar.exe", 0);
			return FALSE;
		}

		for (int i = 0; i < 73; ++i)
			*functionPointers[i] = GetProcAddress(binkInstance, functionNames[i]);

		std::thread(ModLoaderThread).detach();
	}

	return TRUE;
}