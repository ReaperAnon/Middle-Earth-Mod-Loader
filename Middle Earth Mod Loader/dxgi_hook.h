#pragma once

#include "framework.h"

namespace DXGI
{
	LONG WINAPI PresentDetour(IDXGISwapChain* swapChain, unsigned syncInterval, unsigned flags);

	bool HookDXGI();

	void ToggleHook();

	void UnhookDXGI();
}