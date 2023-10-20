#pragma once

#include "framework.h"

namespace ModLoader
{
	struct VersionInfo
	{
		std::vector<int> versionNum;

		VersionInfo() { versionNum = { 1, 0, 0 }; }
		VersionInfo(std::vector<int> version) : versionNum(version) {}
	};


	class LogInterface : public LogAPI
	{
	private:
		std::string* pluginName = nullptr;

	public:
		LogInterface(std::string* namePtr) : pluginName(namePtr) {}

		void Log(const char* fmt, ...) override;

		void LogError(const char* fmt, ...) override;
	};


	class MenuInterface : public MenuAPI
	{
		struct MenuEntry
		{
			std::function<void()> drawFunction = nullptr;
			std::function<void()> defaultFunction = nullptr;
			std::function<void()> saveFunction = nullptr;
			std::string menuName = "";

			MenuEntry(std::function<void()> drawFunc, std::function<void()> defaultFunc, std::function<void()> saveFunc, std::string menuName = "") : drawFunction(drawFunc), defaultFunction(defaultFunc), saveFunction(saveFunc), menuName(menuName) {}
		};

	private:
		ImGuiContext* GetContext() const override;

		void GetAllocators(ImGuiMemAllocFunc* allocFunc, ImGuiMemFreeFunc* freeFunc, void** userData) const override;

	public:
		std::vector<MenuEntry*> menuEntries;

		std::vector<std::function<void()>> drawCallEntries;

		~MenuInterface();

		bool HasMenuEntries();

		bool HasDefaultCallbacks();

		bool HasSaveCallbacks();

		void DefaultAll();

		void SaveAll();

		void ClearEntries();

		void RegisterMenuEntry(std::function<void()> drawFunction, std::function<void()> defaultFunction = nullptr, std::function<void()> saveFunction = nullptr, std::string menuName = "") override;

		void RegisterDrawCallEntry(std::function<void()> drawFunction) override;

		void DisplayNotice(const char* noticeText) override;

		bool IsMenuOpen() const override;

		bool IsDebugActive() const override;

		bool IsGamePaused() const override;

		void SetGamePaused(bool state) override;
	};


	class PluginInterface : public PluginAPI
	{
	public:
		bool isEnabled = true;
		HINSTANCE pluginHandle;
		std::wstring fileNameUnicode;
		std::string fileNameANSI;
		std::string pluginName;
		std::string shortName;
		std::string pluginDesc;

		VersionInfo versionInfo;

		LogInterface logInterface { &shortName };

		MenuInterface menuInterface;

		PluginInterface(const std::wstring& fileNameW, const std::string& fileNameA, HINSTANCE handle);
		
		~PluginInterface();

		void RegisterPlugin(const std::string& pluginName, const std::string& shortName, const std::vector<int>& pluginVersion = {}, const std::string& pluginDescription = "") override;
	};
}

namespace Interface
{
	void ShowRegisteredDrawCalls();

	extern "C" bool IsGamePaused();

	void SetGamePaused(bool state);

	extern "C" bool IsModMenuOpen();

	void ToggleModConfigMenu(void*);

	void ToggleLogMenu(void*);

	void ExitMenus(void*);

	void ShowNoticeMenu(bool* showMenu);

	void DrawNoticeMenu();

	void DisplayNotice(const char* text);

	void ShowLogMenu(bool* showMenu);

	void ShowFontScaleMenu(bool* showMenu);

	void ShowModConfigMenu(bool* showMenu);

	void ShowRegisteredModMenus();
}