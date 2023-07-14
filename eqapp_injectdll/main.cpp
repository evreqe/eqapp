#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <windows.h>
#include <tlhelp32.h>
#include <fcntl.h>
#include <io.h>

#include <psapi.h>
#pragma comment(lib, "psapi.lib")

const char* g_ApplicationName = "EQ Application Inject DLL";

const char* g_DLLName = "eqapp_dll.dll";

const char* g_GameProcessName = "eqgame.exe";

HWND g_WindowHandle = NULL;

void EQAPP_EnableDebugPrivileges()
{
    HANDLE token;

    if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &token))
    {
        TOKEN_PRIVILEGES tokenPrivileges;
        TOKEN_PRIVILEGES tokenPrivilegesPrevious;

        DWORD tokenPrivilegesSize = sizeof(TOKEN_PRIVILEGES);

        LUID locallyUniqueIdentifier;

        if (LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &locallyUniqueIdentifier))
        {
            tokenPrivileges.PrivilegeCount           = 1;
            tokenPrivileges.Privileges[0].Luid       = locallyUniqueIdentifier;
            tokenPrivileges.Privileges[0].Attributes = 0;

            AdjustTokenPrivileges
            (
                token,
                FALSE,
                &tokenPrivileges,
                sizeof(TOKEN_PRIVILEGES),
                &tokenPrivilegesPrevious,
                &tokenPrivilegesSize
            );

            tokenPrivilegesPrevious.PrivilegeCount            = 1;
            tokenPrivilegesPrevious.Privileges[0].Luid        = locallyUniqueIdentifier;
            tokenPrivilegesPrevious.Privileges[0].Attributes |= (SE_PRIVILEGE_ENABLED);
    
            AdjustTokenPrivileges
            (
                token,
                FALSE,
                &tokenPrivilegesPrevious,
                tokenPrivilegesSize,
                NULL,
                NULL
            );
        }
    }

    CloseHandle(token);
}

DWORD_PTR EQAPP_GetModuleBaseAddress(DWORD processID, const wchar_t* moduleName)
{
    DWORD_PTR baseAddress = NULL;

    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, processID);

    if (snapshot == INVALID_HANDLE_VALUE)
    {
        return baseAddress;
    }

    MODULEENTRY32 module_entry;
    module_entry.dwSize = sizeof(MODULEENTRY32);

    if (Module32First(snapshot, &module_entry))
    {
        do
        {
            if (wcscmp(module_entry.szModule, moduleName) == 0)
            {
                baseAddress = (DWORD_PTR)module_entry.modBaseAddr;
                break;
            }
        } while (Module32Next(snapshot, &module_entry));
    }

    CloseHandle(snapshot);

    return baseAddress;
}

DWORD_PTR EQAPP_GetBaseAddressByProcessHandle(HANDLE processHandle)
{
    DWORD_PTR baseAddress = NULL;

    if (processHandle == NULL)
    {
        return baseAddress;
    }

    DWORD bytesRequired = 0;

    if (EnumProcessModules(processHandle, NULL, 0, &bytesRequired))
    {
        if (bytesRequired)
        {
            LPBYTE moduleArrayBytes = (LPBYTE)LocalAlloc(LPTR, bytesRequired);

            if (moduleArrayBytes)
            {
                DWORD moduleCount = bytesRequired / sizeof(HMODULE);
                HMODULE* moduleArray = (HMODULE*)moduleArrayBytes;

                if (EnumProcessModules(processHandle, moduleArray, bytesRequired, &bytesRequired))
                {
                    baseAddress = (DWORD_PTR)moduleArray[0];
                }

                LocalFree(moduleArrayBytes);
            }
        }
    }

    return baseAddress;
}

BOOL CALLBACK EQAPP_EnumWindowsProc(HWND hwnd, LPARAM lparam)
{
    DWORD processID;
    GetWindowThreadProcessId(hwnd, &processID);

    if (processID == lparam)
    {
        g_WindowHandle = hwnd;

        return FALSE;
    }

    return TRUE;
}

FARPROC EQAPP_GetRelativeEntryAddress(LPCSTR pcszLibrary, const char* szEntryFunction)
{
    if (GetFileAttributesA(pcszLibrary) == INVALID_FILE_ATTRIBUTES)
    {
        return NULL;
    }

    HINSTANCE hLibrary = LoadLibraryA(pcszLibrary);
    if (hLibrary == NULL)
    {
        return NULL;
    }
 
    FARPROC pFunction = GetProcAddress(hLibrary, szEntryFunction);
    if (pFunction == NULL)
    {
        return NULL;
    }

    return (FARPROC)((DWORD_PTR)pFunction - (DWORD_PTR)hLibrary);
}

LPVOID EQAPP_GetAbsoluteAddress(DWORD_PTR dwBaseAddress, LPVOID pFunction)
{
    return (LPVOID)((DWORD_PTR)pFunction + dwBaseAddress);
}

bool EQAPP_CallRemoteFunction(HANDLE hProcess, LPVOID pFunction)
{
    DWORD dwExitCode;
 
    HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)pFunction, NULL, 0, NULL);
    if (hThread == NULL)
    {
        return false;
    }

    if (WaitForSingleObject(hThread, 20000) != WAIT_OBJECT_0)
    {
        return false;
    }

    if (GetExitCodeThread(hThread, &dwExitCode) == FALSE)
    {
        return false;
    }

    return (dwExitCode != 0);
}

int main(int argc, char *argv[])
{
    std::printf("%s\n", g_ApplicationName);

    std::printf("Build: %s %s\n", __DATE__, __TIME__);

    std::printf("Enabling debug privileges\n");

    EQAPP_EnableDebugPrivileges();

    DWORD processes[4096];
    DWORD needed;
    if (EnumProcesses(processes, sizeof(processes), &needed) == 0)
    {
        std::printf("ERROR: EnumProcesses() failed\n");
        return EXIT_FAILURE;
    }

    if (needed == 0)
    {
        printf("ERROR: needed is zero\n");
        return EXIT_FAILURE;
    }

    DWORD numProcesses = needed / sizeof(DWORD);

    DWORD i = 0;
    for (i = 0; i < numProcesses; i++)
    {
        if (processes[i] == 0)
        {
            std::printf("INFO: processes[i] is zero at index: %d\n", i);
            continue;
        }

        HANDLE processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processes[i]);
        if (processHandle == NULL)
        {
            std::printf("INFO: processHandle is NULL at index: %d\n", i);
            continue;
        }

        HMODULE moduleHandle;
        DWORD needed2;
        if (EnumProcessModules(processHandle, &moduleHandle, sizeof(moduleHandle), &needed2))
        {
            char moduleName[1024] = {0};
            GetModuleBaseNameA(processHandle, moduleHandle, moduleName, sizeof(moduleName));

            std::printf("%d: %s\n", i, moduleName);

            if (std::strcmp(moduleName, g_GameProcessName) == 0)
            {
                std::printf("INFO: moduleName == '%s'\n", g_GameProcessName);

                DWORD_PTR baseAddress = EQAPP_GetBaseAddressByProcessHandle(processHandle);

                std::printf("INFO: baseAddress: 0x%I64X\n", baseAddress);

                bool isDLLAlreadyInjected = false;

                HMODULE modules[1024];
                DWORD needed3;
                if (EnumProcessModules(processHandle, modules, sizeof(modules), &needed3))
                {
                    DWORD numModules = needed3 / sizeof(HMODULE);

                    DWORD j = 0;
                    for (j = 0; j < numModules; j++)
                    {
                        char moduleNameEx[1024] = {0};
                        GetModuleBaseNameA(processHandle, modules[j], moduleNameEx, sizeof(moduleNameEx));

                        //std::printf("Module Name Ex: %s\n", moduleNameEx);

                        if (strcmp(moduleNameEx, g_DLLName) == 0)
                        {
                            std::printf("DLL is already injected in to the game (Name: %s, ID: %d | 0x%I64X)\nSkipping...\n", moduleNameEx, (int)processes[i], (long long)processes[i]);

                            isDLLAlreadyInjected = true;

                            MessageBoxA(0, "DLL is already injected", g_ApplicationName, MB_ICONINFORMATION);

                            break;
                        }
                    }
                }

                if (isDLLAlreadyInjected == false)
                {
                    std::printf("Game process found (Name: %s, ID: %d | 0x%I64X)\nInjecting DLL...\n", moduleName, (int)processes[i], (long long)processes[i]);

                    char DLLFullPathName[MAX_PATH] = {0};
                    GetFullPathNameA(g_DLLName, MAX_PATH, DLLFullPathName, NULL);

                    std::printf("DLL Full Path Name: %s\n", DLLFullPathName);

                    std::size_t DLLFullPathNameSize = std::strlen(DLLFullPathName);

                    LPVOID remoteMemory = VirtualAllocEx(processHandle, NULL, DLLFullPathNameSize, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
                    if (remoteMemory != NULL)
                    {
                        WriteProcessMemory(processHandle, remoteMemory, DLLFullPathName, DLLFullPathNameSize, NULL);

                        HMODULE moduleKernel32 = GetModuleHandleA("Kernel32");
                        if (moduleKernel32 != NULL)
                        {
                            HANDLE remoteThread = CreateRemoteThread(processHandle, NULL, 0, (LPTHREAD_START_ROUTINE)GetProcAddress(moduleKernel32, "LoadLibraryA"), remoteMemory, 0, NULL);
                            if (remoteThread != NULL)
                            {
                                //DWORD exitCode;
                                //GetExitCodeThread(remoteThread, &exitCode);

                                WaitForSingleObject(remoteThread, INFINITE);
                                CloseHandle(remoteThread);

                                VirtualFreeEx(processHandle, remoteMemory, 0, MEM_RELEASE);

                                Sleep(100);
                            }
                        }
                    }

                    HMODULE moduleApp = LoadLibraryA(g_DLLName);
                    if (moduleApp != NULL)
                    {
                        HANDLE remoteThread2 = CreateRemoteThread(processHandle, NULL, 0, (LPTHREAD_START_ROUTINE)GetProcAddress(moduleApp, "Inject"), NULL, 0, NULL);
                        if (remoteThread2 != NULL)
                        {
                            WaitForSingleObject(remoteThread2, INFINITE);
                            CloseHandle(remoteThread2);

                            Sleep(100);
                        }
                    }
                }
            }
        }

        CloseHandle(processHandle);
    }

    std::printf("Done\n");

    system("pause");

    ////MessageBoxA(NULL, "Done!", g_ApplicationName, MB_ICONINFORMATION);

    return EXIT_SUCCESS;
}
