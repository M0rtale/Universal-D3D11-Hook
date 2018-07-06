#pragma once
#include <d3d11.h>
#include "detours.h"

typedef HRESULT(__stdcall *tD3D11Present)(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);

namespace Hook
{
	//Helpers
	extern ID3D11Device *pDevice;
	extern ID3D11DeviceContext *pContext;
	extern IDXGISwapChain* pSwapChain;
	extern ID3D11RenderTargetView* pRenderTargetView;

	//original Function
	extern tD3D11Present oPresent;

	//Function Wrapper
	HRESULT D3D11Present(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);

	//Detour
	void HookFunction(PVOID *oFunction, PVOID pDetour);
	void UnhookFunction(PVOID *oFunction, PVOID pDetour);
}