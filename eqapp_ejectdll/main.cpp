#include "common.h"
#include "utility.h"

import eq_constants;
import eqapp_constants;

const char* g_ApplicationName = "EQ Application Eject DLL";

const char* g_DLLFileName = eqapp::Constants::DLLFileName.c_str();

const char* g_GameProcessName = eq::Constants::GameProcessName.c_str();

int main(int argc, char *argv[])
{
    std::print("{}\n", g_ApplicationName);

    std::print("Build: {} {}\n", __DATE__, __TIME__);

    std::print("Enabling debug privileges\n");

    util::WinAPI::EnableDebugPrivileges();

    DWORD processes[4096];
    DWORD needed;
    if (EnumProcesses(processes, sizeof(processes), &needed) == 0)
    {
        std::print("ERROR: EnumProcesses() failed\n");
        return EXIT_FAILURE;
    }

    if (needed == 0)
    {
        std::print("ERROR: needed is zero\n");
        return EXIT_FAILURE;
    }

    DWORD numProcesses = needed / sizeof(DWORD);

    DWORD i = 0;
    for (i = 0; i < numProcesses; i++)
    {
        if (processes[i] == 0)
        {
            std::print("INFO: processes[i] is zero at index: {}\n", i);
            continue;
        }

        HANDLE processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processes[i]);
        if (processHandle == NULL)
        {
            std::print("INFO: processHandle is NULL at index: {}\n", i);
            continue;
        }

        HMODULE moduleHandle;
        DWORD needed2;
        if (EnumProcessModules(processHandle, &moduleHandle, sizeof(moduleHandle), &needed2))
        {
            char moduleName[1024] = {0};
            GetModuleBaseNameA(processHandle, moduleHandle, moduleName, sizeof(moduleName));

            std::print("{}: {}\n", i, moduleName);

            if (std::strcmp(moduleName, g_GameProcessName) == 0)
            {
                std::print("INFO: moduleName == '{}'\n", g_GameProcessName);

                DWORD_PTR baseAddress = util::WinAPI::GetBaseAddressByProcessHandle(processHandle);

                std::print("INFO: baseAddress: {}\n", baseAddress);

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

                        //std::print("moduleNameEx: {}\n", moduleNameEx);

                        if (std::strcmp(moduleNameEx, g_DLLFileName) == 0)
                        {
                            std::print("DLL found in the game process (Name: {}, ID: {})\n", moduleNameEx, processes[i]);

                            isDLLInjected = true;

                            break;
                        }
                    }
                }

                if (isDLLInjected == true)
                {
                    std::print("Game process found (Name: {}, ID: {})\nEjecting DLL...\n", moduleName, processes[i]);

                    char DLLFullPathName[MAX_PATH] = {0};
                    GetFullPathNameA(g_DLLFileName, MAX_PATH, DLLFullPathName, NULL);

                    std::print("DLL Full Path Name: {}\n", DLLFullPathName);

                    std::size_t DLLFullPathNameSize = std::strlen(DLLFullPathName);

                    HMODULE moduleApp = LoadLibraryA(g_DLLFileName);
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

    std::print("Done\n");

    system("pause");

    return EXIT_SUCCESS;
}
