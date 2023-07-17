#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <windows.h>
#include <tlhelp32.h>
#include <fcntl.h>
#include <io.h>

#include <psapi.h>
#pragma comment(lib, "psapi.lib")

const char* g_ApplicationName = "EQ Application Eject DLL";

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

                bool isDLLInjected = false;

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

                        //std::printf("moduleNameEx: %s\n", moduleNameEx);

                        if (strcmp(moduleNameEx, g_DLLName) == 0)
                        {
                            std::printf("DLL found in the game (Name: %s, ID: %d | 0x%I64X)\n", moduleNameEx, (int)processes[i], (long long)processes[i]);

                            isDLLInjected = true;

                            break;
                        }
                    }
                }

                if (isDLLInjected == true)
                {
                    std::printf("Game process found (Name: %s, ID: %d | 0x%I64X)\nEjecting DLL...\n", moduleName, (int)processes[i], (long long)processes[i]);

                    char DLLFullPathName[MAX_PATH] = {0};
                    GetFullPathNameA(g_DLLName, MAX_PATH, DLLFullPathName, NULL);

                    std::printf("DLL Full Path Name: %s\n", DLLFullPathName);

                    std::size_t DLLFullPathNameSize = std::strlen(DLLFullPathName);

                    HMODULE moduleApp = LoadLibraryA(g_DLLName);
                    if (moduleApp != NULL)
                    {
                        HANDLE remoteThread2 = CreateRemoteThread(processHandle, NULL, 0, (LPTHREAD_START_ROUTINE)GetProcAddress(moduleApp, "Eject"), NULL, 0, NULL);
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

    return EXIT_SUCCESS;
}
