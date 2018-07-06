#include "DllMain.h"

ID3D11Device *Hook::pDevice = NULL;
ID3D11DeviceContext *Hook::pContext = NULL;
IDXGISwapChain* Hook::pSwapChain = NULL;
ID3D11RenderTargetView* Hook::pRenderTargetView = NULL;

void Start()
{
	D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;

	HWND hWindow = FindWindowA(NULL, "Your Window"); // TODO: Modify this.

#pragma region Initialise DXGI_SWAP_CHAIN_DESC
	DXGI_SWAP_CHAIN_DESC scd;
	ZeroMemory(&scd, sizeof(scd));

	scd.BufferCount = 1;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // sets color formatting, we are using RGBA
	scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // says what we are doing with the buffer
	scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH; // msdn explains better than i can: https://msdn.microsoft.com/en-us/library/windows/desktop/bb173076(v=vs.85).aspx
	scd.OutputWindow = hWindow; // our gamewindow, obviously
	scd.SampleDesc.Count = 1; // Set to 1 to disable multisampling
	scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD; // D3D related stuff, cant really describe what it does
	scd.Windowed = ((GetWindowLongPtr(hWindow, GWL_STYLE) & WS_POPUP) != 0) ? false : true; // check if our game is windowed
	scd.BufferDesc.Width = 1920; // temporary width
	scd.BufferDesc.Height = 1080; // temporary height
	scd.BufferDesc.RefreshRate.Numerator = 144; // refreshrate in Hz
	scd.BufferDesc.RefreshRate.Denominator = 1; // no clue, lol
#pragma endregion

	if (FAILED(D3D11CreateDeviceAndSwapChain(
		NULL, D3D_DRIVER_TYPE_HARDWARE, NULL,
		NULL, &featureLevel, 1, D3D11_SDK_VERSION,
		&scd, &Hook::pSwapChain,
		&Hook::pDevice, NULL, &Hook::pContext
	)))
	{// failed to create D3D11 device
		return;
	}

	//Get VTable Pointers
	DWORD_PTR* pSwapChainVT = reinterpret_cast<DWORD_PTR*>(Hook::pSwapChain);
	DWORD_PTR* pDeviceVT = reinterpret_cast<DWORD_PTR*>(Hook::pDevice); // Device not needed, but prolly need it to draw stuff in Present, so it is included
	DWORD_PTR* pContextVT = reinterpret_cast<DWORD_PTR*>(Hook::pContext);

	//Pointer->Table
	pSwapChainVT = reinterpret_cast<DWORD_PTR*>(pSwapChainVT[0]);
	pDeviceVT = reinterpret_cast<DWORD_PTR*>(pDeviceVT[0]);
	pContextVT = reinterpret_cast<DWORD_PTR*>(pContextVT[0]);

	Hook::oPresent = reinterpret_cast<tD3D11Present>(pSwapChainVT[8]); // Present Function

	//Hook using Detour
	Hook::HookFunction(reinterpret_cast<PVOID*>(&Hook::oPresent), Hook::D3D11Present);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hModule); // PERFOMANCE???
		CreateThread(NULL, NULL, reinterpret_cast<LPTHREAD_START_ROUTINE>(Start), NULL, NULL, NULL);
		break;

	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

