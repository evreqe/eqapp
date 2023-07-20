module;

#include "common.h"
#include "utility.h"

export module eqapp_interpretcommand;

export import eq;
export import eq_functions;

export import eqapp_log;

export
{

void EQAPP_InterpretCommand_ConvertText(std::string& text);
bool EQAPP_InterpretCommand_HandleInterpretCommand(const std::string& commandText);

void EQAPP_InterpretCommand_ConvertText(std::string& text)
{
    if (text.contains("{PlayerID}") == true)
    {
        uint32_t playerSpawnID = EQ_GetPlayerSpawnID();
        if (playerSpawnID != 0xFFFFFFFF)
        {
            std::string playerSpawnIDAsString = std::to_string(playerSpawnID);
            if (playerSpawnIDAsString.size() != 0)
            {
                util::String::ReplaceAll(text, "{PlayerID}", playerSpawnIDAsString);
            }
        }
    }

    if (text.contains("{PlayerName}") == true)
    {
        std::string playerSpawnName = EQ_GetPlayerSpawnName();
        if (playerSpawnName.size() != 0)
        {
            util::String::ReplaceAll(text, "{PlayerName}", playerSpawnName);
        }
    }

    if (text.contains("{TargetID}") == true)
    {
        uint32_t targetSpawnID = EQ_GetTargetSpawnID();
        if (targetSpawnID != 0xFFFFFFFF)
        {
            std::string targetSpawnIDAsString = std::to_string(targetSpawnID);
            if (targetSpawnIDAsString.size() != 0)
            {
                util::String::ReplaceAll(text, "{TargetID}", targetSpawnIDAsString);
            }
        }
    }

    if (text.contains("{TargetName}") == true)
    {
        std::string targetSpawnName = EQ_GetTargetSpawnName();
        if (targetSpawnName.size() != 0)
        {
            util::String::ReplaceAll(text, "{TargetName}", targetSpawnName);
        }
    }
}

bool EQAPP_InterpretCommand_HandleInterpretCommand(const std::string& commandText)
{
    if (commandText.starts_with("//Test ") == true)
    {
        std::string arg0;
        std::string arg1;
        std::string arg2;
        auto result = scn::scan(commandText, "{} {} {}", arg0, arg1, arg2);
        if (result)
        {
            std::string_view remainder = result.range_as_string_view();

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

    if (commandText == "//Jump")
    {
        EQ_ExecuteCommandEx(eq::Constants::ExecuteCommand::JUMP, true);
        EQ_ExecuteCommandEx(eq::Constants::ExecuteCommand::JUMP, false);
        return true;
    }

    return false;
}

}
