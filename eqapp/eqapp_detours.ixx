module;

#include "common.h"

export module eqapp_detours;

export import eq;
export import eq_functions;

export import eqapp_log;

export import eqapp_console;
export import eqapp_boxchatclient;

export
{

bool g_EQApp_Detours_IsLooping = false;;

#define EQ_DEFINE_DETOUR(functionName) EQ_FUNCTION_TYPE_##functionName EQAPP_REAL_FUNCTION_##functionName = NULL;

#define EQ_EQGAME_INIT_DETOUR(functionName) EQAPP_REAL_FUNCTION_##functionName = (EQ_FUNCTION_TYPE_##functionName)eq::EQGame::Addresses::Functions::##functionName;

#define EQ_ADD_DETOUR(functionName) DetourAttach(&(PVOID&)EQAPP_REAL_FUNCTION_##functionName, EQAPP_DETOURED_FUNCTION_##functionName);

#define EQ_REMOVE_DETOUR(functionName) DetourDetach(&(PVOID&)EQAPP_REAL_FUNCTION_##functionName, EQAPP_DETOURED_FUNCTION_##functionName);

EQ_DEFINE_DETOUR(DrawNetStatus);
EQ_DEFINE_DETOUR(ExecuteCommand);
EQ_DEFINE_DETOUR(CEverQuest__InterpretCommand);

void EQAPP_Detours_Load();
void EQAPP_Detours_Unload();
void EQAPP_Detours_DisplayText();
void EQAPP_Detours_Loop();

void EQAPP_DETOURED_FUNCTION_DrawNetStatus(uint32_t x, uint32_t y, uintptr_t worldPointer);
bool EQAPP_DETOURED_FUNCTION_ExecuteCommand(uint32_t commandID, bool keyDown, void* data, void* keyCombo);
void EQAPP_DETOURED_FUNCTION_CEverQuest__InterpretCommand(void* thisPointer, uintptr_t* playerSpawn, const char* text);

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

    DetourTransactionCommit();
}

void EQAPP_Detours_Unload()
{
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());

    EQ_REMOVE_DETOUR(DrawNetStatus);
    EQ_REMOVE_DETOUR(ExecuteCommand);
    EQ_REMOVE_DETOUR(CEverQuest__InterpretCommand);

    DetourTransactionCommit();
}

void EQAPP_Detours_DisplayText()
{
    uint32_t drawTextX = 8;
    uint32_t drawTextY = 32;
    uint32_t drawTextOffsetY = 12;

    EQ_DrawText("EQ Application", drawTextX, drawTextY);
    drawTextY += drawTextOffsetY;

    // Console
    if (g_Console.IsLoaded() == true)
    {
        EQ_DrawText("Console loaded", drawTextX, drawTextY);
        drawTextY += drawTextOffsetY;
    }

    // BoxChatClient
    if (g_BoxChatClient.IsLoaded() == true)
    {
        if (g_BoxChatClient.IsEnabled() == true)
        {
            if (g_BoxChatClient.IsConnected() == true)
            {
                EQ_DrawText("Box Chat connected", drawTextX, drawTextY);
            }
            else
            {
                EQ_DrawText("Box Chat disconnected", drawTextX, drawTextY);
            }

            drawTextOffsetY += 16;
        }
    }
}

void EQAPP_Detours_Loop()
{
    EQAPP_Detours_DisplayText();

    // Console
    if (g_Console.IsLoaded() == true)
    {
        g_Console.Print();
    }

    // BoxChatClient
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
    g_Log.write("----------------------------\n");
    g_Log.write("ExecuteCommand() commandID: {}\n", commandID);
    g_Log.write("ExecuteCommand() keyDown: {}\n", keyDown);
    g_Log.write("----------------------------\n");

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

    g_Log.write("CEverQuest__InterpretCommand() text: {}\n", text);

    if (std::strcmp(text, "//Sit") == 0)
    {
        EQ_InterpretCommand("/sit");
        EQ_PrintTextToChat("You do a sit!");
        return;
    }

    if (std::strcmp(text, "//Jump") == 0)
    {
        EQ_ExecuteCommandEx(eq::Constants::ExecuteCommand::JUMP, true);
        EQ_ExecuteCommandEx(eq::Constants::ExecuteCommand::JUMP, false);
        EQ_PrintTextToChat("You do a jump!");
        return;
    }

    // BoxChatClient
    if (g_BoxChatClient.IsLoaded() == true)
    {
        if (g_BoxChatClient.IsEnabled() == true)
        {
            if (g_BoxChatClient.HandleInterpetCommand(text) == true)
            {
                return;
            }
        }
    }

    EQAPP_REAL_FUNCTION_CEverQuest__InterpretCommand(thisPointer, playerSpawn, text);
}

}
