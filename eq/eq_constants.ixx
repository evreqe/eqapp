module;

#include "common.h"

export module eq_constants;

export
{

namespace eq
{
    namespace Constants
    {
        const char* WindowTitleDefault     = "EverQuest";
        const char* WindowTitleModified    = "EverQuest*";

        constexpr float PI = std::numbers::pi_v<float>;

        const uint32_t NumHotbars          = 11;
        const uint32_t NumHotbarButtons    = 12;

        namespace Spawn
        {
            namespace Type
            {
                const uint32_t Player     = 0;
                const uint32_t NPC        = 1;
                const uint32_t Corpse     = 2;
                const uint32_t Unknown    = 3;    // custom value for our use

                const std::unordered_map<uint32_t, std::string> Strings =
                {
                    {Player,     "Player"},
                    {NPC,        "NPC"},
                    {Corpse,     "Corpse"},
                    {Unknown,    "Unknown"},
                };
            }

            namespace Pitch
            {
                const float Default    = 0.0f;       // looking forward, view is centered
                const float Min        = -128.0f;    // looking all the way down
                const float Max        = 128.0f;     // looking all the way up
            }

            namespace HeadingSpeed
            {
                const float Default    = 0.0f;      // not turning
                const float Min        = -12.0f;    // turning right
                const float Max        = 12.0f;     // turning left
            }

            namespace AreaFriction
            {
                const float Default     = 0.625f;
                const float Slippery    = 0.9900000095f;    // ice or slime is on the floor making the player slide around
            }

            namespace AccelerationFriction
            {
                const float Default     = 0.8000000119f;
                const float Slippery    = 0.01999999955f;    // ice or slime is on the floor making the player slide around
            }
        }

        namespace CActorInterface
        {
            namespace Type
            {
                const uint32_t Unknown          = 0;
                const uint32_t Player           = 1;
                const uint32_t Corpse           = 2;
                const uint32_t Switch           = 3;    // doors are switches
                const uint32_t Missile          = 4;
                const uint32_t Object           = 5;
                const uint32_t Ladder           = 6;
                const uint32_t Tree             = 7;
                const uint32_t Wall             = 8;    // may also be a tree
                const uint32_t PlacedObject     = 9;
            }
        }

        namespace Heading
        {
            const float Min           = 0.0f;
            const float Max           = 512.0f;
            const float MaxHalf       = 256.0f;
            const float MaxQuarter    = 128.0f;
            const float MaxEighth     = 64.0f;

            const float North         = 0.0f;
            const float NorthWest     = 64.0f;
            const float West          = 128.0f;
            const float SouthWest     = 192.0f;
            const float South         = 256.0f;
            const float SouthEast     = 320.0f;
            const float East          = 384.0f;
            const float NorthEast     = 448.0f;
        }

        namespace Direction
        {
            const uint32_t North        = 0;
            const uint32_t NorthWest    = 1;
            const uint32_t NorthEast    = 2;
            const uint32_t South        = 3;
            const uint32_t SouthWest    = 4;
            const uint32_t SouthEast    = 5;
            const uint32_t West         = 6;
            const uint32_t East         = 7;
            const uint32_t Unknown      = 8;

            const std::unordered_map<uint32_t, std::string> Strings =
            {
                {North,        "North"},
                {NorthWest,    "North West"},
                {NorthEast,    "North East"},
                {South,        "South"},
                {SouthWest,    "South West"},
                {SouthEast,    "South East"},
                {West,         "West"},
                {East,         "East"},
                {Unknown,      "Unknown"},
            };
        }

        // found in get_melee_range() function
        namespace MeleeRange
        {
            const float Min    = 14.0f;
            const float Max    = 75.0f;
        }

        namespace JumpStrength
        {
            const float Default    = 1.65f;
        }

        // slider in options menu
        namespace FarClipPlane
        {
            const uint32_t Min    = 0;
            const uint32_t Max    = 20;
        }

        namespace MouseWheelDelta
        {
            const signed int Default    = 0;       // not scrolling
            const signed int Min        = -120;    // scrolling down
            const signed int Max        = 120;     // scrolling up
        }

        namespace GameState
        {
            const uint32_t CharacterSelect    = 1;      // at character select
            const uint32_t Loading2           = 4;      // after character select, going to in game
            const uint32_t InGame             = 5;      // in game
            const uint32_t Loading            = 6;      // after server select, going to character select
            const uint32_t Loading3           = 253;    // right before going to in game
            const uint32_t Null               = 0xFFFFFFFF;
        }

        namespace CameraView
        {
            const uint32_t FirstPerson     = 0;
            const uint32_t Overhead        = 1;
            const uint32_t Chase           = 2;
            const uint32_t UserDefined1    = 3;
            const uint32_t UserDefined2    = 4;
            const uint32_t Tether          = 5;
            const uint32_t MouseScroll     = 6;    // mouse wheel scroll out to third person
            const uint32_t Front           = 7;    // when you change your appearance or dye your armor
        }

        namespace EnvironmentType
        {
            const uint8_t Water    = 5;    // uint8_t
        }

        // used with CEverQuest__PrintText() when printing text to the chat window
        namespace ChatTextColor
        {
            const uint32_t White0       = 0;
            const uint32_t Default      = 1;
            const uint32_t DarkGreen    = 2;
            const uint32_t Default2     = 3;
            const uint32_t DarkBlue     = 4;
            const uint32_t Pink         = 5;
            const uint32_t DarkGray     = 6;
            const uint32_t White2       = 7;
            const uint32_t Default3     = 8;
            const uint32_t Default4     = 9;
            const uint32_t White        = 10;
            const uint32_t Default5     = 11;
            const uint32_t Gray         = 12;
            const uint32_t Red          = 13;
            const uint32_t Green        = 14;
            const uint32_t Yellow       = 15;
            const uint32_t Blue         = 16;
            const uint32_t Blue2        = 17;
            const uint32_t Teal         = 18;
            const uint32_t Default6     = 19;
            const uint32_t White20      = 20;
            const uint32_t Orange       = 21;
            const uint32_t Brown        = 22;
        }

        // used with CDisplay__DrawText() when drawing text on the screen
        namespace DrawTextColor
        {
            const uint32_t Black        = 0;     // ARGB 0xFF000000
            const uint32_t Default      = 1;     // ARGB 0xFF606060
            const uint32_t DarkGreen    = 2;     // ARGB 0xFF008000
            const uint32_t Default2     = 3;     // ARGB 0xFF606060
            const uint32_t DarkBlue     = 4;     // ARGB 0xFF000080
            const uint32_t Pink         = 5;     // ARGB 0xFFF000F0
            const uint32_t DarkGray     = 6;     // ARGB 0xFF808080
            const uint32_t White2       = 7;     // ARGB 0xFFE0E0E0
            const uint32_t Default3     = 8;     // ARGB 0xFF606060
            const uint32_t Default4     = 9;     // ARGB 0xFF606060
            const uint32_t White        = 10;    // ARGB 0xFFF0F0F0
            const uint32_t Default5     = 11;    // ARGB 0xFF606060
            const uint32_t Gray         = 12;    // ARGB 0xFFA0A0A0
            const uint32_t Red          = 13;    // ARGB 0xFFF00000
            const uint32_t Green        = 14;    // ARGB 0xFF00F000
            const uint32_t Yellow       = 15;    // ARGB 0xFFF0F000
            const uint32_t Blue         = 16;    // ARGB 0xFF0000F0
            const uint32_t Blue2        = 17;    // ARGB 0xFF0000AF
            const uint32_t Teal         = 18;    // ARGB 0xFF00F0F0
            const uint32_t Default6     = 19;    // ARGB 0xFF606060
            const uint32_t Black2       = 20;    // ARGB 0xFF000000
        }
    }
}

}
