module;

#include "common.h"
#include "utility.h"

export module eqapp_interpretcommand;

export import eq;
export import eq_functions;

export import eqapp_log;
export import eqapp_timer;

export
{

namespace eqapp
{

class InterpretCommand
{

public:

    InterpretCommand();
    ~InterpretCommand();

    static InterpretCommand& GetInstance()
    {
        static InterpretCommand instance;
        return instance;
    }

private:

    InterpretCommand(const InterpretCommand&) = delete;
    InterpretCommand(InterpretCommand&&) = delete;
    InterpretCommand& operator=(const InterpretCommand&) = delete;
    InterpretCommand& operator=(InterpretCommand&&) = delete;

public:

    void Load();
    void Unload();
    bool IsEnabled();
    bool IsLoaded();
    void Toggle();
    void Enable();
    void Disable();
    void AddCommandTextToList(const std::string& commandText);
    void Execute();
    void ConvertCommandText(std::string& commandText);
    bool HandleInterpretCommand(const std::string& commandText);

private:

    const std::string m_className = "Interpret Command";

    bool m_isEnabled = true;
    bool m_isLoaded = false;

    std::list<std::string> m_commandTextList;
    eqapp::Timer m_commandTextListTimer;

};

InterpretCommand::InterpretCommand()
{
    //
}

InterpretCommand::~InterpretCommand()
{
    //
}

void InterpretCommand::Load()
{
    m_commandTextList.clear();

    m_isLoaded = true;
}

void InterpretCommand::Unload()
{
    m_commandTextList.clear();

    m_isLoaded = false;
}

bool InterpretCommand::IsEnabled()
{
    return m_isEnabled;
}

bool InterpretCommand::IsLoaded()
{
    return m_isLoaded;
}

void InterpretCommand::Toggle()
{
    util::ToggleBool(m_isEnabled);
    std::print(std::cout, "{}: {}\n", m_className, m_isEnabled);
}

void InterpretCommand::Enable()
{
    if (m_isEnabled == false)
    {
        Toggle();
    }
}

void InterpretCommand::Disable()
{
    if (m_isEnabled == true)
    {
        Toggle();
    }
}

void InterpretCommand::AddCommandTextToList(const std::string& commandText)
{
    m_commandTextList.push_back(commandText);
}

void InterpretCommand::Execute()
{
    if (m_commandTextList.size() == 0)
    {
        return;
    }

    std::string commandText = m_commandTextList.front();
    if (commandText.size() == 0)
    {
        m_commandTextList.pop_front();
        return;
    }

    if (commandText.starts_with("/") == false)
    {
        m_commandTextList.pop_front();
        return;
    }

    bool bUseTimer = false;

    bool bHasTimeElapsed = false;

    bool bUsePause = false;

    eqapp::Timer::TimeInterval timeInterval = 1;

    if (commandText.starts_with("//Pause ") == true)
    {
        bUsePause = true;

        std::string arg0;
        eqapp::Timer::TimeInterval arg1;
        auto result = scn::scan(commandText, "{} {}", arg0, arg1);
        if (result)
        {
            timeInterval = arg1;

            bUseTimer = true;
        }
    }

    if (m_commandTextListTimer.HasTimeElapsedInSeconds(timeInterval) == true)
    {
        bHasTimeElapsed = true;

        m_commandTextListTimer.Restart();
    }

    if (bUseTimer == true && bHasTimeElapsed == false)
    {
        return;
    }

    m_commandTextList.pop_front();

    if (bUsePause == true)
    {
        return;
    }

    EQ_InterpretCommand(commandText);
}

void InterpretCommand::ConvertCommandText(std::string& commandText)
{
    if (commandText.contains("{PlayerID}") == true)
    {
        uint32_t playerSpawnID = EQ_GetPlayerSpawnID();
        if (playerSpawnID != 0xFFFFFFFF)
        {
            std::string playerSpawnIDAsString = std::to_string(playerSpawnID);
            if (playerSpawnIDAsString.size() != 0)
            {
                util::String::ReplaceAll(commandText, "{PlayerID}", playerSpawnIDAsString);
            }
        }
    }

    if (commandText.contains("{PlayerName}") == true)
    {
        std::string playerSpawnName = EQ_GetPlayerSpawnName();
        if (playerSpawnName.size() != 0)
        {
            util::String::ReplaceAll(commandText, "{PlayerName}", playerSpawnName);
        }
    }

    if (commandText.contains("{TargetID}") == true)
    {
        uint32_t targetSpawnID = EQ_GetTargetSpawnID();
        if (targetSpawnID != 0xFFFFFFFF)
        {
            std::string targetSpawnIDAsString = std::to_string(targetSpawnID);
            if (targetSpawnIDAsString.size() != 0)
            {
                util::String::ReplaceAll(commandText, "{TargetID}", targetSpawnIDAsString);
            }
        }
    }

    if (commandText.contains("{TargetName}") == true)
    {
        std::string targetSpawnName = EQ_GetTargetSpawnName();
        if (targetSpawnName.size() != 0)
        {
            util::String::ReplaceAll(commandText, "{TargetName}", targetSpawnName);
        }
    }
}

bool InterpretCommand::HandleInterpretCommand(const std::string& commandText)
{
    if (commandText == "//InterpretCommandToggle")
    {
        Toggle();
        return true;
    }

    if (commandText == "//InterpretCommandEnable")
    {
        Enable();
        return true;
    }

    if (commandText == "//InterpretCommandDisable")
    {
        Disable();
        return true;
    }

    if (commandText.starts_with("//Test ") == true)
    {
        std::string arg0;
        std::string arg1;
        int arg2;
        auto result = scn::scan(commandText, "{} {} {}", arg0, arg1, arg2);
        if (result)
        {
            std::string remainder = result.range_as_string();

            std::print(std::cout, "arg0: {}\n", arg0);
            std::print(std::cout, "arg1: {}\n", arg1);
            std::print(std::cout, "arg2: {}\n", arg2);
            std::print(std::cout, "remainder: {}\n", remainder);
        }

        return true;
    }

    if (commandText == "//AutoRun")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::AUTORUN);
        return true;
    }

    if (commandText == "//Beep")
    {
        util::WinAPI::Beep();
        return true;
    }

    if (commandText == "//BeepWarning")
    {
        util::WinAPI::BeepEx(MB_ICONWARNING);
        return true;
    }

    if (commandText == "//BeepError")
    {
        util::WinAPI::BeepEx(MB_ICONERROR);
        return true;
    }

    if (commandText == "//Jump")
    {
        EQ_ExecuteCommandEx(eq::Constants::ExecuteCommand::JUMP, true);
        EQ_ExecuteCommandEx(eq::Constants::ExecuteCommand::JUMP, false);
        return true;
    }

    if (commandText == "//Sit")
    {
        AddCommandTextToList("/sit on");
        return true;
    }

    if (commandText == "//Stand")
    {
        AddCommandTextToList("/sit off");
        return true;
    }

    if (commandText == "//SpawnList")
    {
        std::vector<uintptr_t> spawnList = EQ_GetSpawnList();

        std::print(std::cout, "Spawn List size: {}\n", spawnList.size());

        return true;
    }

    return false;
}

}

inline eqapp::InterpretCommand& g_InterpretCommand = eqapp::InterpretCommand::GetInstance();

}
