#include "dxgi_hook.h"
#include "imgui_implem.h"

bool IsInitialized = false;
ID3D11Device* D3D11Device = nullptr;
ID3D11DeviceContext* D3D11DeviceContext = nullptr;
ID3D11RenderTargetView* D3D11RenderTargetView = nullptr;

typedef DWORD(WINAPI* tPresent)(IDXGISwapChain* swapChain, unsigned syncInterval, unsigned flags);
typedef HRESULT(WINAPI* tResizeBuffers)(IDXGISwapChain* swapChain, UINT bufferCount, UINT width, UINT height, DXGI_FORMAT newFormat, UINT swapChainFlags);

tPresent Present = nullptr;
tPresent PresentOriginal = nullptr;
tResizeBuffers ResizeBuffers = nullptr;
tResizeBuffers ResizeBuffersOriginal = nullptr;

static void ReleaseRenderTarget()
{
	if (D3D11RenderTargetView)
	{
		D3D11RenderTargetView->Release();
		D3D11RenderTargetView = nullptr;
	}
}

static bool CreateRenderTarget(IDXGISwapChain* swapChain)
{
	ID3D11Texture2D* backBuffer = nullptr;
	if (FAILED(swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer)) || backBuffer == nullptr)
		return false;

	D3D11_TEXTURE2D_DESC desc{};
	backBuffer->GetDesc(&desc);
	if (desc.Width == 0 || desc.Height == 0)
	{
		backBuffer->Release();
		return false;
	}

	const HRESULT hr = D3D11Device->CreateRenderTargetView(backBuffer, nullptr, &D3D11RenderTargetView);
	backBuffer->Release();
	return SUCCEEDED(hr) && D3D11RenderTargetView != nullptr;
}

LONG WINAPI DXGI::PresentDetour(IDXGISwapChain* swapChain, unsigned syncInterval, unsigned flags)
{
	if (!IsInitialized)
	{
		if (SUCCEEDED(swapChain->GetDevice(__uuidof(ID3D11Device), (LPVOID*)&D3D11Device)))
		{
			D3D11Device->GetImmediateContext(&D3D11DeviceContext);
			if (CreateRenderTarget(swapChain))
			{
				ImGuiImplem::InitImGuiDX11(D3D11Device, D3D11DeviceContext);
				IsInitialized = true;
			}
		}
		return PresentOriginal(swapChain, syncInterval, flags);
	}

	if (D3D11RenderTargetView == nullptr)
	{
		CreateRenderTarget(swapChain);
		if (D3D11RenderTargetView == nullptr)
			return PresentOriginal(swapChain, syncInterval, flags);
	}

	ImGuiImplem::ImGuiPresent(D3D11DeviceContext, D3D11RenderTargetView);
	return PresentOriginal(swapChain, syncInterval, flags);
}

HRESULT WINAPI DXGI::ResizeBuffersDetour(IDXGISwapChain* swapChain, UINT bufferCount, UINT width, UINT height, DXGI_FORMAT newFormat, UINT swapChainFlags)
{
	ReleaseRenderTarget();
	const HRESULT hr = ResizeBuffersOriginal(swapChain, bufferCount, width, height, newFormat, swapChainFlags);
	if (SUCCEEDED(hr))
		CreateRenderTarget(swapChain);
	return hr;
}

bool DXGI::HookDXGI()
{
	ImGuiImplem::InitImGuiWin32();

	WNDCLASSEX windowClass;
	ZeroMemory(&windowClass, sizeof(windowClass));
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.lpszClassName = L"TempWndClass";
	windowClass.hInstance = GetModuleHandle(NULL);
	windowClass.style = CS_HREDRAW | CS_VREDRAW;
	windowClass.lpfnWndProc = DefWindowProc;
	RegisterClassEx(&windowClass);

	HWND tempWindow = CreateWindow(windowClass.lpszClassName, L"TempDXWindow", WS_OVERLAPPEDWINDOW, 0, 0, 100, 100, NULL, NULL, windowClass.hInstance, NULL);

	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = tempWindow;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.Windowed = TRUE;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	const D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_1 };

	bool result = false;
	IDXGISwapChain* swapChainTemp = nullptr;
	ID3D11Device* D3D11DeviceTemp = nullptr;
	if (SUCCEEDED(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, featureLevels, 2, D3D11_SDK_VERSION, &swapChainDesc, &swapChainTemp, &D3D11DeviceTemp, nullptr, nullptr)))
	{
		void** vTable = *reinterpret_cast<void***>(swapChainTemp);
		Present = (tPresent)vTable[8];
		ResizeBuffers = (tResizeBuffers)vTable[13];
		swapChainTemp->Release();
		D3D11DeviceTemp->Release();

		result = MH_CreateHook(Present, PresentDetour, (LPVOID*)&PresentOriginal) == MH_OK
			&& MH_CreateHook(ResizeBuffers, ResizeBuffersDetour, (LPVOID*)&ResizeBuffersOriginal) == MH_OK
			&& MH_EnableHook(Present) == MH_OK
			&& MH_EnableHook(ResizeBuffers) == MH_OK;
	}

	DestroyWindow(tempWindow);
	UnregisterClass(windowClass.lpszClassName, windowClass.hInstance);

	return result;
}

void DXGI::UnhookDXGI()
{
	MH_DisableHook(Present);
	MH_DisableHook(ResizeBuffers);
	ReleaseRenderTarget();

	ImGuiImplem::UnInitImGuiDX11();
	ImGuiImplem::UnInitImGuiWin32();
	D3D11DeviceContext = nullptr;
	D3D11Device = nullptr;
}
