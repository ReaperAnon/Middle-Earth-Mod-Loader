#include "gamehook.h"

const SHORT bytePattern[] = { 0xFF, 0x15, 0x2A, 0xD7, 0x16, 0x01 };

const SHORT patternOffset = 0;

MemoryTools memTool(L"ShadowOfWar.exe");


extern "C"
{
	BYTE* activeWindowAddress = nullptr;
	BYTE* activeWindowJmpAddr = nullptr;
    void* origFuncAddr = nullptr;

	void ReplaceGetActiveWindow();
}

void GameHook::InitalizeMod()
{
    while (!FindAddresses())
        Sleep(500);

    CreateHooks();
}

bool GameHook::FindAddresses()
{
    activeWindowAddress = memTool.Scan(bytePattern, sizeof(bytePattern) / sizeof(bytePattern[0]), patternOffset);
    activeWindowJmpAddr = activeWindowAddress + 0x6;
    return activeWindowAddress;
}

void GameHook::CreateHooks()
{
    MH_CreateHook(activeWindowAddress, reinterpret_cast<void*>(ReplaceGetActiveWindow), &origFuncAddr);
    MH_EnableHook(activeWindowAddress);
}
