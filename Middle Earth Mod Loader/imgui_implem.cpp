#include "imgui_implem.h"
#include "interface.h"
#include "wndproc.h"

HWND WndProc::PROCHWND;
WNDPROC WndProc::OLDWNDPROC;

ImFont* ImGuiImplem::Astoria;
ImFont* ImGuiImplem::Inconsolata;
ImFont* ImGuiImplem::InconsolataBold;


BOOL CALLBACK ImGuiImplem::WorkerProc(HWND hwnd, LPARAM lParam)
{
	DWORD processID;
	GetWindowThreadProcessId(hwnd, &processID);
	if (processID == GetCurrentProcessId())
	{
		WndProc::PROCHWND = hwnd;
		return FALSE;
	}

	return TRUE;
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT WINAPI ImGuiImplem::WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;

	return CallWindowProc(WndProc::OLDWNDPROC, hWnd, uMsg, wParam, lParam);
}

void ImGuiImplem::ImGuiStyleSetup()
{
	ImGui::StyleColorsDark();

	ImGuiStyle& style = ImGui::GetStyle();
	style.WindowRounding = 2;
	style.ChildRounding = 1;
	style.FrameRounding = 3;
	style.PopupRounding = 3;
	style.FramePadding = ImVec2(12, 3);
	style.ItemInnerSpacing = ImVec2(9, 3);
	style.WindowMenuButtonPosition = ImGuiDir_None;

	ImVec4* colors = ImGui::GetStyle().Colors;
	colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
	colors[ImGuiCol_WindowBg] = ImVec4(0.05f, 0.05f, 0.05f, 1.00f);
	colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_PopupBg] = ImVec4(0.05f, 0.05f, 0.05f, 0.93f);
	colors[ImGuiCol_Border] = ImVec4(0.19f, 0.19f, 0.19f, 0.29f);
	colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.24f);
	colors[ImGuiCol_FrameBg] = ImVec4(0.23f, 0.23f, 0.23f, 0.54f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.19f, 0.19f, 0.19f, 0.54f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
	colors[ImGuiCol_TitleBg] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.40f, 0.40f, 0.40f, 0.54f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
	colors[ImGuiCol_CheckMark] = ImVec4(228.0f / 255.0f, 132.0f / 255.0f, 4.0f / 255.0f, 1.0f);
	colors[ImGuiCol_SliderGrab] = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
	colors[ImGuiCol_Button] = ImVec4(0.23f, 0.23f, 0.23f, 0.54f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.40f, 0.40f, 0.40f, 0.54f);
	colors[ImGuiCol_ButtonActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
	colors[ImGuiCol_Header] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.25f, 0.25f, 0.25f, 0.33f);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.20f, 0.22f, 0.23f, 0.33f);
	colors[ImGuiCol_Separator] = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
	colors[ImGuiCol_SeparatorHovered] = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
	colors[ImGuiCol_SeparatorActive] = ImVec4(0.40f, 0.44f, 0.47f, 1.00f);
	colors[ImGuiCol_ResizeGrip] = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(0.40f, 0.44f, 0.47f, 1.00f);
	colors[ImGuiCol_Tab] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
	colors[ImGuiCol_TabHovered] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
	colors[ImGuiCol_TabActive] = ImVec4(0.20f, 0.20f, 0.20f, 0.36f);
	colors[ImGuiCol_TabUnfocused] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
	colors[ImGuiCol_PlotLines] = ImVec4(228.0f / 255.0f, 132.0f / 255.0f, 4.0f / 255.0f, 1.0f);
	colors[ImGuiCol_PlotLinesHovered] = ImVec4(228.0f / 255.0f, 132.0f / 255.0f, 4.0f / 255.0f, 1.0f);
	colors[ImGuiCol_PlotHistogram] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
	colors[ImGuiCol_TableHeaderBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
	colors[ImGuiCol_TableBorderStrong] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
	colors[ImGuiCol_TableBorderLight] = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
	colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
	colors[ImGuiCol_TextSelectedBg] = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
	colors[ImGuiCol_DragDropTarget] = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
	colors[ImGuiCol_NavHighlight] = ImVec4(228.0f / 255.0f, 132.0f / 255.0f, 4.0f / 255.0f, 1.0f);
	colors[ImGuiCol_NavWindowingHighlight] = ImVec4(228.0f / 255.0f, 132.0f / 255.0f, 4.0f / 255.0f, 1.0f);
	colors[ImGuiCol_NavWindowingDimBg] = ImVec4(228.0f / 255.0f, 132.0f / 255.0f, 4.0f / 255.0f, 1.0f);
	colors[ImGuiCol_ModalWindowDimBg] = ImVec4(228.0f / 255.0f, 132.0f / 255.0f, 4.0f / 255.0f, 1.0f);

	ImGuiIO& io = ImGui::GetIO();
	io.IniFilename = ".\\modloader\\imgui.ini";
	//io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;
	io.ConfigFlags &= ~ImGuiConfigFlags_NavEnableGamepad;
	io.ConfigFlags &= ~ImGuiConfigFlags_NavEnableKeyboard;
	Astoria = io.Fonts->AddFontFromFileTTF(".\\modloader\\font\\AstoriaRoman.ttf", 16);
	Inconsolata = io.Fonts->AddFontFromFileTTF(".\\modloader\\font\\Inconsolata-Regular.ttf", 16);
	InconsolataBold = io.Fonts->AddFontFromFileTTF(".\\modloader\\font\\Inconsolata-Medium.ttf", 16);
}

void ImGuiImplem::InitImGuiWin32()
{
	EnumWindows(WorkerProc, 0);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplWin32_Init(WndProc::PROCHWND);

	WndProc::OLDWNDPROC = (WNDPROC)SetWindowLongPtrW(WndProc::PROCHWND, GWLP_WNDPROC, (LONG_PTR)WndProc);
}

void ImGuiImplem::InitImGuiDX11(ID3D11Device* D3D11Device, ID3D11DeviceContext* D3D11DeviceContext)
{
	ImGuiStyleSetup();
	ImGui_ImplDX11_Init(D3D11Device, D3D11DeviceContext);
}

void ImGuiImplem::UnInitImGuiWin32()
{
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
	SetWindowLongPtr(WndProc::PROCHWND, GWLP_WNDPROC, (LONG_PTR)WndProc::OLDWNDPROC);
}

void ImGuiImplem::UnInitImGuiDX11()
{
	ImGui_ImplDX11_Shutdown();
}

void ImGuiImplem::ImGuiPresent(ID3D11DeviceContext* D3D11DeviceContext, ID3D11RenderTargetView* D3D11RenderTargetView)
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	Interface::ShowRegisteredModMenus();
	Interface::ShowRegisteredDrawCalls();

	ImGui::EndFrame();
	ImGui::Render();

	D3D11DeviceContext->OMSetRenderTargets(1, &D3D11RenderTargetView, NULL);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}