#include "input.h"
#include "wndproc.h"

// input_processor.h
bool InputProcessor<ImGuiKey>::ShouldProcessInputs()
{
    if (GetForegroundWindow() != WndProc::PROCHWND)
        return false;

    return true;
}

bool InputProcessor<ImGuiKey>::IsActionPressed(GameKey<ImGuiKey>* gameKey)
{
    if (gameKey->SecondKey != ImGuiKey_None)
        return ImGui::IsKeyDown(gameKey->FirstKey) && ImGui::IsKeyDown(gameKey->SecondKey);

    return ImGui::IsKeyDown(gameKey->FirstKey);
}

bool InputProcessor<ImGuiKey>::IsModifierPressed(GameKey<ImGuiKey>* gameKey)
{
    return ImGui::IsKeyDown(gameKey->ModKey);
}

bool InputProcessor<ImGuiKey>::IsActionBlocked(GameKey<ImGuiKey>* gameKey)
{
    if (gameKey->SecondKey != ImGuiKey_None)
        return KeyBlocks[gameKey->FirstKey - 512] && KeyBlocks[gameKey->SecondKey - 512];

    return KeyBlocks[gameKey->FirstKey - 512];
}

void InputProcessor<ImGuiKey>::SetActionBlock(GameKey<ImGuiKey>* gameKey, bool state)
{
    if (gameKey->SecondKey != ImGuiKey_None)
        KeyBlocks[gameKey->SecondKey - 512] = state;

    KeyBlocks[gameKey->FirstKey - 512] = state;
}


// input.h
InputProcessor<ImGuiKey> Input::inputProcessor;

GameKey<ImGuiKey>* Input::GetGameKey(size_t idx)
{
    return inputProcessor.GameKeys[idx];
}

GameKey<ImGuiKey>* Input::MakeGameKey(const wchar_t* inputString)
{
    std::wstring key(inputString);
    size_t firstEnd = key.find_first_of(' ');
    size_t secondEnd = key.find_last_of(' ');

    inputProcessor.GameKeys.push_back(
        new GameKey<ImGuiKey>
        (
            (ImGuiKey)std::stoi(key.substr(0, firstEnd)),
            (ImGuiKey)std::stoi(key.substr(firstEnd + 1, secondEnd - firstEnd - 1)),
            (ImGuiKey)std::stoi(key.substr(secondEnd + 1, std::string::npos))
        ));

    return inputProcessor.GameKeys.back();
}

GameKey<ImGuiKey>* Input::MakeGameKey(ImGuiKey firstKey, ImGuiKey secondKey, ImGuiKey modKey)
{
    inputProcessor.GameKeys.push_back(
        new GameKey<ImGuiKey>
        (
            firstKey,
            secondKey,
            modKey
        ));

    return inputProcessor.GameKeys.back();
}

void Input::StartInputProcessing()
{
    inputProcessor.KeyBlocks.resize(ImGuiKey_NamedKey_COUNT);
    inputProcessor.StartInputProcessing();
}

void Input::StopInputProcessing()
{
    inputProcessor.StopInputProcessing();
}