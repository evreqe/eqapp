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
    std::string GetDisplayText();
    void Execute();
    bool HandleInterpetCommand(const std::string& commandText);

private:

    const std::string m_className = "ESP";

    bool m_isEnabled = true;
    bool m_isLoaded = false;

    float m_distance = 512.0f;

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

std::string ESP::GetDisplayText()
{
    std::string displayText;

    auto displayTextBackInserter = std::back_inserter(displayText);

    if (IsEnabled() == true)
    {
        std::format_to(displayTextBackInserter, "{} enabled\n", m_className);
    }
    else
    {
        std::format_to(displayTextBackInserter, "{} disabled\n", m_className);
    }

    return displayText;
}

void ESP::Execute()
{
    uintptr_t playerSpawn = EQ_GetPlayerSpawn();
    if (playerSpawn == NULL)
    {
        return;
    }

    eq::Location playerSpawnLocation = EQ_GetSpawnLocation(playerSpawn);

    eq::ScreenCoordinates playerSpawnScreenCoordinates;
    bool isPlayerOnScreen = EQ_GetScreenCoordinatesByLocation(playerSpawnLocation, playerSpawnScreenCoordinates);
    if (isPlayerOnScreen == true)
    {
        EQ_DrawText("+", playerSpawnScreenCoordinates.X, playerSpawnScreenCoordinates.Y);
    }

/*
    uintptr_t targetSpawn = EQ_GetTargetSpawn();
    if (targetSpawn != NULL)
    {
        eq::Location targetSpawnLocation = EQ_GetSpawnLocation(targetSpawn);

        EQ_DrawLine3D(playerSpawnLocation, targetSpawnLocation, eq::ColorARGB::Gray);
    }
*/

    eq::SpawnList spawnList = EQ_GetSpawnList();

    for (auto& spawn : spawnList)
    {
        if (spawn == playerSpawn)
        {
            continue;
        }

        uint8_t spawnType = EQ_GetSpawnType(spawn);
        if (spawnType == eq::Constants::Spawn::Type::Player)
        {
            continue;
        }

        float spawnDistance = EQ_GetSpawnDistance3D(spawn);
        if (spawnDistance > m_distance)
        {
            continue;
        }

        std::string spawnNameNumbered = EQ_GetSpawnNameNumbered(spawn);
        if (spawnNameNumbered.empty() == true)
        {
            continue;
        }

        if (spawnNameNumbered.contains("_familiar") == true)
        {
            continue;
        }

        if (spawnNameNumbered.contains("_Mount") == true)
        {
            continue;
        }

        std::string spawnLastName = EQ_GetSpawnLastName(spawn);
        if (spawnNameNumbered.empty() == false)
        {
            if (spawnLastName.ends_with(" Pet") == true)
            {
                continue;
            }

            if (spawnLastName.ends_with(" Mercenary") == true)
            {
                continue;
            }
        }

        eq::Location spawnLocation = EQ_GetSpawnLocation(spawn);

        eq::ScreenCoordinates spawnScreenCoordinates;
        bool spawnIsOnScreen = EQ_GetScreenCoordinatesByLocation(spawnLocation, spawnScreenCoordinates);
        if (spawnIsOnScreen == true)
        {
            std::string spawnText = std::format("+ {}", spawnNameNumbered);

            EQ_DrawText(spawnText, spawnScreenCoordinates.X, spawnScreenCoordinates.Y);
        }
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
