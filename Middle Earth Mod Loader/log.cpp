#include "log.h"
#include "imgui_implem.h"
#include "settings.h"
#include <cstring>

ModLoaderLog::ModLoaderLog()
{
    textBuffer = "";

    FILETIME min;
    int fileCount = 0;
    WIN32_FIND_DATA findData;
    std::wstring foundFileName;
    const std::wstring logPath = L".\\modloader\\logs\\";
    HANDLE foundHandle = FindFirstFile((logPath + L"*.log").data(), &findData);
    if (foundHandle != INVALID_HANDLE_VALUE) {
        min = findData.ftLastWriteTime;
        foundFileName = findData.cFileName;
        do
        {
            if ((findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0)
                continue;

            if (fileCount >= Settings::logLimit) {
                DeleteFile((logPath + findData.cFileName).data());
                continue;
            }

            ++fileCount;
            if (CompareFileTime(&findData.ftLastWriteTime, &min) < 0) {
                min = findData.ftLastWriteTime;
                foundFileName = findData.cFileName;
            }
        } while (FindNextFile(foundHandle, &findData));
    }
    FindClose(foundHandle);

    using namespace std::chrono;
    auto const now = system_clock::now();
    auto const time = current_zone()->to_local(now);
    auto const sec = duration_cast<seconds>(now - time_point_cast<minutes>(now)).count();
    std::wstring fileName = std::format(L"{0:%Y-%m-%d %I-%M-}{1:0>2}{2:%p}", time, sec, time) + L".log";
    if (fileCount < Settings::logLimit) {
        logHandle = CreateFile((logPath + fileName).data(), GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    } else {
        MoveFile((logPath + foundFileName).data(), (logPath + fileName).data());
        logHandle = CreateFile((logPath + fileName).data(), GENERIC_WRITE, FILE_SHARE_READ, NULL, TRUNCATE_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    }
}

ModLoaderLog::~ModLoaderLog()
{
    CloseHandle(logHandle);
}

void ModLoaderLog::Clear()
{
    textBuffer.clear();
    entryBorders.clear();
}

void ModLoaderLog::Log(std::string_view source, std::string_view message)
{
    using namespace std::chrono;
    auto const now = system_clock::now();
    auto const time = current_zone()->to_local(now);
    auto const ms = duration_cast<milliseconds>(now - time_point_cast<seconds>(now)).count();

    auto iter = textBuffer.size();
    textBuffer.append(std::format("{0:%Y/%m/%d %X}.{1:0>3} ", time, ms));
    entryBorders.push_back(textBuffer.size());

    textBuffer.append(std::format("[{0}]", source));
    entryBorders.push_back(textBuffer.size());

    textBuffer.append(std::format(" {0}\n", message));
    entryBorders.push_back(textBuffer.size());

    if(logHandle != INVALID_HANDLE_VALUE)
        WriteFile(logHandle, &*(textBuffer.begin() + iter), textBuffer.size() - iter, nullptr, NULL);
}

void ModLoaderLog::Show(const char* title, bool* showMenu)
{
    if (!ImGui::Begin(title, showMenu))
    {
        ImGui::End();
        return;
    }

    static bool showTimestamps = true;
    auto context = ImGui::GetCurrentContext();
    ImGui::Checkbox("Show Timestamps", &showTimestamps);
    bool clear = ImGui::Button("Clear");
    ImGui::SameLine();
    bool copy = ImGui::Button("Copy");
    ImGui::SameLine();
    textFilter.Draw("Filter", -100.0f);
    ImGuiID inputFieldID = context->LastItemData.ID;
    ImGui::Separator();

    if (ImGui::BeginChild("scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar))
    {
        if (clear)
            Clear();
        if (copy)
            ImGui::LogToClipboard();
        
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
        ImGui::PushFont(ImGuiImplem::Inconsolata);

        auto const base = textBuffer.begin();
        if (textFilter.IsActive())
        {
            // store the filter output possibly
            for (int lineNr = 0; lineNr < entryBorders.size() / 3; lineNr++)
            {
                auto const timestampStart = lineNr > 0 ? base + entryBorders[lineNr * 3 - 1] : base;
                auto const timestampEnd = base + entryBorders[lineNr * 3];
                auto const sourceStart = base + entryBorders[lineNr * 3];
                auto const sourceEnd = base + entryBorders[lineNr * 3 + 1];
                auto const entryStart = base + entryBorders[lineNr * 3 + 1];
                auto const entryEnd = base + entryBorders[lineNr * 3 + 2];
                if (textFilter.PassFilter(&*sourceStart, &*entryEnd))
                {
                    if (showTimestamps) {
                        ImGui::PushStyleColor(ImGuiCol_Text, context->Style.Colors[ImGuiCol_TextDisabled]);
                        ImGui::TextUnformatted(&*timestampStart, &*timestampEnd);
                        ImGui::PopStyleColor();
                        ImGui::SameLine();
                    }

                    ImGui::PushFont(ImGuiImplem::InconsolataBold);
                    ImGui::TextUnformatted(&*sourceStart, &*sourceEnd);
                    ImGui::SameLine();
                    ImGui::TextUnformatted(":");
                    ImGui::PopFont();

                    ImGui::SameLine();
                    ImGui::TextUnformatted(&*entryStart, &*entryEnd);
                }
            }
        }
        else
        {
            ImGuiListClipper clipper;
            clipper.Begin(entryBorders.size() / 3);
            while (clipper.Step())
            {
                for (int lineNr = clipper.DisplayStart; lineNr < clipper.DisplayEnd; lineNr++)
                {
                    auto const timestampStart = lineNr > 0 ? base + entryBorders[lineNr * 3 - 1] : base;
                    auto const timestampEnd = base + entryBorders[lineNr * 3];
                    auto const sourceStart = base + entryBorders[lineNr * 3];
                    auto const sourceEnd = base + entryBorders[lineNr * 3 + 1];
                    auto const entryStart = base + entryBorders[lineNr * 3 + 1];
                    auto const entryEnd = base + entryBorders[lineNr * 3 + 2];

                    if (showTimestamps) {
                        ImGui::PushStyleColor(ImGuiCol_Text, context->Style.Colors[ImGuiCol_TextDisabled]);
                        ImGui::TextUnformatted(&*timestampStart, &*timestampEnd);
                        ImGui::PopStyleColor();
                        ImGui::SameLine();
                    }

                    ImGui::PushFont(ImGuiImplem::InconsolataBold);
                    ImGui::TextUnformatted(&*sourceStart, &*sourceEnd);
                    if (ImGui::IsMouseHoveringRect(context->LastItemData.Rect.Min, context->LastItemData.Rect.Max, true)) {
                        ImGui::GetForegroundDrawList(context->CurrentWindow)->AddRect(context->LastItemData.Rect.Min, context->LastItemData.Rect.Max, IM_COL32(255, 255, 255, 255), 1, 0, 2.f);
                        if (ImGui::IsMouseClicked(ImGuiMouseButton_Left, false)) {
                            ImGui::ActivateItemByID(inputFieldID);
                        }
                        if (ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
                            for (auto iter = sourceStart; iter < sourceEnd; iter++)
                                context->IO.AddInputCharacter(*iter);
                        }
                    }

                    ImGui::SameLine();
                    ImGui::TextUnformatted(":");
                    ImGui::PopFont();

                    ImGui::SameLine();
                    ImGui::TextUnformatted(&*entryStart, &*entryEnd);
                }
            }
            clipper.End();
        }

        ImGui::PopFont();
        ImGui::PopStyleVar();

        if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
            ImGui::SetScrollHereY(1.0f);
    }
    ImGui::EndChild();
    ImGui::End();
}