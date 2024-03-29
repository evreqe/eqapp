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
    if (m_commandTextList.empty() == true)
    {
        return;
    }

    std::string commandText = m_commandTextList.front();
    if (commandText.empty() == true)
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

        if (auto result = scn::scan<std::string, eqapp::Timer::TimeInterval>(commandText, "{} {}"))
        {
            const auto& [arg0, arg1] = result->values();

            timeInterval = arg1;

            bUseTimer = true;
        }
        else
        {
            std::print(std::cout, "ERROR: {}", result.error().msg());
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
    if (commandText.contains("{") == false && commandText.contains("}") == false)
    {
        return;
    }

    if (commandText.contains("{PlayerID}") == true)
    {
        uint32_t playerSpawnID = EQ_GetPlayerSpawnID();
        if (playerSpawnID != eq::Constants::Spawn::ID::Null)
        {
            std::string playerSpawnIDAsString = std::to_string(playerSpawnID);
            if (playerSpawnIDAsString.empty() == false)
            {
                util::String::ReplaceAll(commandText, "{PlayerID}", playerSpawnIDAsString);
            }
        }
    }

    if (commandText.contains("{PlayerName}") == true)
    {
        std::string playerSpawnName = EQ_GetPlayerSpawnName();
        if (playerSpawnName.empty() == false)
        {
            util::String::ReplaceAll(commandText, "{PlayerName}", playerSpawnName);
        }
    }

    if (commandText.contains("{TargetID}") == true)
    {
        uint32_t targetSpawnID = EQ_GetTargetSpawnID();
        if (targetSpawnID != eq::Constants::Spawn::ID::Null)
        {
            std::string targetSpawnIDAsString = std::to_string(targetSpawnID);
            if (targetSpawnIDAsString.empty() == false)
            {
                util::String::ReplaceAll(commandText, "{TargetID}", targetSpawnIDAsString);
            }
        }
    }

    if (commandText.contains("{TargetName}") == true)
    {
        std::string targetSpawnName = EQ_GetTargetSpawnName();
        if (targetSpawnName.empty() == false)
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
        if (auto result = scn::scan<std::string, std::string, int, float>(commandText, "{} {} {} {}"))
        {
            const auto& [arg0, arg1, arg2, arg3] = result->values();

            std::print(std::cout, "arg0: {}\n", arg0);
            std::print(std::cout, "arg1: {}\n", arg1);
            std::print(std::cout, "arg2: {}\n", arg2);
            std::print(std::cout, "arg3: {}\n", arg3);
        }
        else
        {
            std::print(std::cout, "ERROR: {}", result.error().msg());
        }

        return true;
    }

    if (commandText.starts_with("//SplitText ") == true)
    {
        auto textList = util::String::Split(commandText, ' ');
        if (textList.empty() == false)
        {
            for (auto& text : textList)
            {
                std::print(std::cout, "Text: {}\n", text);
            }
        }

        return true;
    }

    if (commandText.starts_with("//Echo ") == true)
    {
        std::string afterText = util::String::GetAfter(commandText, " ");
        if (afterText.empty() == false)
        {
            std::print(std::cout, "Echo: {}\n", afterText);
        }

        return true;
    }

    if (commandText.starts_with("//Target ") == true)
    {
        if (auto result = scn::scan<std::string>(commandText, "{}"))
        {
            auto remainder = std::string_view{result->range()};

            if (remainder.size() != 0)
            {
                uintptr_t spawn = EQ_GetSpawnByNameOrNameNumbered(remainder.data());
                if (spawn != NULL)
                {
                    EQ_SetTargetSpawn(spawn);
                }
            }
        }
        else
        {
            std::print(std::cout, "ERROR: {}", result.error().msg());
        }

        return true;
    }

    if (commandText.starts_with("//TargetID ") == true)
    {
        if (auto result = scn::scan<std::string, uint32_t>(commandText, "{} {}"))
        {
            const auto& [arg0, arg1] = result->values();

            EQ_SetTargetSpawnByID(arg1);
        }
        else
        {
            std::print(std::cout, "ERROR: {}", result.error().msg());
        }

        return true;
    }

    if (commandText.starts_with("//Multiline ") == true)
    {
        if (commandText.contains(";") == true)
        {
            std::string afterText = util::String::GetAfter(commandText, " ");
            if (afterText.empty() == false)
            {
                std::vector<std::string> commandTextList_ = util::String::Split(afterText, ';');

                for (auto& commandText_ : commandTextList_)
                {
                    AddCommandTextToList(commandText_);
                }
            }
        }

        return true;
    }

    if (commandText == "//SpawnList")
    {
        eq::SpawnList spawnList = EQ_GetSpawnList();

        std::print(std::cout, "Spawn List size: {}\n", spawnList.size());

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

    if (commandText == "//CastRayToTarget")
    {
        bool result = EQ_CastRayToTarget();
        std::print(std::cout, "CastRayToTarget(): {}\n", result);
        return true;
    }

    if (commandText == "//CastRay")
    {
        uintptr_t playerSpawn = EQ_GetPlayerSpawn();
        uintptr_t targetSpawn = EQ_GetTargetSpawn();

        if (playerSpawn != NULL && targetSpawn != NULL)
        {
            eq::Location playerSpawnLocation = EQ_GetSpawnLocation(playerSpawn);
            eq::Location targetSpawnLocation = EQ_GetSpawnLocation(targetSpawn);

            bool result = EQ_CastRay(playerSpawnLocation, targetSpawnLocation);
            std::print(std::cout, "CastRay(): {}\n", result);
        }

        return true;
    }

    if (commandText.starts_with("//PlaySound ") == true)
    {
        std::string afterText = util::String::GetAfter(commandText, " ");
        if (afterText.empty() == false)
        {
            if (afterText.ends_with(".mp3") == false)
            {
                if (afterText.ends_with(".wav") == false)
                {
                    afterText.append(".wav");
                }

                EQ_PlaySound(afterText);
            }
            else
            {
                std::print(std::cout, "PlaySound: MP3 files are not supported.\n");
            }
        }

        return true;
    }

    if (commandText == "//StopSound")
    {
        EQ_StopSound();

        return true;
    }

    if (commandText == "//AcceptInvite")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::CLEAR_TARGET);
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::INVITE_FOLLOW);

        return true;
    }

    if (commandText == "//TurnLeft")
    {
        EQ_TurnLeft();

        return true;
    }

    if (commandText == "//TurnRight")
    {
        EQ_TurnRight();

        return true;
    }

    if (commandText == "//TurnAround")
    {
        EQ_TurnAround();

        return true;
    }

    if (commandText == "//FaceNorth")
    {

        EQ_SetPlayerSpawnHeadingNorth();

        return true;
    }

    if (commandText == "//FaceNorthWest")
    {

        EQ_SetPlayerSpawnHeadingNorthWest();

        return true;
    }

    if (commandText == "//FaceWest")
    {

        EQ_SetPlayerSpawnHeadingWest();

        return true;
    }

    if (commandText == "//FaceSouthWest")
    {

        EQ_SetPlayerSpawnHeadingSouthWest();

        return true;
    }

    if (commandText == "//FaceSouth")
    {

        EQ_SetPlayerSpawnHeadingSouth();

        return true;
    }

    if (commandText == "//FaceSouthEast")
    {

        EQ_SetPlayerSpawnHeadingSouthEast();

        return true;
    }

    if (commandText == "//FaceEast")
    {

        EQ_SetPlayerSpawnHeadingEast();

        return true;
    }

    if (commandText == "//FaceNorthEast")
    {

        EQ_SetPlayerSpawnHeadingNorthEast();

        return true;
    }

    if (commandText.starts_with("//Face ") == true)
    {
        std::string afterText = util::String::GetAfter(commandText, " ");
        if (afterText.empty() == false)
        {
            uintptr_t spawn = EQ_GetSpawnByNameOrNameNumbered(afterText);
            if (spawn != NULL)
            {
                EQ_TurnPlayerTowardsSpawn(spawn);
            }
        }

        return true;
    }

    if (commandText == "//FaceTarget")
    {
        EQ_TurnPlayerTowardsTarget();

        return true;
    }

    if (commandText == "//LookAtTarget")
    {
        EQ_LookPlayerAtTarget();

        return true;
    }

    if (commandText == "//StopSong" || commandText == "//Melody")
    {
        EQ_InterpretCommand("/stopsong");
        EQ_InterpretCommand("/melody");

        return true;
    }

    if (commandText.starts_with("//Melody ") == true)
    {
        std::string afterText = util::String::GetAfter(commandText, " ");
        if (afterText.empty() == false)
        {
            std::vector<std::string> tokenList = util::String::Split(afterText, ',');
            if (tokenList.empty() == false)
            {
                EQ_InterpretCommand("/stopsong");
                EQ_InterpretCommand("/melody");

                std::stringstream ss;
                ss << "/melody";

                for (auto& token : tokenList)
                {
                    if (util::String::IsDigits(token) == true)
                    {
                        ss << " " << token;
                    }
                }

                EQ_InterpretCommand(ss.str());
            }
        }

        return true;
    }

    if (commandText == "//ZoneInfo")
    {
        std::string zoneLongName = EQ_GetZoneLongName();
        std::string zoneShortName = EQ_GetZoneShortName();
        int zoneID = EQ_GetZoneID();

        std::string zoneInfoText = std::format("ZoneInfo: {} ({}) [ID: {}]", zoneLongName, zoneShortName, zoneID);

        std::print(std::cout, "{}\n", zoneInfoText);

        return true;
    }

    if (commandText == "//SetForegroundWindow")
    {
        HWND window = EQ_GetWindow();

        ShowWindow(window, SW_SHOW);
        SetForegroundWindow(window);
        SetFocus(window);

        return true;
    }

    // **************************************************
    // ExecuteCommand functions
    // **************************************************

    if (commandText == "//AutoRun")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::AUTORUN);
        return true;
    }

    if (commandText == "//Jump")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::JUMP);
        return true;
    }

    if (commandText == "//Consider")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::CONSIDER);
        return true;
    }

    if (commandText == "//Duck")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::DUCK);
        return true;
    }

    if (commandText == "//Hail")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::HAIL);
        return true;
    }

    if (commandText == "//Use")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::USE);
        return true;
    }

    if (commandText == "//CenterView")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::CENTERVIEW);
        return true;
    }

    if (commandText == "//TargetMe")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::TARGETME);
        return true;
    }

    if (commandText == "//Party1")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::PARTY1);
        return true;
    }

    if (commandText == "//Party2")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::PARTY2);
        return true;
    }

    if (commandText == "//Party3")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::PARTY3);
        return true;
    }

    if (commandText == "//Party4")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::PARTY4);
        return true;
    }

    if (commandText == "//Party5")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::PARTY5);
        return true;
    }

    if (commandText == "//TargetPC")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::TARGETPC);
        return true;
    }

    if (commandText == "//TargetNPC")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::TARGETNPC);
        return true;
    }

    if (commandText == "//TargetCorpse")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::TARGETCORPSE);
        return true;
    }

    if (commandText == "//Hot1_1")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::HOT1_1);
        return true;
    }

    if (commandText == "//Hot1_2")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::HOT1_2);
        return true;
    }

    if (commandText == "//Hot1_3")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::HOT1_3);
        return true;
    }

    if (commandText == "//Hot1_4")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::HOT1_4);
        return true;
    }

    if (commandText == "//Hot1_5")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::HOT1_5);
        return true;
    }

    if (commandText == "//Hot1_6")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::HOT1_6);
        return true;
    }

    if (commandText == "//Hot1_7")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::HOT1_7);
        return true;
    }

    if (commandText == "//Hot1_8")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::HOT1_8);
        return true;
    }

    if (commandText == "//Hot1_9")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::HOT1_9);
        return true;
    }

    if (commandText == "//Hot1_10")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::HOT1_10);
        return true;
    }

    if (commandText == "//Hot1_11")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::HOT1_11);
        return true;
    }

    if (commandText == "//Hot1_12")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::HOT1_12);
        return true;
    }

    if (commandText == "//Hot2_1")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::HOT2_1);
        return true;
    }

    if (commandText == "//Hot2_2")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::HOT2_2);
        return true;
    }

    if (commandText == "//Hot2_3")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::HOT2_3);
        return true;
    }

    if (commandText == "//Hot2_4")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::HOT2_4);
        return true;
    }

    if (commandText == "//Hot2_5")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::HOT2_5);
        return true;
    }

    if (commandText == "//Hot2_6")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::HOT2_6);
        return true;
    }

    if (commandText == "//Hot2_7")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::HOT2_7);
        return true;
    }

    if (commandText == "//Hot2_8")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::HOT2_8);
        return true;
    }

    if (commandText == "//Hot2_9")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::HOT2_9);
        return true;
    }

    if (commandText == "//Hot2_10")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::HOT2_10);
        return true;
    }

    if (commandText == "//Hot2_11")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::HOT2_11);
        return true;
    }

    if (commandText == "//Hot2_12")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::HOT2_12);
        return true;
    }

    if (commandText == "//Hot3_1")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::HOT3_1);
        return true;
    }

    if (commandText == "//Hot3_2")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::HOT3_2);
        return true;
    }

    if (commandText == "//Hot3_3")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::HOT3_3);
        return true;
    }

    if (commandText == "//Hot3_4")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::HOT3_4);
        return true;
    }

    if (commandText == "//Hot3_5")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::HOT3_5);
        return true;
    }

    if (commandText == "//Hot3_6")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::HOT3_6);
        return true;
    }

    if (commandText == "//Hot3_7")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::HOT3_7);
        return true;
    }

    if (commandText == "//Hot3_8")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::HOT3_8);
        return true;
    }

    if (commandText == "//Hot3_9")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::HOT3_9);
        return true;
    }

    if (commandText == "//Hot3_10")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::HOT3_10);
        return true;
    }

    if (commandText == "//Hot3_11")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::HOT3_11);
        return true;
    }

    if (commandText == "//Hot3_12")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::HOT3_12);
        return true;
    }

    if (commandText == "//Hot4_1")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::HOT4_1);
        return true;
    }

    if (commandText == "//Hot4_2")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::HOT4_2);
        return true;
    }

    if (commandText == "//Hot4_3")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::HOT4_3);
        return true;
    }

    if (commandText == "//Hot4_4")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::HOT4_4);
        return true;
    }

    if (commandText == "//Hot4_5")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::HOT4_5);
        return true;
    }

    if (commandText == "//Hot4_6")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::HOT4_6);
        return true;
    }

    if (commandText == "//Hot4_7")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::HOT4_7);
        return true;
    }

    if (commandText == "//Hot4_8")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::HOT4_8);
        return true;
    }

    if (commandText == "//Hot4_9")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::HOT4_9);
        return true;
    }

    if (commandText == "//Hot4_10")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::HOT4_10);
        return true;
    }

    if (commandText == "//Hot4_11")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::HOT4_11);
        return true;
    }

    if (commandText == "//Hot4_12")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::HOT4_12);
        return true;
    }

    if (commandText == "//Hot5_1")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::HOT5_1);
        return true;
    }

    if (commandText == "//Hot5_2")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::HOT5_2);
        return true;
    }

    if (commandText == "//Hot5_3")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::HOT5_3);
        return true;
    }

    if (commandText == "//Hot5_4")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::HOT5_4);
        return true;
    }

    if (commandText == "//Hot5_5")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::HOT5_5);
        return true;
    }

    if (commandText == "//Hot5_6")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::HOT5_6);
        return true;
    }

    if (commandText == "//Hot5_7")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::HOT5_7);
        return true;
    }

    if (commandText == "//Hot5_8")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::HOT5_8);
        return true;
    }

    if (commandText == "//Hot5_9")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::HOT5_9);
        return true;
    }

    if (commandText == "//Hot5_10")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::HOT5_10);
        return true;
    }

    if (commandText == "//Hot5_11")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::HOT5_11);
        return true;
    }

    if (commandText == "//Hot5_12")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::HOT5_12);
        return true;
    }

    if (commandText == "//Hot6_1")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::HOT6_1);
        return true;
    }

    if (commandText == "//Hot6_2")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::HOT6_2);
        return true;
    }

    if (commandText == "//Hot6_3")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::HOT6_3);
        return true;
    }

    if (commandText == "//Hot6_4")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::HOT6_4);
        return true;
    }

    if (commandText == "//Hot6_5")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::HOT6_5);
        return true;
    }

    if (commandText == "//Hot6_6")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::HOT6_6);
        return true;
    }

    if (commandText == "//Hot6_7")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::HOT6_7);
        return true;
    }

    if (commandText == "//Hot6_8")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::HOT6_8);
        return true;
    }

    if (commandText == "//Hot6_9")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::HOT6_9);
        return true;
    }

    if (commandText == "//Hot6_10")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::HOT6_10);
        return true;
    }

    if (commandText == "//Hot6_11")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::HOT6_11);
        return true;
    }

    if (commandText == "//Hot6_12")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::HOT6_12);
        return true;
    }

    if (commandText == "//Hot7_1")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::HOT7_1);
        return true;
    }

    if (commandText == "//Hot7_2")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::HOT7_2);
        return true;
    }

    if (commandText == "//Hot7_3")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::HOT7_3);
        return true;
    }

    if (commandText == "//Hot7_4")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::HOT7_4);
        return true;
    }

    if (commandText == "//Hot7_5")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::HOT7_5);
        return true;
    }

    if (commandText == "//Hot7_6")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::HOT7_6);
        return true;
    }

    if (commandText == "//Hot7_7")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::HOT7_7);
        return true;
    }

    if (commandText == "//Hot7_8")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::HOT7_8);
        return true;
    }

    if (commandText == "//Hot7_9")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::HOT7_9);
        return true;
    }

    if (commandText == "//Hot7_10")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::HOT7_10);
        return true;
    }

    if (commandText == "//Hot7_11")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::HOT7_11);
        return true;
    }

    if (commandText == "//Hot7_12")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::HOT7_12);
        return true;
    }

    if (commandText == "//Hot8_1")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::HOT8_1);
        return true;
    }

    if (commandText == "//Hot8_2")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::HOT8_2);
        return true;
    }

    if (commandText == "//Hot8_3")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::HOT8_3);
        return true;
    }

    if (commandText == "//Hot8_4")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::HOT8_4);
        return true;
    }

    if (commandText == "//Hot8_5")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::HOT8_5);
        return true;
    }

    if (commandText == "//Hot8_6")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::HOT8_6);
        return true;
    }

    if (commandText == "//Hot8_7")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::HOT8_7);
        return true;
    }

    if (commandText == "//Hot8_8")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::HOT8_8);
        return true;
    }

    if (commandText == "//Hot8_9")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::HOT8_9);
        return true;
    }

    if (commandText == "//Hot8_10")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::HOT8_10);
        return true;
    }

    if (commandText == "//Hot8_11")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::HOT8_11);
        return true;
    }

    if (commandText == "//Hot8_12")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::HOT8_12);
        return true;
    }

    if (commandText == "//Hot9_1")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::HOT9_1);
        return true;
    }

    if (commandText == "//Hot9_2")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::HOT9_2);
        return true;
    }

    if (commandText == "//Hot9_3")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::HOT9_3);
        return true;
    }

    if (commandText == "//Hot9_4")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::HOT9_4);
        return true;
    }

    if (commandText == "//Hot9_5")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::HOT9_5);
        return true;
    }

    if (commandText == "//Hot9_6")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::HOT9_6);
        return true;
    }

    if (commandText == "//Hot9_7")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::HOT9_7);
        return true;
    }

    if (commandText == "//Hot9_8")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::HOT9_8);
        return true;
    }

    if (commandText == "//Hot9_9")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::HOT9_9);
        return true;
    }

    if (commandText == "//Hot9_10")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::HOT9_10);
        return true;
    }

    if (commandText == "//Hot9_11")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::HOT9_11);
        return true;
    }

    if (commandText == "//Hot9_12")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::HOT9_12);
        return true;
    }

    if (commandText == "//Hot10_1")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::HOT10_1);
        return true;
    }

    if (commandText == "//Hot10_2")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::HOT10_2);
        return true;
    }

    if (commandText == "//Hot10_3")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::HOT10_3);
        return true;
    }

    if (commandText == "//Hot10_4")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::HOT10_4);
        return true;
    }

    if (commandText == "//Hot10_5")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::HOT10_5);
        return true;
    }

    if (commandText == "//Hot10_6")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::HOT10_6);
        return true;
    }

    if (commandText == "//Hot10_7")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::HOT10_7);
        return true;
    }

    if (commandText == "//Hot10_8")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::HOT10_8);
        return true;
    }

    if (commandText == "//Hot10_9")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::HOT10_9);
        return true;
    }

    if (commandText == "//Hot10_10")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::HOT10_10);
        return true;
    }

    if (commandText == "//Hot10_11")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::HOT10_11);
        return true;
    }

    if (commandText == "//Hot10_12")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::HOT10_12);
        return true;
    }

    if (commandText == "//Hot11_1")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::HOT11_1);
        return true;
    }

    if (commandText == "//Hot11_2")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::HOT11_2);
        return true;
    }

    if (commandText == "//Hot11_3")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::HOT11_3);
        return true;
    }

    if (commandText == "//Hot11_4")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::HOT11_4);
        return true;
    }

    if (commandText == "//Hot11_5")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::HOT11_5);
        return true;
    }

    if (commandText == "//Hot11_6")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::HOT11_6);
        return true;
    }

    if (commandText == "//Hot11_7")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::HOT11_7);
        return true;
    }

    if (commandText == "//Hot11_8")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::HOT11_8);
        return true;
    }

    if (commandText == "//Hot11_9")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::HOT11_9);
        return true;
    }

    if (commandText == "//Hot11_10")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::HOT11_10);
        return true;
    }

    if (commandText == "//Hot11_11")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::HOT11_11);
        return true;
    }

    if (commandText == "//Hot11_12")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::HOT11_12);
        return true;
    }

    if (commandText == "//Cast1")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::CAST1);
        return true;
    }

    if (commandText == "//Cast2")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::CAST2);
        return true;
    }

    if (commandText == "//Cast3")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::CAST3);
        return true;
    }

    if (commandText == "//Cast4")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::CAST4);
        return true;
    }

    if (commandText == "//Cast5")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::CAST5);
        return true;
    }

    if (commandText == "//Cast6")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::CAST6);
        return true;
    }

    if (commandText == "//Cast7")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::CAST7);
        return true;
    }

    if (commandText == "//Cast8")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::CAST8);
        return true;
    }

    if (commandText == "//Cast9")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::CAST9);
        return true;
    }

    if (commandText == "//Cast10")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::CAST10);
        return true;
    }

    if (commandText == "//Cast11")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::CAST11);
        return true;
    }

    if (commandText == "//Cast12")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::CAST12);
        return true;
    }

    if (commandText == "//Cast13")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::CAST13);
        return true;
    }

    if (commandText == "//Cast14")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::CAST14);
        return true;
    }

    if (commandText == "//Who")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::WHO);
        return true;
    }

    if (commandText == "//InviteFollow")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::INVITE_FOLLOW);
        return true;
    }

    if (commandText == "//Disband")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::DISBAND);
        return true;
    }

    if (commandText == "//Camp")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::CAMP);
        return true;
    }

    if (commandText == "//SitStand")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::SIT_STAND);
        return true;
    }

    if (commandText == "//RunWalk")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::RUN_WALK);
        return true;
    }

    if (commandText == "//Screenshot")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::SCREENCAP);
        return true;
    }

    if (commandText == "//FirstPersonCamera")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::FIRST_PERSON_CAMERA);
        return true;
    }

    if (commandText == "//OverheadCamera")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::OVERHEAD_CAMERA);
        return true;
    }

    if (commandText == "//ChaseCamera")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::CHASE_CAMERA);
        return true;
    }

    if (commandText == "//User1Camera")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::USER1_CAMERA);
        return true;
    }

    if (commandText == "//User2Camera")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::USER2_CAMERA);
        return true;
    }

    if (commandText == "//TetherCamera")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::TETHER_CAMERA);
        return true;
    }

    if (commandText == "//CloseTopWindow")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::CLOSE_TOP_WINDOW);
        return true;
    }

    if (commandText == "//ClearTarget")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::CLEAR_TARGET);
        return true;
    }

    if (commandText == "//StopCast")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::CMD_STOP_CAST);
        return true;
    }

    if (commandText == "//OpenBags")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::OPEN_INV_BAGS);
        return true;
    }

    if (commandText == "//CloseBags")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::CLOSE_INV_BAGS);
        return true;
    }

    if (commandText == "//ToggleBags")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::TOGGLE_INV_BAGS);
        return true;
    }

    if (commandText == "//ClipboardPaste")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::CMD_CLIPBOARD_PASTE);
        return true;
    }

    if (commandText == "//XTarget1")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::TARGET_XTARGET_1);
        return true;
    }

    if (commandText == "//XTarget2")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::TARGET_XTARGET_2);
        return true;
    }

    if (commandText == "//XTarget3")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::TARGET_XTARGET_3);
        return true;
    }

    if (commandText == "//XTarget4")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::TARGET_XTARGET_4);
        return true;
    }

    if (commandText == "//XTarget5")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::TARGET_XTARGET_5);
        return true;
    }

    if (commandText == "//XTarget6")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::TARGET_XTARGET_6);
        return true;
    }

    if (commandText == "//XTarget7")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::TARGET_XTARGET_7);
        return true;
    }

    if (commandText == "//XTarget8")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::TARGET_XTARGET_8);
        return true;
    }

    if (commandText == "//XTarget9")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::TARGET_XTARGET_9);
        return true;
    }

    if (commandText == "//XTarget10")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::TARGET_XTARGET_10);
        return true;
    }

    if (commandText == "//XTarget11")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::TARGET_XTARGET_11);
        return true;
    }

    if (commandText == "//XTarget12")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::TARGET_XTARGET_12);
        return true;
    }

    if (commandText == "//XTarget13")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::TARGET_XTARGET_13);
        return true;
    }

    if (commandText == "//XTarget14")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::TARGET_XTARGET_14);
        return true;
    }

    if (commandText == "//XTarget15")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::TARGET_XTARGET_15);
        return true;
    }

    if (commandText == "//XTarget16")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::TARGET_XTARGET_16);
        return true;
    }

    if (commandText == "//XTarget17")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::TARGET_XTARGET_17);
        return true;
    }

    if (commandText == "//XTarget18")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::TARGET_XTARGET_18);
        return true;
    }

    if (commandText == "//XTarget19")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::TARGET_XTARGET_19);
        return true;
    }

    if (commandText == "//XTarget20")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::TARGET_XTARGET_20);
        return true;
    }

    if (commandText == "//MoveDown")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::CMD_MOVE_DOWN);
        return true;
    }

    if (commandText == "//StopAction")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::CMD_STOP_ACTION);
        return true;
    }

    if (commandText == "//InspectBuffs")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::CMD_INSPECT_BUFFS);
        return true;
    }

    if (commandText == "//ToggleFPS")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::TOGGLE_FPS);
        return true;
    }

    if (commandText == "//ToggleMemInfo")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::TOGGLE_MEMINFO);
        return true;
    }

    if (commandText == "//ConfirmYes")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::CONFIRM_YES);
        return true;
    }

    if (commandText == "//ConfirmNo")
    {
        EQ_ExecuteCommand(eq::Constants::ExecuteCommand::CONFIRM_NO);
        return true;
    }

    return false;
}

}

inline eqapp::InterpretCommand& g_InterpretCommand = eqapp::InterpretCommand::GetInstance();

}
