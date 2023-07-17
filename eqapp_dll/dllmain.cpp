#include "dllmain.h"

EXTERN_DLL_EXPORT DWORD __stdcall Inject()
{
    //MessageBoxA(0, "Loading application", "Inject()", MB_ICONINFORMATION);

    util::WinAPI::EnableDebugPrivileges();

    g_Application.SetModule(g_Module);

    g_Application.Load();

    return 0;
}

EXTERN_DLL_EXPORT DWORD __stdcall Eject()
{
    //MessageBoxA(0, "Unloading application", "Eject()", MB_ICONINFORMATION);

    g_Application.Unload();

    HMODULE thisModule = GetModuleHandleA(g_DLLName);

    FreeLibraryAndExitThread(thisModule, 0);

    return 0;
}

BOOL __stdcall DllMain(HMODULE hmodule, DWORD reason, LPVOID reserved)
{
    g_Module = hmodule;

    if (reason == DLL_PROCESS_ATTACH)
    {
        //MessageBoxA(0, "reason == DLL_PROCESS_ATTACH", "DllMain()", MB_ICONINFORMATION);

        DisableThreadLibraryCalls(hmodule);
    }
    else if (reason == DLL_PROCESS_DETACH)
    {
        //MessageBoxA(0, "reason == DLL_PROCESS_DETACH", "DllMain()", MB_ICONINFORMATION);
    }

    return TRUE;
}
