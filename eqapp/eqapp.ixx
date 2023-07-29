module;

#include "common.h"
#include "utility.h"

export module eqapp;

export import eq;

export import eqapp_constants;
export import eqapp_log;
export import eqapp_detours;
export import eqapp_console;
export import eqapp_boxchatclient;
export import eqapp_follow;
export import eqapp_windowtitle;
export import eqapp_interpretcommand;
export import eqapp_macromanager;

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

    void Load();
    void Unload();

    void SetModule(HMODULE hmodule);

private:

    const std::string m_className = "Application";

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

void Application::Load()
{
    g_Log.deleteFileContents();
    g_Log.open();

    g_Log.write("{} loaded!    Build: {} {}\n", eqapp::Constants::ApplicationName, __TIME__, __DATE__);

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

    g_Console.Load();
    g_InterpretCommand.Load();
    g_MacroManager.Load();
    g_WindowTitle.Load();
    g_BoxChatClient.Load();
    g_Follow.Load();

    // Box Chat
    if (EQ_IsInGame() == true)
    {
        if (g_BoxChatClient.IsLoaded() == true)
        {
            if (g_BoxChatClient.IsEnabled() == true)
            {
                if (g_BoxChatClient.IsServerRunning() == true)
                {
                    g_BoxChatClient.ConnectAsPlayerSpawnName();
                }
            }
        }
    }

    std::print(std::cout, "********** Loaded! **********\n");
    g_Console.Print();

    m_isLoaded = true;
}

void Application::Unload()
{
    g_Follow.Unload();
    g_BoxChatClient.Unload();
    g_WindowTitle.Unload();
    g_MacroManager.Unload();
    g_InterpretCommand.Unload();

    std::print(std::cout, "********** Unloaded! **********\n");
    g_Console.Print();

    g_Console.Unload();

    EQAPP_Detours_Unload();

    g_Log.write("{} unloaded!    Build: {} {}\n", eqapp::Constants::ApplicationName, __TIME__, __DATE__);

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
