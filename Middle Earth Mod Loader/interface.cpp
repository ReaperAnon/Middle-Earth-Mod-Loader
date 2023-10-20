#include "interface.h"
#include "settings.h"
#include "log.h"
#include "dllmain.h"

bool wasSaved = true;
bool showLogMenu = false;
bool showNoticeMenu = false;
bool showFontScaleMenu = false;
bool showModConfigMenu = false;
bool isGamePaused = false;
std::string noticeText = "";

void Interface::ShowRegisteredDrawCalls()
{
    for (auto& pluginEntry : pluginEntries)
    {
        for (auto& drawCallEntry : pluginEntry->menuInterface.drawCallEntries)
            drawCallEntry();
    }
}

bool Interface::IsGamePaused()
{
    return isGamePaused || IsModMenuOpen();
}

bool Interface::IsModMenuOpen()
{
    return showModConfigMenu || showNoticeMenu|| showFontScaleMenu;
}

void Interface::SetGamePaused(bool state)
{
    isGamePaused = state;
}

void Interface::ToggleModConfigMenu(void*) // called via hotkey
{
    showModConfigMenu = !showModConfigMenu;
    if (!showModConfigMenu) {
        showFontScaleMenu = false;
        showNoticeMenu = false;
    }
}

void Interface::ToggleLogMenu(void*) // called via hotkey
{
    showLogMenu = !showLogMenu;
}

void Interface::ExitMenus(void*) // called via hotkey
{
    if (showNoticeMenu)
        showNoticeMenu = false;
    else
        showModConfigMenu = false;
}

void Interface::ShowNoticeMenu(bool* showMenu)
{
    if (!*showMenu)
        return;

    ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
    if (ImGui::Begin("Notice", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::FocusWindow(ImGui::GetCurrentWindow());
        ImGui::Text(noticeText.c_str());

        float size = ImGui::CalcTextSize("OK").x + ImGui::GetStyle().FramePadding.x * 2.0f;
        float avail = ImGui::GetContentRegionAvail().x;
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ((avail - size) * 0.5f)); // center the OK button
        if (ImGui::Button("OK")) {
            *showMenu = false;
        }
    }
    ImGui::End();
}

void Interface::DisplayNotice(const char* text)
{
    noticeText = text;
    showNoticeMenu = true;
}

void Interface::ShowLogMenu(bool* showMenu)
{
    if (!*showMenu)
        return;

    ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);
    ModLoaderLog::GetSingleton()->Show("Log", showMenu);
}

void Interface::ShowFontScaleMenu(bool* showMenu)
{
    if (!*showMenu)
        return;

    ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
    if (ImGui::Begin("Set Font Scale", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::FocusWindow(ImGui::GetCurrentWindow());
        ImGui::SliderFloat("Scale", &Settings::fontScale, 0.5f, 3.f, "%.2f", ImGuiSliderFlags_AlwaysClamp);
        if (ImGui::Button("OK")) { ImGui::GetIO().FontGlobalScale = Settings::fontScale; *showMenu = false; }
    }
    ImGui::End();
}

void Interface::ShowModConfigMenu(bool* showMenu)
{
    if (!*showMenu) {
        if (Settings::saveWhenClosing && !wasSaved) {
            wasSaved = true;
            for (auto& pluginEntry : pluginEntries) {
                for (auto& menuEntry : pluginEntry->menuInterface.menuEntries) {
                    if (menuEntry->saveFunction)
                        menuEntry->saveFunction();
                }
            }
        }
        return;
    }

    wasSaved = false;
    ImGuiIO& io = ImGui::GetIO();
    bool defaultSaveLast = Settings::saveOnDefault;

    ImGui::SetNextWindowSize(ImVec2(io.DisplaySize.x / 2.f, io.DisplaySize.y / 2.f), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x / 2.f, io.DisplaySize.y / 2.f), ImGuiCond_FirstUseEver, ImVec2(0.5f, 0.5f));
    if (ImGui::Begin("Mod Configuration Menu", showMenu, ImGuiWindowFlags_MenuBar))
    {
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::Button("Show Log")) { showLogMenu = !showLogMenu; }
            if (ImGui::Button("Font Scale")) { showFontScaleMenu = !showFontScaleMenu; }
            ImGui::EndMenuBar();
        }

        // Mod Listing
        static int modIdx = 0;
        int lastModIdx = modIdx;
        {
            bool wasSeparated = false;
            ImGui::BeginChild("modListing", ImVec2(ImGui::GetWindowWidth() * 0.3334f, 0), true);
            for (int i = 0; i < pluginEntries.size(); i++) {
                if (!pluginEntries[i]->isEnabled)
                    continue;

                if (!pluginEntries[i]->menuInterface.HasMenuEntries())
                    continue;

                if (ImGui::Selectable(pluginEntries[i]->pluginName.c_str(), modIdx == i))
                    modIdx = i;
            }

            if (Settings::showAllMods) {
                for (int i = 1; i < pluginEntries.size(); i++) {
                    if (!pluginEntries[i]->isEnabled)
                        continue;

                    if (pluginEntries[i]->menuInterface.HasMenuEntries())
                        continue;

                    if (!wasSeparated) {
                        ImGui::Spacing();
                        ImGui::Separator();
                        ImGui::Spacing();
                        wasSeparated = true;
                    }

                    if (ImGui::Selectable(pluginEntries[i]->pluginName.c_str(), modIdx == i))
                        modIdx = i;
                }
            }
            ImGui::EndChild();
        }
        ImGui::SameLine();

        // Mod Settings
        ImGui::BeginGroup();
        ImGui::BeginChild("##modSettings", ImVec2(0, -ImGui::GetFrameHeightWithSpacing())); // Leave room for 1 line below
        ImGui::Text(pluginEntries[modIdx]->pluginName.c_str());
        ImGui::SameLine();
        ImGui::TextColored(ImVec4(0.67, 0.67, 0.67, 0.5), "%d.%d.%d", pluginEntries[modIdx]->versionInfo.versionNum[0], pluginEntries[modIdx]->versionInfo.versionNum[1], pluginEntries[modIdx]->versionInfo.versionNum[2]);
        ImGui::Separator();
        ImGui::Spacing();
        ImGui::Spacing();

        static int menuIdx = -1;
        if (lastModIdx != modIdx) // reset last tab index for the upcoming tab bar to 0 to avoid crashes when switching mod menus
            menuIdx = -1;

        if (ImGui::BeginTabBar("##modTabs", ImGuiTabBarFlags_None))
        {
            if (ImGui::BeginTabItem("Description"))
            {
                menuIdx = -1;
                ImGui::Spacing();
                ImGui::Spacing();
                if (pluginEntries[modIdx]->pluginDesc.size())
                    ImGui::TextWrapped(pluginEntries[modIdx]->pluginDesc.c_str());
                else
                    ImGui::TextWrapped("Mod menu for the %s mod.%s", pluginEntries[modIdx]->pluginName.c_str(), !pluginEntries[modIdx]->menuInterface.HasMenuEntries() ? " Has no settings." : "");
                ImGui::EndTabItem();
            }
            for (int i = 0; i < pluginEntries[modIdx]->menuInterface.menuEntries.size(); i++)
            {
                auto& menuEntry = pluginEntries[modIdx]->menuInterface.menuEntries[i];
                if (ImGui::BeginTabItem(menuEntry->menuName.c_str()))
                {
                    menuIdx = i;
                    ImGui::Spacing();
                    if(menuEntry->drawFunction)
                        menuEntry->drawFunction();
                    ImGui::EndTabItem();
                }
            }
            ImGui::EndTabBar();
        }
        ImGui::EndChild();

        if (pluginEntries[modIdx]->menuInterface.HasMenuEntries()) { // don't display buttons if mod has no options to set
            if (menuIdx < 0) // Description menu
            {
                if (pluginEntries[modIdx]->menuInterface.HasDefaultCallbacks()) {
                    if (ImGui::Button("Default")) {
                        pluginEntries[modIdx]->menuInterface.DefaultAll();
                        if (defaultSaveLast)
                            pluginEntries[modIdx]->menuInterface.SaveAll();
                    }

                    if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal)) {
                        ImGui::BeginTooltip();
                        ImGui::SetTooltip("%s", Settings::saveOnDefault ? "Sets all of the mod's options to their defaults and saves them to file." : "Sets all of the mod's options to their defaults.");
                        ImGui::EndTooltip();
                    }
                }
                if (!Settings::saveWhenClosing && pluginEntries[modIdx]->menuInterface.HasSaveCallbacks()) {
                    ImGui::SameLine();
                    if (ImGui::Button("Save"))
                        pluginEntries[modIdx]->menuInterface.SaveAll();

                    if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal)) {
                        ImGui::BeginTooltip();
                        ImGui::SetTooltip("Saves all of the mod's options to file.");
                        ImGui::EndTooltip();
                    }
                }
            }
            else
            {
                bool wasDefaultShown = false;
                if (pluginEntries[modIdx]->menuInterface.menuEntries[menuIdx]->defaultFunction) {
                    wasDefaultShown = true;
                    if (ImGui::Button("Default")) {
                        pluginEntries[modIdx]->menuInterface.menuEntries[menuIdx]->defaultFunction();
                        if (defaultSaveLast && pluginEntries[modIdx]->menuInterface.menuEntries[menuIdx]->saveFunction)
                            pluginEntries[modIdx]->menuInterface.menuEntries[menuIdx]->saveFunction();
                    }

                    if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal)) {
                        ImGui::BeginTooltip();
                        ImGui::SetTooltip("%s", Settings::saveOnDefault ? "Sets the selected tab's options to their defaults and saves them to file." : "Sets the selected tab's options to their defaults.");
                        ImGui::EndTooltip();
                    }
                }
                if (!Settings::saveWhenClosing && pluginEntries[modIdx]->menuInterface.menuEntries[menuIdx]->saveFunction) {
                    if (wasDefaultShown)
                        ImGui::SameLine();
                    if (ImGui::Button("Save"))
                        pluginEntries[modIdx]->menuInterface.menuEntries[menuIdx]->saveFunction();

                    if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal)) {
                        ImGui::BeginTooltip();
                        ImGui::SetTooltip("Saves the selected tab's options to file.");
                        ImGui::EndTooltip();
                    }
                }
            }
        }
        ImGui::EndGroup();
    }
    ImGui::End();
}

void Interface::ShowRegisteredModMenus()
{
    ShowNoticeMenu(&showNoticeMenu);
    ShowLogMenu(&showLogMenu);
    ShowFontScaleMenu(&showFontScaleMenu);
    ShowModConfigMenu(&showModConfigMenu);
}

namespace ModLoader
{
    PluginInterface::PluginInterface(const std::wstring& fileNameW, const std::string& fileNameA, HINSTANCE handle) : fileNameUnicode(fileNameW), fileNameANSI(fileNameA), pluginHandle(handle)
    {
        this->pluginName = fileNameA.substr(0, fileNameA.find_last_of('.')); // remove ".dll" part from filename
        this->shortName = this->pluginName;
    }

    PluginInterface::~PluginInterface()
    {
        CloseHandle(pluginHandle);
    }

    void PluginInterface::RegisterPlugin(const std::string& pluginName, const std::string& shortName, const std::vector<int>& pluginVersion, const std::string& pluginDescription)
    {
        this->pluginName = pluginName;
        this->shortName = shortName;
        if (pluginVersion.size())
            this->versionInfo = pluginVersion;

        if (pluginDescription.size())
            this->pluginDesc = pluginDescription;
    }

    void LogInterface::Log(const char* fmt, ...)
    {
        ModLoaderLog::logMutex.lock();
        char buf[512];
        va_list args;
        va_start(args, fmt);
        const size_t len = vsnprintf(buf, sizeof(buf), fmt, args);
        va_end(args);

        std::string ret;
        if (len < sizeof(buf))
            ret = { buf, len };
        else if (len < 0)
            ret = { "FORMATTING ERROR" };
        else
            ret = { "FORMATTED STRING TOO LONG" };

        ModLoaderLog::GetSingleton()->Log(*this->pluginName, ret);
        ModLoaderLog::logMutex.unlock();
    }

    void LogInterface::LogError(const char* fmt, ...)
    {
        ModLoaderLog::logMutex.lock();
        char buf[512];
        va_list args;
        va_start(args, fmt);
        const size_t len = vsnprintf(buf, sizeof(buf), fmt, args);
        va_end(args);

        std::string ret;
        if (len < sizeof(buf))
            ret = { buf, len };
        else if (len < 0)
            ret = { "FORMATTING ERROR" };
        else
            ret = { "FORMATTED STRING TOO LONG" };

        ModLoaderLog::GetSingleton()->Log(*pluginName + " ERROR", ret);
        ModLoaderLog::logMutex.unlock();
    }

    MenuInterface::~MenuInterface()
    {
        for (auto& entry : menuEntries)
            delete entry;
    }

    void MenuInterface::ClearEntries()
    {
        for (auto& entry : menuEntries)
            delete entry;

        menuEntries.clear();
        drawCallEntries.clear();
    }

    ImGuiContext* MenuInterface::GetContext() const
    {
        return ImGui::GetCurrentContext();
    }

    void MenuInterface::GetAllocators(ImGuiMemAllocFunc* allocFunc, ImGuiMemFreeFunc* freeFunc, void** userData) const
    {
        ImGui::GetAllocatorFunctions(allocFunc, freeFunc, userData);
    }

    bool MenuInterface::HasMenuEntries()
    {
        return menuEntries.size();
    }

    bool MenuInterface::HasDefaultCallbacks()
    {
        bool ret = false;
        for (auto& entry : menuEntries)
            ret |= entry->defaultFunction != nullptr;

        return ret;
    }

    bool MenuInterface::HasSaveCallbacks()
    {
        bool ret = false;
        for (auto& entry : menuEntries)
            ret |= entry->saveFunction != nullptr;

        return ret;
    }

    void MenuInterface::DefaultAll()
    {
        for (auto& entry : menuEntries) {
            if (entry->defaultFunction)
                entry->defaultFunction();
        }
    }

    void MenuInterface::SaveAll()
    {
        for (auto& entry : menuEntries) {
            if (entry->saveFunction)
                entry->saveFunction();
        }
    }

    void MenuInterface::RegisterMenuEntry(std::function<void()> drawFunction, std::function<void()> defaultFunction, std::function<void()> saveFunction, std::string menuName)
    {
        size_t menuCount = this->menuEntries.size();
        std::string baseName = menuCount == 0 ? "Settings" : "Extra Settings";
        if (menuCount > 1)
            baseName.append(" " + std::to_string(menuCount));

        menuEntries.push_back(new MenuEntry(drawFunction, defaultFunction, saveFunction, menuName.empty() ? baseName : menuName));
    }

    void MenuInterface::RegisterDrawCallEntry(std::function<void()> drawFunction)
    {
        drawCallEntries.push_back(drawFunction);
    }

    void MenuInterface::DisplayNotice(const char* noticeText)
    {
        Interface::DisplayNotice(noticeText);
    }

    bool MenuInterface::IsMenuOpen() const
    {
        return Interface::IsModMenuOpen();
    }

    bool MenuInterface::IsDebugActive() const
    {
        return Settings::debugState;
    }

    void MenuInterface::SetGamePaused(bool state)
    {
        Interface::SetGamePaused(state);
    }

    bool MenuInterface::IsGamePaused() const
    {
        return Interface::IsGamePaused();
    }
}