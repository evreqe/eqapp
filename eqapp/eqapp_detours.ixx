module;

#include "common.h"

export module eqapp_detours;

export import eq;
export import eq_functions;

export import eqapp_constants;
export import eqapp_log;
export import eqapp_console;
export import eqapp_nodraw;
export import eqapp_boxchatclient;
export import eqapp_chatevents;
export import eqapp_follow;
export import eqapp_interpretcommand;
export import eqapp_windowtitle;
export import eqapp_macromanager;
export import eqapp_soundmanager;
export import eqapp_esp;

export
{

#define EQ_DEFINE_DETOUR(functionName) EQ_FUNCTION_TYPE_##functionName EQAPP_REAL_FUNCTION_##functionName = NULL;
#define EQ_EQGAME_INIT_DETOUR(functionName) EQAPP_REAL_FUNCTION_##functionName = (EQ_FUNCTION_TYPE_##functionName)eq::EQGame::Addresses::Functions::##functionName;
#define EQ_EQGRAPHICS_INIT_DETOUR(functionName) EQAPP_REAL_FUNCTION_##functionName = (EQ_FUNCTION_TYPE_##functionName)eq::EQGraphics::Addresses::Functions::##functionName;
#define EQ_ADD_DETOUR(functionName) DetourAttach(&(PVOID&)EQAPP_REAL_FUNCTION_##functionName, EQAPP_DETOURED_FUNCTION_##functionName);
#define EQ_REMOVE_DETOUR(functionName) DetourDetach(&(PVOID&)EQAPP_REAL_FUNCTION_##functionName, EQAPP_DETOURED_FUNCTION_##functionName);

EQ_DEFINE_DETOUR(DrawNetStatus)
EQ_DEFINE_DETOUR(ExecuteCommand)
EQ_DEFINE_DETOUR(ChatManager__PrintText)
EQ_DEFINE_DETOUR(SoundManager__PlaySound)
EQ_DEFINE_DETOUR(CEverQuest__InterpretCommand)
EQ_DEFINE_DETOUR(CEverQuest__SetGameState)
EQ_DEFINE_DETOUR(CRender__RenderScene)
EQ_DEFINE_DETOUR(CRender__UpdateDisplay)
EQ_DEFINE_DETOUR(CParticleSystem__CreateSpellEmitter)

void EQAPP_Detours_Load();
void EQAPP_Detours_Unload();
void EQAPP_Detours_DisplayText();
void EQAPP_Detours_Loop();

void EQAPP_DETOURED_FUNCTION_DrawNetStatus(uint32_t x, uint32_t y, uintptr_t worldPointer);
bool EQAPP_DETOURED_FUNCTION_ExecuteCommand(uint32_t commandID, bool keyDown, void* data, void* keyCombo);
void EQAPP_DETOURED_FUNCTION_ChatManager__PrintText(void* thisPointer, const char* text, uint32_t chatTextColor, bool isLogOK, bool doPercentConvert, char* nullString);
void EQAPP_DETOURED_FUNCTION_SoundManager__PlaySound(void* thisPointer, int soundID, void* soundControl);
void EQAPP_DETOURED_FUNCTION_CEverQuest__InterpretCommand(void* thisPointer, uintptr_t* playerSpawn, const char* text);
void EQAPP_DETOURED_FUNCTION_CEverQuest__SetGameState(void* thisPointer, int gameState);
void EQAPP_DETOURED_FUNCTION_CRender__RenderScene(void* thisPointer);
void EQAPP_DETOURED_FUNCTION_CRender__UpdateDisplay(void* thisPointer);
int EQAPP_DETOURED_FUNCTION_CParticleSystem__CreateSpellEmitter
(
    void* thisPointer,
    int index,
    unsigned long arg2,
    int arg3,
    float particleDensity,
    float particleOpacity,
    void* position,
    void* actor,
    void* bone,
    void* particlePoint,
    void** emitter,
    float arg11,
    bool arg12,
    bool arg13,
    int previewMode
);

void EQAPP_Detours_Load()
{
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());

    EQ_EQGAME_INIT_DETOUR(DrawNetStatus)
    EQ_ADD_DETOUR(DrawNetStatus)

    EQ_EQGAME_INIT_DETOUR(ExecuteCommand)
    EQ_ADD_DETOUR(ExecuteCommand)

    EQ_EQGAME_INIT_DETOUR(ChatManager__PrintText)
    EQ_ADD_DETOUR(ChatManager__PrintText)

    EQ_EQGAME_INIT_DETOUR(SoundManager__PlaySound)
    EQ_ADD_DETOUR(SoundManager__PlaySound)

    EQ_EQGAME_INIT_DETOUR(CEverQuest__InterpretCommand)
    EQ_ADD_DETOUR(CEverQuest__InterpretCommand)

    EQ_EQGAME_INIT_DETOUR(CEverQuest__SetGameState)
    EQ_ADD_DETOUR(CEverQuest__SetGameState)

    EQ_EQGRAPHICS_INIT_DETOUR(CRender__RenderScene)
    EQ_ADD_DETOUR(CRender__RenderScene)

    EQ_EQGRAPHICS_INIT_DETOUR(CRender__UpdateDisplay)
    EQ_ADD_DETOUR(CRender__UpdateDisplay)

    EQ_EQGRAPHICS_INIT_DETOUR(CParticleSystem__CreateSpellEmitter)
    EQ_ADD_DETOUR(CParticleSystem__CreateSpellEmitter)

    DetourTransactionCommit();
}

void EQAPP_Detours_Unload()
{
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());

    EQ_REMOVE_DETOUR(DrawNetStatus)
    EQ_REMOVE_DETOUR(ExecuteCommand)
    EQ_REMOVE_DETOUR(ChatManager__PrintText)
    EQ_REMOVE_DETOUR(SoundManager__PlaySound)
    EQ_REMOVE_DETOUR(CEverQuest__InterpretCommand)
    EQ_REMOVE_DETOUR(CEverQuest__SetGameState)
    EQ_REMOVE_DETOUR(CRender__RenderScene)
    EQ_REMOVE_DETOUR(CRender__UpdateDisplay)
    EQ_REMOVE_DETOUR(CParticleSystem__CreateSpellEmitter)

    DetourTransactionCommit();
}

void EQAPP_Detours_DisplayText()
{
    const uint32_t drawTextX = 8;
    const uint32_t drawTextY = 32;

    std::string displayText = std::format("{}\n", eqapp::Constants::ApplicationName);

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

    // ESP
    if (g_ESP.IsLoaded() == true)
    {
        if (g_ESP.IsEnabled() == true)
        {
            g_ESP.Execute();
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

    // Sound Manager
    if (g_SoundManager.IsLoaded() == true)
    {
        if (g_SoundManager.IsEnabled() == true)
        {
            //g_SoundManager.Execute();
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

void EQAPP_DETOURED_FUNCTION_ChatManager__PrintText(void* thisPointer, const char* text, uint32_t chatTextColor, bool isLogOK, bool doPercentConvert, char* nullString)
{
    if (thisPointer == NULL)
    {
        return;
    }

    //uintptr_t chatManager = EQ_GetChatManager();
    //if ((uintptr_t)thisPointer != chatManager)
    //{
        //return;
    //}

    if (text == NULL)
    {
        return;
    }

    //g_Log.write("ChatManager__PrintText() text: {}\n", text);

    std::string chatText = text;

    if (chatText.empty() == true)
    {
        return;
    }

    // Chat Events
    if (g_ChatEvents.IsLoaded() == true)
    {
        if (g_ChatEvents.IsEnabled() == true)
        {
            if (g_ChatEvents.HandleChatManagerPrintText(chatText) == true)
            {
                std::print(std::cout, "#Chat Event!\n");
            }
        }
    }

    EQAPP_REAL_FUNCTION_ChatManager__PrintText(thisPointer, text, chatTextColor, isLogOK, doPercentConvert, nullString);
}

void EQAPP_DETOURED_FUNCTION_SoundManager__PlaySound(void* thisPointer, int soundID, void* soundControl)
{
    if (thisPointer == NULL)
    {
        return;
    }

    if (soundControl == NULL)
    {
        return;
    }

    // Sound Manager
    if (g_SoundManager.IsLoaded() == true)
    {
        if (g_SoundManager.IsEnabled() == true)
        {
            if (g_SoundManager.HandleSoundManagerPlaySound(soundID) == true)
            {
                return;
            }
        }
    }

    EQAPP_REAL_FUNCTION_SoundManager__PlaySound(thisPointer, soundID, soundControl);
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

    if (commandText.empty() == true)
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

        // ESP
        if (g_ESP.IsLoaded() == true)
        {
            if (g_ESP.HandleInterpetCommand(commandText) == true)
            {
                return;
            }
        }

        // Chat Events
        if (g_ChatEvents.IsLoaded() == true)
        {
            if (g_ChatEvents.HandleInterpetCommand(commandText) == true)
            {
                return;
            }
        }

        // Sound Manager
        if (g_SoundManager.IsLoaded() == true)
        {
            if (g_SoundManager.HandleInterpetCommand(commandText) == true)
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

        // No Draw
        if (g_NoDraw.IsLoaded() == true)
        {
            if (g_NoDraw.HandleInterpetCommand(commandText) == true)
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

void EQAPP_DETOURED_FUNCTION_CRender__RenderScene(void* thisPointer)
{
    if (thisPointer == NULL)
    {
        return;
    }

    // No Draw
    if (g_NoDraw.IsLoaded() == true)
    {
        if (g_NoDraw.IsEnabled() == true)
        {
            if (g_NoDraw.HandleCRenderRenderScene() == true)
            {
                return;
            }
        }
    }

    EQAPP_REAL_FUNCTION_CRender__RenderScene(thisPointer);
}

void EQAPP_DETOURED_FUNCTION_CRender__UpdateDisplay(void* thisPointer)
{
    if (thisPointer == NULL)
    {
        return;
    }

    // No Draw
    if (g_NoDraw.IsLoaded() == true)
    {
        if (g_NoDraw.IsEnabled() == true)
        {
            if (g_NoDraw.HandleCRenderRenderScene() == true)
            {
                return;
            }
        }
    }

    EQAPP_REAL_FUNCTION_CRender__UpdateDisplay(thisPointer);
}

int EQAPP_DETOURED_FUNCTION_CParticleSystem__CreateSpellEmitter
(
    void* thisPointer,
    int index,
    unsigned long arg2,
    int arg3,
    float particleDensity,
    float particleOpacity,
    void* position,
    void* actor,
    void* bone,
    void* particlePoint,
    void** emitter,
    float arg11,
    bool arg12,
    bool arg13,
    int previewMode
)
{
    if (thisPointer == NULL)
    {
        return 0;
    }

    // No Draw
    if (g_NoDraw.IsLoaded() == true)
    {
        if (g_NoDraw.IsEnabled() == true)
        {
            if (g_NoDraw.HandleCParticleSystemCreateSpellEmitter() == true)
            {
                return 0;
            }
        }
    }

    return EQAPP_REAL_FUNCTION_CParticleSystem__CreateSpellEmitter
    (
        thisPointer,
        index,
        arg2,
        arg3,
        particleDensity,
        particleOpacity,
        position,
        actor,
        bone,
        particlePoint,
        emitter,
        arg11,
        arg12,
        arg13,
        previewMode
    );
}

}
