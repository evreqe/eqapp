module;

#include "common.h"

export module eqapp_detours;

export import eq;
export import eq_classes;
export import eq_functions;

export import eqapp_log;

export
{

#define EQ_DEFINE_DETOUR(functionName) EQ_FUNCTION_TYPE_##functionName EQAPP_REAL_FUNCTION_##functionName = NULL;

#define EQ_EQGAME_INIT_DETOUR(functionName) EQAPP_REAL_FUNCTION_##functionName = (EQ_FUNCTION_TYPE_##functionName)eq::EQGame::Addresses::Functions::##functionName;

#define EQ_ADD_DETOUR(functionName) DetourAttach(&(PVOID&)EQAPP_REAL_FUNCTION_##functionName, EQAPP_DETOURED_FUNCTION_##functionName);

#define EQ_REMOVE_DETOUR(functionName) DetourDetach(&(PVOID&)EQAPP_REAL_FUNCTION_##functionName, EQAPP_DETOURED_FUNCTION_##functionName);

EQ_DEFINE_DETOUR(DrawNetStatus);
EQ_DEFINE_DETOUR(ExecuteCommand);
EQ_DEFINE_DETOUR(CEverQuest__InterpretCommand);

void EQAPP_Detours_Load();
void EQAPP_Detours_Unload();

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

void EQAPP_DETOURED_FUNCTION_DrawNetStatus(uint32_t x, uint32_t y, uintptr_t worldPointer)
{
    if (worldPointer == NULL)
    {
        return;
    }

    EQ_DrawText("EQ Application", 8, 32);

    EQAPP_REAL_FUNCTION_DrawNetStatus(x, y, worldPointer);
}

bool EQAPP_DETOURED_FUNCTION_ExecuteCommand(uint32_t commandID, bool keyDown, void* data, void* keyCombo)
{
    g_Log.write("ExecuteCommand() commandID: {}\n", commandID);
    g_Log.write("ExecuteCommand() keyDown: {}\n", keyDown);
    g_Log.write("----------------------------\n", keyDown);

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

    if (strcmp(text, "//Sit") == 0)
    {
        EQ_InterpretCommand("/sit");
        EQ_PrintTextToChat("You do a sit!");
        return;
    }

    if (strcmp(text, "//Jump") == 0)
    {
        EQ_ExecuteCommandEx(eq::Constants::ExecuteCommand::JUMP, true);
        EQ_ExecuteCommandEx(eq::Constants::ExecuteCommand::JUMP, false);
        EQ_PrintTextToChat("You do a jump!");
        return;
    }

    EQAPP_REAL_FUNCTION_CEverQuest__InterpretCommand(thisPointer, playerSpawn, text);
}

}
