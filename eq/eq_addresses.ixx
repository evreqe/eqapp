module;

#include "common.h"

#include "mq.h"

export module eq_addresses;

export
{

namespace eq
{
    namespace EQGame
    {
        uintptr_t BaseAddress = (uintptr_t)GetModuleHandleA(nullptr);

        constexpr uintptr_t PreferredAddress = 0x140000000;

        uintptr_t FixAddress(uintptr_t address)
        {
            return address - PreferredAddress + BaseAddress;
        }

        namespace Addresses
        {
            uintptr_t VersionDate = FixAddress(__ActualVersionDate_x);
            uintptr_t VersionTime = FixAddress(__ActualVersionTime_x);

            uintptr_t WindowHandle = FixAddress(__HWnd_x);

            uintptr_t World = FixAddress(__gWorld_x);

            namespace Pointers
            {
                uintptr_t ControlledSpawn = FixAddress(pinstControlledPlayer_x);
                uintptr_t PlayerSpawn = FixAddress(pinstLocalPlayer_x);
                uintptr_t TargetSpawn = FixAddress(pinstTarget_x);

                uintptr_t SpawnManager = FixAddress(pinstSpawnManager_x);
                uintptr_t SoundManager = FixAddress(pinstEQSoundManager_x);

                uintptr_t CEverQuest = FixAddress(pinstCEverQuest_x);
                uintptr_t CDisplay = FixAddress(pinstCDisplay_x);

                uintptr_t CRender = FixAddress(pinstRenderInterface_x);
                uintptr_t SGraphicsEngine = FixAddress(pinstSGraphicsEngine_x);
            }

            namespace Functions
            {
                // non-member functions
                uintptr_t DrawNetStatus = FixAddress(DrawNetStatus_x); // draw the F11 text and bar in top left of screen showing ping
                uintptr_t ExecuteCommand = FixAddress(__ExecuteCmd_x); // execute functions that keys and slash commands are bound to
                uintptr_t GetTime = FixAddress(__EQGetTime_x); // uses GetTickCount() to calculate time that has passed
                uintptr_t GetBearing = FixAddress(__get_bearing_x); // calculates number of degrees to face towards another spawn
                uintptr_t GetMeleeRange = FixAddress(__get_melee_range_x);
                uintptr_t GetChatManager = FixAddress(ChatManagerClient__Instance_x);
                uintptr_t DrawText = FixAddress(CDisplay__WriteTextHD2_x);

                // ChatManager
                uintptr_t ChatManager__PrintText = FixAddress(CEverQuest__dsp_chat_x);

                // CharacterZoneClient
                uintptr_t CharacterZoneClient__GetCurrentHitPoints = FixAddress(CharacterZoneClient__Cur_HP_x);
                uintptr_t CharacterZoneClient__GetCurrentManaPoints = FixAddress(CharacterZoneClient__Cur_Mana_x);
                uintptr_t CharacterZoneClient__GetMaxHitPoints = FixAddress(CharacterZoneClient__Max_HP_x);
                uintptr_t CharacterZoneClient__GetMaxManaPoints = FixAddress(CharacterZoneClient__Max_Mana_x);

                // Spawn
                uintptr_t Spawn__GetCharacterZoneClient = FixAddress(PlayerClient__GetPcClient_x);
                uintptr_t Spawn__GetLevel = FixAddress(PlayerZoneClient__GetLevel_x);

                // SpawnManager
                uintptr_t SpawnManager__GetSpawnByID = FixAddress(PlayerManagerClient__GetSpawnByID_x);
                uintptr_t SpawnManager__GetSpawnByName = FixAddress(PlayerManagerClient__GetSpawnByName_x);

                // SoundManager
                uintptr_t SoundManager__PlaySound = FixAddress(EqSoundManager__WavePlay_x);

                // CEverQuest
                uintptr_t CEverQuest__DoPercentConvert = FixAddress(CEverQuest__DoPercentConvert_x);
                uintptr_t CEverQuest__InterpretCommand = FixAddress(CEverQuest__InterpretCmd_x);
                uintptr_t CEverQuest__SetGameState = FixAddress(CEverQuest__SetGameState_x);

                // CDisplay
                //
            }
        }
    }

    namespace EQGraphics
    {
        uintptr_t BaseAddress = (uintptr_t)GetModuleHandleA("EQGraphicsDX9.dll");

        constexpr uintptr_t PreferredAddress = 0x180000000;

        uintptr_t FixAddress(uintptr_t address)
        {
            return address - PreferredAddress + BaseAddress;
        }

        namespace Addresses
        {
            namespace Functions
            {
                // CParticleSystem
                uintptr_t CParticleSystem__Render = FixAddress(CParticleSystem__Render_x);
                uintptr_t CParticleSystem__CreateSpellEmitter = FixAddress(CParticleSystem__CreateSpellEmitter_x);

                // CRender
                uintptr_t CRender__RenderScene = FixAddress(CRender__RenderScene_x);
                uintptr_t CRender__RenderBlind = FixAddress(CRender__RenderBlind_x);
                uintptr_t CRender__ResetDevice = FixAddress(CRender__ResetDevice_x);
                uintptr_t CRender__UpdateDisplay = FixAddress(CRender__UpdateDisplay_x);
            }
        }
    }

    namespace EQMain
    {
        uintptr_t BaseAddress = (uintptr_t)GetModuleHandleA("eqmain.dll");

        constexpr uintptr_t PreferredAddress = 0x180000000;

        uintptr_t FixAddress(uintptr_t address)
        {
            return address - PreferredAddress + BaseAddress;
        }
    }
}

}
