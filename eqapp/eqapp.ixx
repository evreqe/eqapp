module;

#include "common.h"

export module eqapp;

export import eq;
export import eqapp_detours;

export import eqapp_log;

export
{

namespace eqapp
{

class Application
{

public:

    Application();
    ~Application();

    static Application& GetInstance()
    {
        static Application instance;
        return instance;
    }

private:

    Application(const Application&) = delete;
    Application(Application&&) = delete;
    Application& operator=(const Application&) = delete;
    Application& operator=(Application&&) = delete;

public:

    void EnableDebugPrivileges();

    void Load();
    void Unload();

    void SetModule(HMODULE hmodule);

private:

    const std::string m_name = "EQ Application";

    bool m_isLoaded = false;

    HMODULE m_module = NULL;

};

Application::Application()
{
    //
}

Application::~Application()
{
    //
}

void Application::EnableDebugPrivileges()
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

void Application::Load()
{
    g_Log.deleteFileContents();
    g_Log.open();

    g_Log.write("{} loaded!    Build: {} {}\n", m_name, __TIME__, __DATE__);

    char* versionDateAsString = eq::Memory::ReadString(eq::EQGame::Addresses::VersionDate);
    char* versionTimeAsString = eq::Memory::ReadString(eq::EQGame::Addresses::VersionTime);

    g_Log.write("Version Date: {}\n", versionDateAsString);
    g_Log.write("Version Time: {}\n", versionTimeAsString);

    g_Log.write("DrawNetStatus: {:#x}\n", eq::EQGame::Addresses::Functions::DrawNetStatus);
    g_Log.write("ExecuteCommand: {:#x}\n", eq::EQGame::Addresses::Functions::ExecuteCommand);

    g_Log.write("EQ_GetCEverQuest(): {:#x}\n", EQ_GetCEverQuest());
    g_Log.write("EQ_GetCDisplay(): {:#x}\n", EQ_GetCDisplay());
    g_Log.write("EQ_GetChatManager(): {:#x}\n", EQ_GetChatManager());

    g_Log.write("m_module: {:#x}\n", (uint64_t)m_module);

    g_Log.write("GetModuleHandle(eqapp_dll): {:#x}\n", (uint64_t)GetModuleHandleA("eqapp_dll.dll"));

    g_Log.write("EQ_GetPlayerSpawn(): {:#x}\n", EQ_GetPlayerSpawn());

    g_Log.write("EQ_GetSpawnByName(Evre): {:#x}\n", EQ_GetSpawnByName("Evre"));

    EQAPP_Detours_Load();

    m_isLoaded = true;
}

void Application::Unload()
{
    EQAPP_Detours_Unload();

    g_Log.write("{} unloaded!    Build: {} {}\n", m_name, __TIME__, __DATE__);

    g_Log.close();

    m_isLoaded = false;
}

void Application::SetModule(HMODULE hmodule)
{
    m_module = hmodule;
}

}

inline eqapp::Application& g_Application = eqapp::Application::GetInstance();

}
