module;

#include "common.h"

export module eq_offsets;

export
{

namespace eq
{
    namespace Offsets
    {
        namespace CEverQuest
        {
            const uint32_t GameState    = 0x5E4;    // uint32_t    // "Gamestate at crash = %d\n"
        }

        namespace SpawnManager
        {
            const uint32_t FirstSpawn    = 0x10;    // uint32_t pointer
            const uint32_t LastSpawn     = 0x18;    // uint32_t pointer
        }

        namespace Spawn
        {
            const uint32_t Previous                          = 0x04;      // uintptr_t pointer
            const uint32_t Next                              = 0x08;      // uintptr_t pointer
            const uint32_t JumpStrength                      = 0x20;      // float       // determines how high up you will jump upward, but not forward
            const uint32_t SwimStrength                      = 0x24;      // float
            const uint32_t SpeedMultiplier                   = 0x28;      // float       // spells like SoW
            const uint32_t AreaFriction                      = 0x2C;      // float       // sliding on slippery surfaces or walking up slopes
            const uint32_t AccelerationFriction              = 0x30;      // float       // sliding on slippery surfaces or walking up slopes
            const uint32_t CollideWithCActorInterfaceType    = 0x34;      // uint32_t
            const uint32_t FloorZ                            = 0x38;      // float       // Z-axis value where feet touch the ground
            const uint32_t Timer                             = 0x40;      // uint32_t    // timestamp
            const uint32_t LastName                          = 0x48;      // char[32]
            const uint32_t Y                                 = 0x74;      // float
            const uint32_t X                                 = 0x78;      // float
            const uint32_t Z                                 = 0x7C;      // float
            const uint32_t SpeedX                            = 0x70;      // float
            const uint32_t SpeedY                            = 0x74;      // float
            const uint32_t SpeedZ                            = 0x78;      // float
            const uint32_t MovementSpeed                     = 0x8C;      // float       // how fast you are moving while walking, running, riding a mount, etc
            const uint32_t Heading                           = 0x90;      // float       // turning
            const uint32_t Angle                             = 0x94;      // float
            const uint32_t AccelerationAngle                 = 0x98;      // float
            const uint32_t HeadingSpeed                      = 0x9C;      // float       // turning speed, -12 to 12
            const uint32_t Pitch                             = 0xA0;      // float       // look down and up, -128 to 128    // look forward, 0
            const uint32_t LastHeadEnvironmentType           = 0xA4;      // uint32_t     // touching water or lava, etc
            const uint32_t LastBodyEnvironmentType           = 0xA8;      // uint32_t
            const uint32_t LastFeetEnvironmentType           = 0xAC;      // uint32_t
            const uint32_t HeadEnvironmentType               = 0xB0;      // uint8_t
            const uint32_t FeetEnvironmentType               = 0xB1;      // uint8_t
            const uint32_t BodyEnvironmentType               = 0xB2;      // uint8_t
            const uint32_t BodyEnvironmentType2              = 0xB3;      // uint8_t
            const uint32_t NameNumbered                      = 0xB4;      // char[64]
            const uint32_t Name                              = 0xF4;      // char[64]
            const uint32_t Type                              = 0x135;     // uint8_t
            const uint32_t HeightZ                           = 0x150;     // float       // Z-axis value of the height of the player
            const uint32_t Height                            = 0x154;     // float
            const uint32_t Width                             = 0x158;     // float
            const uint32_t Length                            = 0x15C;     // float
            const uint32_t ID                                = 0x168;     // uint32_t
            const uint32_t State                             = 0x16C;     // uint32_t
            const uint32_t VehicleSpawn                      = 0x170;     // uintptr_t    // boats, airships, etc
            const uint32_t MountSpawn                        = 0x178;     // uintptr_t    // horses, etc
            const uint32_t MountRiderSpawn                   = 0x180;     // uintptr_t    // spawn that is riding the mount
            const uint32_t IsTargetable                      = 0x18C;     // uint8_t
            const uint32_t IsClickThrough                    = 0x18E;     // uint8_t

            namespace SizeOf
            {
                const uint32_t Name               = 0x40;    // char[64]
                const uint32_t NameNumbered       = 0x40;    // char[64]
                const uint32_t LastName           = 0x20;    // char[32]
                const uint32_t ActorDefinition    = 0x40;    // char[64]
            }
        }
    }
}

}
