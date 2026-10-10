#pragma once
#include "framework.h"

namespace DXGI
{
	bool HookDXGI();
	void UnhookDXGI();

	LONG WINAPI PresentDetour(IDXGISwapChain* swapChain, unsigned syncInterval, unsigned flags);
	HRESULT WINAPI ResizeBuffersDetour(IDXGISwapChain* swapChain, UINT bufferCount, UINT width, UINT height, DXGI_FORMAT newFormat, UINT swapChainFlags);
}
