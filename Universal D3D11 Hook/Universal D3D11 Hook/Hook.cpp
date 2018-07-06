#include "Hook.h"

tD3D11Present Hook::oPresent = NULL;

HRESULT __stdcall Hook::D3D11Present(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{
	//this is called every frame.

	return Hook::oPresent(pSwapChain, SyncInterval, Flags);
}

//Detour
void Hook::HookFunction(PVOID *oFunction, PVOID pDetour)
{
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(oFunction, pDetour);
	DetourTransactionCommit();
}
void Hook::UnhookFunction(PVOID *oFunction, PVOID pDetour)
{
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourDetach(oFunction, pDetour);
	DetourTransactionCommit();
}