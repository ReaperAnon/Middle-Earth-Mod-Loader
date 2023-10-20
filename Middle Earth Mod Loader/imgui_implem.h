#pragma once

#include "framework.h"

namespace ImGuiImplem
{
	extern ImFont* Astoria;
	extern ImFont* Inconsolata;
	extern ImFont* InconsolataBold;

	BOOL CALLBACK WorkerProc(HWND hwnd, LPARAM lParam);

	LRESULT WINAPI WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	void ImGuiStyleSetup();

	void InitImGuiWin32();

	void InitImGuiDX11(ID3D11Device* D3D11Device, ID3D11DeviceContext* D3D11DeviceContext);

	void UnInitImGuiWin32();

	void UnInitImGuiDX11();

	void ImGuiPresent(ID3D11DeviceContext* D3D11DeviceContext, ID3D11RenderTargetView* D3D11RenderTargetView);
}