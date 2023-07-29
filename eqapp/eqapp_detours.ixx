module;

#include "common.h"

export module eqapp_detours;

export import eq;
export import eq_functions;

export import eqapp_constants;
export import eqapp_log;
export import eqapp_console;
export import eqapp_boxchatclient;
export import eqapp_follow;
export import eqapp_interpretcommand;
export import eqapp_windowtitle;
export import eqapp_macromanager;

export
{

#define EQ_DEFINE_DETOUR(functionName) EQ_FUNCTION_TYPE_##functionName EQAPP_REAL_FUNCTION_##functionName = NULL;
#define EQ_EQGAME_INIT_DETOUR(functionName) EQAPP_REAL_FUNCTION_##functionName = (EQ_FUNCTION_TYPE_##functionName)eq::EQGame::Addresses::Functions::##functionName;
#define EQ_ADD_DETOUR(functionName) DetourAttach(&(PVOID&)EQAPP_REAL_FUNCTION_##functionName, EQAPP_DETOURED_FUNCTION_##functionName);
#define EQ_REMOVE_DETOUR(functionName) DetourDetach(&(PVOID&)EQAPP_REAL_FUNCTION_##functionName, EQAPP_DETOURED_FUNCTION_##functionName);

EQ_DEFINE_DETOUR(DrawNetStatus);
EQ_DEFINE_DETOUR(ExecuteCommand);
EQ_DEFINE_DETOUR(CEverQuest__InterpretCommand);
EQ_DEFINE_DETOUR(CEverQuest__SetGameState);

void EQAPP_Detours_Load();
void EQAPP_Detours_Unload();
void EQAPP_Detours_DisplayText();
void EQAPP_Detours_Loop();

void EQAPP_DETOURED_FUNCTION_DrawNetStatus(uint32_t x, uint32_t y, uintptr_t worldPointer);
bool EQAPP_DETOURED_FUNCTION_ExecuteCommand(uint32_t commandID, bool keyDown, void* data, void* keyCombo);
void EQAPP_DETOURED_FUNCTION_CEverQuest__InterpretCommand(void* thisPointer, uintptr_t* playerSpawn, const char* text);
void EQAPP_DETOURED_FUNCTION_CEverQuest__SetGameState(void* thisPointer, int gameState);

void EQAPP_Detours_Load()
{
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());

    EQ_EQGAME_INIT_DETOUR(DrawNetStatus);
    EQ_ADD_DETOUR(DrawNetStatus);

    EQ_EQGAME_INIT_DETOUR(ExecuteCommand);
    EQ_ADD_DETOUR(ExecuteCommand);

    EQ_EQGAME_INIT_DETOUR(CEverQuest__InterpretCommand);
    EQ_ADD_DETOUR(CEverQuest__InterpretCommand);

    EQ_EQGAME_INIT_DETOUR(CEverQuest__SetGameState);
    EQ_ADD_DETOUR(CEverQuest__SetGameState);

    DetourTransactionCommit();
}

void EQAPP_Detours_Unload()
{
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());

    EQ_REMOVE_DETOUR(DrawNetStatus);
    EQ_REMOVE_DETOUR(ExecuteCommand);
    EQ_REMOVE_DETOUR(CEverQuest__InterpretCommand);
    EQ_REMOVE_DETOUR(CEverQuest__SetGameState);

    DetourTransactionCommit();
}

void EQAPP_Detours_DisplayText()
{
    const uint32_t drawTextX = 8;
    const uint32_t drawTextY = 32;

    std::string displayText;

    auto displayTextBackInserter = std::back_inserter(displayText);

    std::format_to(displayTextBackInserter, "{}\n", eqapp::Constants::ApplicationName);

    // Box Chat
    if (g_BoxChatClient.IsLoaded() == true)
    {
         displayText.append(g_BoxChatClient.GetDisplayText());
    }

    // Follow
    if (g_Follow.IsLoaded() == true)
    {
        displayText.append(g_Follow.GetDisplayText());
    }

    // Macro Manager
    if (g_MacroManager.IsLoaded() == true)
    {
        displayText.append(g_MacroManager.GetDisplayText());
    }

    EQ_DrawText(displayText, drawTextX, drawTextY);
}

void EQAPP_Detours_Loop()
{
    EQAPP_Detours_DisplayText();

    // Console
    if (g_Console.IsLoaded() == true)
    {
        g_Console.Print();
    }

    // Interpret Command
    if (g_InterpretCommand.IsLoaded() == true)
    {
        if (g_InterpretCommand.IsEnabled() == true)
        {
            g_InterpretCommand.Execute();
        }
    }

    // Macro Manager
    if (g_MacroManager.IsLoaded() == true)
    {
        if (g_MacroManager.IsEnabled() == true)
        {
            g_MacroManager.Execute();
        }
    }

    // Window Title
    if (g_WindowTitle.IsLoaded() == true)
    {
        if (g_WindowTitle.IsEnabled() == true)
        {
            g_WindowTitle.Execute();
        }
    }

    // Box Chat
    if (g_BoxChatClient.IsLoaded() == true)
    {
        if (g_BoxChatClient.IsEnabled() == true)
        {
            if (g_BoxChatClient.IsConnected() == true)
            {
                g_BoxChatClient.Execute();
            }
        }
    }

    // Follow
    if (g_Follow.IsLoaded() == true)
    {
        if (g_Follow.IsEnabled() == true)
        {
            if (g_Follow.IsFollowing() == true)
            {
                g_Follow.Execute();
            }
        }
    }
}

void EQAPP_DETOURED_FUNCTION_DrawNetStatus(uint32_t x, uint32_t y, uintptr_t worldPointer)
{
    if (worldPointer == NULL)
    {
        return;
    }

    EQAPP_Detours_Loop();

    EQAPP_REAL_FUNCTION_DrawNetStatus(x, y, worldPointer);
}

bool EQAPP_DETOURED_FUNCTION_ExecuteCommand(uint32_t commandID, bool keyDown, void* data, void* keyCombo)
{
    //g_Log.write("----------------------------\n");
    //g_Log.write("ExecuteCommand() commandID: {}\n", commandID);
    //g_Log.write("ExecuteCommand() keyDown: {}\n", keyDown);
    //g_Log.write("----------------------------\n");

    // Follow
    if (g_Follow.IsLoaded() == true)
    {
        if (g_Follow.IsEnabled() == true)
        {
            if (g_Follow.IsFollowing() == true)
            {
                g_Follow.HandleExecuteCommand(commandID, keyDown);
            }
        }
    }

    return EQAPP_REAL_FUNCTION_ExecuteCommand(commandID, keyDown, data, keyCombo);
}

void EQAPP_DETOURED_FUNCTION_CEverQuest__InterpretCommand(void* thisPointer, uintptr_t* playerSpawn, const char* text)
{
    if (thisPointer == NULL)
    {
        return;
    }

    if (playerSpawn == nullptr)
    {
        return;
    }

    if (text == NULL)
    {
        return;
    }

    //g_Log.write("CEverQuest__InterpretCommand() text: {}\n", text);

    std::string commandText = text;

    if (commandText.size() == 0)
    {
        return;
    }

    if (commandText.starts_with("//") == true)
    {
        g_InterpretCommand.ConvertCommandText(commandText);

        // Interpret Command
        if (g_InterpretCommand.IsLoaded() == true)
        {
            if (g_InterpretCommand.HandleInterpretCommand(commandText) == true)
            {
                return;
            }
        }

        // Macro Manager
        if (g_MacroManager.IsLoaded() == true)
        {
            if (g_MacroManager.HandleInterpetCommand(commandText) == true)
            {
                return;
            }
        }

        // Window Title
        if (g_WindowTitle.IsLoaded() == true)
        {
            if (g_WindowTitle.HandleInterpetCommand(commandText) == true)
            {
                return;
            }
        }

        // Box Chat
        if (g_BoxChatClient.IsLoaded() == true)
        {
            if (g_BoxChatClient.HandleInterpetCommand(commandText) == true)
            {
                return;
            }
        }

        // Follow
        if (g_Follow.IsLoaded() == true)
        {
            if (g_Follow.HandleInterpetCommand(commandText) == true)
            {
                return;
            }
        }
    }

    EQAPP_REAL_FUNCTION_CEverQuest__InterpretCommand(thisPointer, playerSpawn, text);
}

void EQAPP_DETOURED_FUNCTION_CEverQuest__SetGameState(void* thisPointer, int gameState)
{
    if (thisPointer == NULL)
    {
        return;
    }

    // Follow
    if (g_Follow.IsLoaded() == true)
    {
        if (g_Follow.IsFollowing() == true)
        {
            g_Follow.StopFollowing();
        }
    }

    EQAPP_REAL_FUNCTION_CEverQuest__SetGameState(thisPointer, gameState);
}

}
