module;

#include "common.h"
#include "utility.h"

export module eqapp_esp;

export import eq;
export import eq_functions;

export import eqapp_log;
export import eqapp_timer;

export
{

namespace eqapp
{

class ESP
{

public:

    ESP();
    ~ESP();

    static ESP& GetInstance()
    {
        static ESP instance;
        return instance;
    }

private:

    ESP(const ESP&) = delete;
    ESP(ESP&&) = delete;
    ESP& operator=(const ESP&) = delete;
    ESP& operator=(ESP&&) = delete;

public:

    void Load();
    void Unload();
    bool IsEnabled();
    bool IsLoaded();
    void Toggle();
    void Enable();
    void Disable();
    void Execute();
    bool HandleInterpetCommand(const std::string& commandText);

private:

    const std::string m_className = "ESP";

    bool m_isEnabled = true;
    bool m_isLoaded = false;

};

ESP::ESP()
{
    //
}

ESP::~ESP()
{
    //
}

void ESP::Load()
{
    //

    m_isLoaded = true;
}

void ESP::Unload()
{
    //

    m_isLoaded = false;
}

bool ESP::IsEnabled()
{
    return m_isEnabled;
}

bool ESP::IsLoaded()
{
    return m_isLoaded;
}

void ESP::Toggle()
{
    util::ToggleBool(m_isEnabled);
    std::print(std::cout, "{}: {}\n", m_className, m_isEnabled);
}

void ESP::Enable()
{
    if (m_isEnabled == false)
    {
        Toggle();
    }
}

void ESP::Disable()
{
    if (m_isEnabled == true)
    {
        Toggle();
    }
}

void ESP::Execute()
{
    uintptr_t playerSpawn = EQ_GetPlayerSpawn();

    float playerSpawnY = EQ_GetSpawnY(playerSpawn);
    float playerSpawnX = EQ_GetSpawnX(playerSpawn);
    float playerSpawnZ = EQ_GetSpawnZ(playerSpawn);

    float screenX = -1.0f;
    float screenY = -1.0f;
    bool result = EQ_WorldLocationToScreenLocation(playerSpawnY, playerSpawnX, playerSpawnZ, screenX, screenY);
    if (result == true)
    {
        EQ_DrawText("+", (uint32_t)screenX, (uint32_t)screenY);
    }
}

bool ESP::HandleInterpetCommand(const std::string& commandText)
{
    if (commandText == "//ESPToggle")
    {
        Toggle();
        return true;
    }

    if (commandText == "//ESPEnable" || commandText == "//ESPOn")
    {
        Enable();
        return true;
    }

    if (commandText == "//ESPDisable" || commandText == "//ESPOff")
    {
        Disable();
        return true;
    }

    return false;
}

}

inline eqapp::ESP& g_ESP = eqapp::ESP::GetInstance();

}
