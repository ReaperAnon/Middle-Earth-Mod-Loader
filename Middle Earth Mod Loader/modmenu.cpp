#include "modmenu.h"
#include "interface.h"
#include "settings.h"
#include "input.h"
#include "dllmain.h"

const char* loOptions[3] = { "Default", "Alphabetical", "Inverse Alphabetical" };

void ModMenu::ShowSettings()
{
    ImGui::Checkbox("Show All Mods", &Settings::showAllMods);
    if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal)) {
        ImGui::BeginTooltip();
        ImGui::SetTooltip("Whether to show all mods, regardless of them having settings or not.");
        ImGui::EndTooltip();
    }
    ImGui::Spacing();
    ImGui::Checkbox("Save On Default", &Settings::saveOnDefault);
    if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal)) {
        ImGui::BeginTooltip();
        ImGui::SetTooltip("Save settings automatically after restoring them to defaults.");
        ImGui::EndTooltip();
    }
    ImGui::Spacing();
    ImGui::Checkbox("Autosave Settings", &Settings::saveWhenClosing);
    if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal)) {
        ImGui::BeginTooltip();
        ImGui::SetTooltip("Have all registered settings be saved automatically when the mod configuration menu is closed.");
        ImGui::EndTooltip();
    }
    ImGui::Spacing();
    ImGui::Checkbox("Debug Mode", &Settings::debugState);
    if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal)) {
        ImGui::BeginTooltip();
        ImGui::SetTooltip("Turns debug mode on, along with any features that depend on it. Functionality depends on its implementation by mods.");
        ImGui::EndTooltip();
    }
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();
    ImGui::Hotkey("Toggle Config Menu", &Input::GetGameKey(0)->FirstKey, &Input::GetGameKey(0)->SecondKey);
    if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal)) {
        ImGui::BeginTooltip();
        ImGui::SetTooltip("Opens and closes the mod configuration menu.");
        ImGui::EndTooltip();
    }
    ImGui::Spacing();
    ImGui::Hotkey("Toggle Log", &Input::GetGameKey(1)->FirstKey, &Input::GetGameKey(1)->SecondKey);
    if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal)) {
        ImGui::BeginTooltip();
        ImGui::SetTooltip("Opens and closes the in-game log.");
        ImGui::EndTooltip();
    }
    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();
    ImGui::Text("Maximum Log File Count");
    if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal)) {
        ImGui::BeginTooltip();
        ImGui::SetTooltip("The maximum number of logs that can be kept in the mod loader's \"logs\" folder.");
        ImGui::EndTooltip();
    }
    ImGui::SliderInt("##numLogs", &Settings::logLimit, 1, 50, "%d", ImGuiSliderFlags_AlwaysClamp);
    if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal)) {
        ImGui::BeginTooltip();
        ImGui::SetTooltip("The maximum number of logs that can be kept in the mod loader's \"logs\" folder.");
        ImGui::EndTooltip();
    }
}

void ShowPlugins()
{
    for (int i = 1; i < pluginEntries.size(); i++) {
        if (ImGui::Checkbox(pluginEntries[i]->fileNameANSI.c_str(), &pluginEntries[i]->isEnabled)) {
            if (pluginEntries[i]->isEnabled)
                LoadPlugin(pluginEntries[i]);
            else
                DisablePlugin(pluginEntries[i]);
        }
    }
}

void ModMenu::RegisterModLoaderMenu()
{
    ModLoader::PluginInterface* pluginInterface = new ModLoader::PluginInterface(L"", "", nullptr);

    pluginEntries.push_back(pluginInterface);
    pluginInterface->RegisterPlugin("Middle Earth Mod Loader", "ModLoader", {1, 0, 0}, "Mod configuration menu for the mod loader.\nUsed for customizing the functionality of the mod configuration menu and to change the plugin load order.");
    pluginInterface->menuInterface.RegisterMenuEntry(ShowSettings, Settings::SetDefaultSettings, Settings::SaveSettings);
    pluginInterface->menuInterface.RegisterMenuEntry(ShowPlugins, nullptr, Settings::SavePlugins, "Plugins");
}