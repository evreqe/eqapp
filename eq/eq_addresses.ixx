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

        void FixAddressEx(uintptr_t& address)
        {
            address = address - PreferredAddress + BaseAddress;
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
                uintptr_t CEverQuest = FixAddress(pinstCEverQuest_x);
                uintptr_t CDisplay = FixAddress(pinstCDisplay_x);
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

                // SpawnManager
                uintptr_t SpawnManager__GetSpawnByID = FixAddress(PlayerManagerClient__GetSpawnByID_x);
                uintptr_t SpawnManager__GetSpawnByName = FixAddress(PlayerManagerClient__GetSpawnByName_x);

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

        void FixAddressEx(uintptr_t& address)
        {
            address = address - PreferredAddress + BaseAddress;
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

        void FixAddressEx(uintptr_t& address)
        {
            address = address - PreferredAddress + BaseAddress;
        }
    }
}

}
