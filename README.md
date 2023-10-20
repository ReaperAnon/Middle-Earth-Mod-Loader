# Middle Earth Mod Loader

![image](https://github.com/ReaperAnon/Middle-Earth-Mod-Loader/assets/63963239/a80a0282-d000-448b-9874-ef0ef98141b9)

### Consider donating if you enjoy my work:
[![ko-fi](https://ko-fi.com/img/githubbutton_sm.svg)](https://ko-fi.com/A0A6P3CRK)

# Details
A DLL loader for Shadow of War with a small and simple plugin API to allow for easily adding in-game menus and logging capabilities to plugins.  

## Requirements
Requires the latest Microsoft Visual C++ Redistributables found [**here**](https://aka.ms/vs/17/release/vc_redist.x64.exe).

## Installation
First, navigate to your game's installation folder located at **"...\steamapps\common\ShadowOfWar\x64"** and rename the **bink2w64.dll** file to **bink2w64_.dll**, so basically add an underscore to the name.

Download the mod loader archive from the **Releases** section [**here**](https://github.com/ReaperAnon/Middle-Earth-Mod-Loader/releases/tag/loader) and place the contents of the archive (download [7zip](https://www.7-zip.org/a/7z2301-x64.exe) to open the archive if you have no archiving application) into the **"x64"** folder where the **"ShadowOfWar.exe"** is, in your game's installation folder.

Create a **"plugins"** folder in the same directory, that is where the plugins you want to load need to go to.

After installation, you should have a new **"bink2w64.dll"** file, a **"modloader"** folder and a **"plugins"** folder.

## Known Issues
Changing any of the settings in the Options menu of the game leads to a crash while the mod menu is loaded. Simply disabling the hook didn't fix it and I don't really have the time or the will anymore to debug and fix it.

As long as you have your UI, Sound and Graphics settings set up the way you want them to, this shouldn't really pose much of an issue. If you want to change them, just swap the names of the DLL files, change the options you want and then swap them back again.

## For Users
Simply download any mods you want and install them according to their instructions. All options for mods (if they have any) and for the mod loader itself can be modified in-game.

The default hotkey to open the **Mod Configuration Menu** is **F10**.

The default hotkey to open the **Log Menu** is **Tilde (`)**.

## For Modders
You can find a sample project you can clone including the mod loader API [here](https://github.com/ReaperAnon/Shadow-of-War-Sample-Project). It demonstrates how to add a plugin and register a mod menu and a custom menu for it. Also comes with some skeleton code where you can add your own functionality.

If you just want to grab the header file to include in a project you can grab it from under the releases section [here](https://github.com/ReaperAnon/Middle-Earth-Mod-Loader/releases/tag/api).

Using the plugin API is not a strict requirement, as it includes some legacy compatibility for any plugins that were made with the old version.

### Using the API
There are 4 functions to interface with the mod loader, each is prototyped in the **modloaderapi.h** header file. They are as follows:

```cpp
#define MODLOADERAPI extern "C" void __declspec(dllexport)

MODLOADERAPI OnRegisterLogAPI(ModLoader::LogAPI*) // first function, called when the logging API is registered for the plugin
MODLOADERAPI OnRegisterMenuAPI(ModLoader::MenuAPI*) // second function, called when the menu API is registered for the plugin
MODLOADERAPI OnPluginLoad(ModLoader::PluginAPI*) // third function, called when the plugin is fully loaded
MODLOADERAPI OnPluginUnload() // called when the plugin is unloaded, like when it gets disabled in-game
```

By default **ImGui is a dependency if using the API**, but it can be disabled by defining the **"MODLOADER_NO_MENU"** key before including **modloaderapi.h** like such:
```cpp
#define MODLOADER_NO_MENU
#include <modloaderapi.h>
```
If this key is defined, then the **MenuAPI** class is also not available for use.

From then on you can do whatever you like and use the objects passed through the arguments to call any of the functions from the header file. They are all commented and fairly straightforward, so you should be able to understand what they all mean.

You can register some basic information about your mod via the PluginAPI* object in OnPluginLoad as follows:
```cpp
api->RegisterPlugin("Display Name", "Log Name", { 1/*major version*/, 0/*minor version*/, 0/*patch version*/ }, "Plugin description goes here.");
```
Even plugins that don't use ImGui or register menus are visible in the mod configuration menu in a separate category so you can see they are loaded.

If you don't register your plugin (or leave certain fields empty) then the display and log name will simply be the file name, the version will be 1.0.0 and the description will be a basic description saying the menu entry is for X mod and whether it comes with options or not.

You can also initialize the log and the menu API singletons through the LogAPI* and MenuAPI* objects by calling
```cpp
ModLoader::LogAPI::Init(api);
```
and
```cpp
ModLoader::MenuAPI::Init(api);
```

Once they are initialized you can invoke them anywhere inside your code by grabbing the singleton instances of each in the following way:
```cpp
ModLoader::LogAPI::GetSingleton()->Log("I am a log entry. This statement is %s.", true ? "true" : "false");

void ShowSettings() {
// I do stuff to display settings in-game.
}

void Defaults() {
// I reset all the mod's settings to their defaults.
}

void Save() {
// I save the mod's settings to file.
}

ModLoader::MenuAPI::GetSingleton()->RegisterMenuEntry(ShowSettings, Defaults, Save, "Settings Tab Name"); 
```
Any ImGui directives inside the ShowSettings function will begin rendering from the following highlighted area:
![image](https://github.com/ReaperAnon/Middle-Earth-Mod-Loader/assets/63963239/30e3a527-2f95-464f-b486-88132ef357c2)

You can register multiple menu entries in the mod configuration menu, which will all become individual tabs inside your plugin's menu entry. You can leave the fourth argument of **RegisterMenuEntry** empty, in which case the tab names will be **Settings**, **Extra Settings**, **Extra Settings 2** and so on by default.


### Not using the API
You can avoid using the API and including the header file by simply including the following function in your code:
```cpp
extern "C" void __declspec(dllexport) OnLoad() {}
```
It will be called just like the others but without any of the functionality that the others bring.

## Credits
[**Dear ImGui**](https://github.com/ocornut/imgui)

[**SimpleIni**](https://github.com/brofield/simpleini)

[**MinHook**](https://github.com/TsudaKageyu/minhook)

[**Astoria Font**](https://fonts.adobe.com/fonts/astoria)

[**Inconsolata Font**](https://fonts.google.com/specimen/Inconsolata)
