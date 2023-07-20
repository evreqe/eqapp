#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <windows.h>
#include <tlhelp32.h>
#include <fcntl.h>
#include <io.h>

#include <psapi.h>
#pragma comment(lib, "psapi.lib")

const char* APPLICATION_NAME = "EQ Title";

const char* GAME_PROCESS_NAME = "eqgame.exe";

HWND g_hwnd = NULL;

// eqgame.exe+BF7490
#define EQ_ADDRESS_PLAYER_NAME 0xBF7490

void enable_debug_privileges()
{
    HANDLE token;

    if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &token))
    {
        TOKEN_PRIVILEGES tp;
        TOKEN_PRIVILEGES tp_previous;

        DWORD cb_previous = sizeof(TOKEN_PRIVILEGES);

        LUID luid;

        if (LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &luid))
        {
            tp.PrivilegeCount           = 1;
            tp.Privileges[0].Luid       = luid;
            tp.Privileges[0].Attributes = 0;

            AdjustTokenPrivileges
            (
                token,
                FALSE,
                &tp,
                sizeof(TOKEN_PRIVILEGES),
                &tp_previous,
                &cb_previous
            );

            tp_previous.PrivilegeCount            = 1;
            tp_previous.Privileges[0].Luid        = luid;
            tp_previous.Privileges[0].Attributes |= (SE_PRIVILEGE_ENABLED);
    
            AdjustTokenPrivileges
            (
                token,
                FALSE,
                &tp_previous,
                cb_previous,
                NULL,
                NULL
            );
        }
    }

    CloseHandle(token);
}

DWORD get_module_base_address(DWORD process_id, const wchar_t* module_name)
{
    DWORD base_address = 0;

    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, process_id);

    if (snapshot != INVALID_HANDLE_VALUE)
    {
        MODULEENTRY32 module_entry;
        module_entry.dwSize = sizeof(MODULEENTRY32);

        if (Module32First(snapshot, &module_entry))
        {
            do
            {
                if (wcscmp(module_entry.szModule, module_name) == 0)
                {
                    base_address = (DWORD)module_entry.modBaseAddr;
                    break;
                }
            } while (Module32Next(snapshot, &module_entry));
        }

        CloseHandle(snapshot);
    }

    return base_address;
}

DWORD_PTR get_base_address_by_process_handle(HANDLE process_handle)
{
    DWORD_PTR base_address = 0;

    HMODULE* module_array;
    LPBYTE module_array_bytes;

    DWORD bytes_required;

    if (process_handle)
    {
        if (EnumProcessModules(process_handle, NULL, 0, &bytes_required))
        {
            if (bytes_required)
            {
                module_array_bytes = (LPBYTE)LocalAlloc(LPTR, bytes_required);

                if (module_array_bytes)
                {
                    unsigned int module_count;

                    module_count = bytes_required / sizeof(HMODULE);
                    module_array = (HMODULE*)module_array_bytes;

                    if (EnumProcessModules(process_handle, module_array, bytes_required, &bytes_required))
                    {
                        base_address = (DWORD_PTR)module_array[0];
                    }

                    LocalFree(module_array_bytes);
                }
            }
        }
    }

    return base_address;
}

BOOL CALLBACK enum_windows_proc(HWND hwnd, LPARAM lparam)
{
    DWORD process_id;
    GetWindowThreadProcessId(hwnd, &process_id);

    if (process_id == lparam)
    {
        g_hwnd = hwnd;

        return FALSE;
    }

    return TRUE;
}

int main(int argc, char *argv[])
{
    printf("%s\n", APPLICATION_NAME);

    printf("Build: %s %s\n", __DATE__, __TIME__);

    printf("Enabling debug privileges.\n");

    enable_debug_privileges();

    DWORD processes[4096];
    DWORD needed;
    if (EnumProcesses(processes, sizeof(processes), &needed) == 0)
    {
        printf("ERROR: EnumProcesses() failed.\n");
        return 0;
    }

    if (needed == 0)
    {
        printf("ERROR: needed is zero.\n");
        return 0;
    }

    DWORD num_processes = needed / sizeof(DWORD);

    size_t i = 0;
    for (i = 0; i < num_processes; i++)
    {
        if (processes[i] == 0)
        {
            printf("INFO: processes is zero at index %d\n", i);
            continue;
        }

        HANDLE process_handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processes[i]);
        if (process_handle == NULL)
        {
            printf("INFO: process_handle is NULL at index %d\n", i);
            continue;
        }

        HMODULE module_handle;
        DWORD needed2;

        if (EnumProcessModules(process_handle, &module_handle, sizeof(module_handle), &needed2))
        {
            char module_name[1024];
            GetModuleBaseNameA(process_handle, module_handle, module_name, sizeof(module_name));

            printf("INFO: module_name at index %d: %s\n", i, module_name);

            if (strcmp(module_name, GAME_PROCESS_NAME) == 0)
            {
                printf("INFO: module_name == eqgame.exe\n");

                DWORD_PTR base_address = get_base_address_by_process_handle(process_handle);

                printf("INFO: base_address: 0x%I64X\n", base_address);

                g_hwnd = NULL;

                EnumWindows(enum_windows_proc, processes[i]);

                if (g_hwnd != NULL)
                {
                    char player_name[32];
                    ReadProcessMemory(process_handle, (LPCVOID)(base_address + EQ_ADDRESS_PLAYER_NAME), player_name, sizeof(player_name), NULL);

                    printf("INFO: player_name: %s\n", player_name);

                    char window_title[1024];
                    sprintf_s(window_title, sizeof(window_title), "EQ: %s", player_name);

                    SetWindowTextA(g_hwnd, window_title);
                }
            }
        }

        CloseHandle(process_handle);
    }

    printf("Done.\n");

    ////MessageBoxA(NULL, "Done!", "Inject DLL", MB_ICONINFORMATION);

    return EXIT_SUCCESS;
}

