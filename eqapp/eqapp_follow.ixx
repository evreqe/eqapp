module;

#include "common.h"
#include "utility.h"

export module eqapp_follow;

export import eq;
export import eq_functions;

export import eqapp_log;
export import eqapp_timer;

export
{

namespace eqapp
{

class Follow
{

public:

    Follow();
    ~Follow();

    static Follow& GetInstance()
    {
        static Follow instance;
        return instance;
    }

private:

    Follow(const Follow&) = delete;
    Follow(Follow&&) = delete;
    Follow& operator=(const Follow&) = delete;
    Follow& operator=(Follow&&) = delete;

public:

    void Load();
    void Unload();
    bool IsEnabled();
    bool IsLoaded();
    bool IsFollowing();
    void Toggle();
    void Enable();
    void Disable();
    void PrintStatus();
    uintptr_t GetSpawn();
    std::string GetSpawnName();
    bool SetSpawnToTargetSpawn();
    bool SetSpawnByName(const std::string& spawnName);
    bool SetSpawnByID(const uint32_t spawnID);
    void StopFollowing();
    float GetDistance();
    void SetDistance(float distance);
    std::string GetDisplayText();
    void Execute();
    bool HandleInterpetCommand(const std::string& commandText);
    bool HandleExecuteCommand(uint32_t commandID, bool keyDown);

private:

    const std::string m_className = "Follow";

    bool m_isEnabled = true;
    bool m_isLoaded = false;

    uintptr_t m_spawn = NULL;

    float m_distance = 8.0f;

};

Follow::Follow()
{
    //
}

Follow::~Follow()
{
    //
}

void Follow::Load()
{
    m_isLoaded = true;
}

void Follow::Unload()
{
    if (IsFollowing() == true)
    {
        StopFollowing();
    }

    m_isLoaded = false;
}

bool Follow::IsEnabled()
{
    return m_isEnabled;
}

bool Follow::IsLoaded()
{
    return m_isLoaded;
}

bool Follow::IsFollowing()
{
    return m_spawn != NULL;
}

void Follow::Toggle()
{
    util::ToggleBool(m_isEnabled);
    std::print(std::cout, "{}: {}\n", m_className, m_isEnabled);

    if (m_isEnabled == false)
    {
        StopFollowing();
    }
}

void Follow::Enable()
{
    if (m_isEnabled == false)
    {
        Toggle();
    }
}

void Follow::Disable()
{
    if (m_isEnabled == true)
    {
        Toggle();
    }
}

void Follow::PrintStatus()
{
    if (IsFollowing() == false)
    {
        std::print(std::cout, "{}: You are not following anyone.\n", m_className);
    }
    else
    {
        std::string spawnName = GetSpawnName();

        std::print(std::cout, "{}: You are following {}.\n", m_className, spawnName);
    }
}

uintptr_t Follow::GetSpawn()
{
    return m_spawn;
}

std::string Follow::GetSpawnName()
{
    if (m_spawn == NULL)
    {
        return std::string();
    }

    return EQ_GetSpawnName(m_spawn);
}

bool Follow::SetSpawnToTargetSpawn()
{
    uintptr_t targetSpawn = EQ_GetTargetSpawn();
    if (targetSpawn != NULL)
    {
        m_spawn = targetSpawn;
        return true;
    }

    return false;
}

bool Follow::SetSpawnByName(const std::string& spawnName)
{
    uintptr_t spawn = EQ_GetSpawnByNameOrNameNumbered(spawnName);
    if (spawn != NULL)
    {
        m_spawn = spawn;
        return true;
    }

    return false;
}

bool Follow::SetSpawnByID(const uint32_t spawnID)
{
    uintptr_t spawn = EQ_GetSpawnByID(spawnID);
    if (spawn != NULL)
    {
        m_spawn = spawn;
        return true;
    }

    return false;
}

void Follow::StopFollowing()
{
    if (m_spawn != NULL)
    {
        std::print(std::cout, "{}: You stop following.\n", m_className);
    }

    m_spawn = NULL;
    EQ_ExecuteCommandEx(eq::Constants::ExecuteCommand::FORWARD, false);
}

float Follow::GetDistance()
{
    return m_distance;
}

void Follow::SetDistance(float distance)
{
    m_distance = distance;
}

std::string Follow::GetDisplayText()
{
    std::string displayText;

    auto displayTextBackInserter = std::back_inserter(displayText);

    if (IsEnabled() == true)
    {
        if (IsFollowing() == true)
        {
            std::format_to(displayTextBackInserter, "{}: {}\n", m_className, GetSpawnName());
        }
    }
    else
    {
        std::format_to(displayTextBackInserter, "{} disabled\n", m_className);
    }

    return displayText;
}

void Follow::Execute()
{
    EQ_TurnPlayerTowardsSpawn(m_spawn);

    float spawnDistance = EQ_GetSpawnDistance2D(m_spawn);
    if (spawnDistance <= m_distance)
    {
        EQ_ExecuteCommandEx(eq::Constants::ExecuteCommand::FORWARD, false);
        return;
    }

    EQ_ExecuteCommandEx(eq::Constants::ExecuteCommand::FORWARD, true);
}

bool Follow::HandleInterpetCommand(const std::string& commandText)
{
    if (commandText == "//FollowToggle")
    {
        Toggle();
        return true;
    }

    if (commandText == "//FollowEnable" || commandText == "//FollowOn")
    {
        Enable();
        return true;
    }

    if (commandText == "//FollowDisable" || commandText == "//FollowOff")
    {
        Disable();
        return true;
    }

    if (commandText == "//FollowStatus")
    {
        PrintStatus();
        return true;
    }

    if (commandText == "//Follow")
    {
        if (SetSpawnToTargetSpawn() == true)
        {
            PrintStatus();
        }
        return true;
    }

    if (commandText.starts_with("//Follow ") == true)
    {
        std::string arg0;
        auto result = scn::scan(commandText, "{}", arg0);
        if (result)
        {
            std::string remainder = result.range_as_string();
            util::String::TrimSpacesOnLeftAndRight(remainder);
            if (remainder.size() != 0)
            {
                if (SetSpawnByName(remainder) == true)
                {
                    PrintStatus();
                }
            }
        }

        return true;
    }

    if (commandText.starts_with("//FollowID ") == true)
    {
        std::string arg0;
        uint32_t arg1;
        auto result = scn::scan(commandText, "{} {}", arg0, arg1);
        if (result)
        {
            if (SetSpawnByID(arg1) == true)
            {
                PrintStatus();
            }
        }

        return true;
    }

    if (commandText == "//StopFollow" || commandText == "//Stop")
    {
        StopFollowing();
        return true;
    }

    if (commandText.starts_with("//FollowDistance ") == true)
    {
        std::string arg0;
        float arg1;
        auto result = scn::scan(commandText, "{} {}", arg0, arg1);
        if (result)
        {
            SetDistance(arg1);
        }

        return true;
    }

    return false;
}

bool Follow::HandleExecuteCommand(uint32_t commandID, bool keyDown)
{
    if
    (
        commandID == eq::Constants::ExecuteCommand::LEFT  ||
        commandID == eq::Constants::ExecuteCommand::RIGHT ||
        commandID == eq::Constants::ExecuteCommand::BACK
    )
    {
        StopFollowing();
        return true;
    }

    return false;
}

}

inline eqapp::Follow& g_Follow = eqapp::Follow::GetInstance();

}
