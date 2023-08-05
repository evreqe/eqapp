module;

#include "common.h"

export module eq_constants;

export
{

namespace eq
{
    using SpawnList = std::vector<uintptr_t>;

    namespace Constants
    {
        const std::string GameProcessName = "eqgame.exe";

        constexpr float PI = std::numbers::pi_v<float>;

        constexpr float FloatInfinity = std::numeric_limits<float>::infinity();

        const uint32_t NumHotbars          = 11;
        const uint32_t NumHotbarButtons    = 12;

        namespace Spawn
        {
            namespace ID
            {
                const uint32_t Null    = 0xFFFFFFFF;
            }

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

            namespace Gender
            {
                const uint8_t Male       = 0;
                const uint8_t Female     = 1;
                const uint8_t Unknown    = 2;
            }

            namespace Race
            {
                const uint32_t Unknown         = 0;
                const uint32_t Human           = 1;
                const uint32_t Barbarian       = 2;
                const uint32_t Erudite         = 3;
                const uint32_t WoodElf         = 4;
                const uint32_t HighElf         = 5;
                const uint32_t DarkElf         = 6;
                const uint32_t HalfElf         = 7;
                const uint32_t Dwarf           = 8;
                const uint32_t Troll           = 9;
                const uint32_t Ogre            = 10;
                const uint32_t Halfling        = 11;
                const uint32_t Gnome           = 12;
                const uint32_t Skeleton        = 60;
                const uint32_t InvisibleMan    = 127;    // enchanter pets, auras, etc
                const uint32_t Iksar           = 128;
                const uint32_t VahShir         = 130;
                const uint32_t Froglok         = 330;
                const uint32_t Chokadai        = 356;
                const uint32_t Skeleton2       = 367;
                const uint32_t Skeleton3       = 484;
                const uint32_t Drakkin         = 522;
                const uint32_t Campfire        = 567;    // fellowship campfires, etc

                namespace Strings
                {
                    const std::unordered_map<uint32_t, std::string> LongName =
                    {
                        {Unknown,         "Unknown"},
                        {Human,           "Human"},
                        {Barbarian,       "Barbarian"},
                        {Erudite,         "Erudite"},
                        {WoodElf,         "Wood Elf"},
                        {HighElf,         "High Elf"},
                        {DarkElf,         "Dark Elf"},
                        {HalfElf,         "Half Elf"},
                        {Dwarf,           "Dwarf"},
                        {Troll,           "Troll"},
                        {Ogre,            "Ogre"},
                        {Halfling,        "Halfling"},
                        {Gnome,           "Gnome"},
                        {InvisibleMan,    "Invisible Man"},
                        {Iksar,           "Iksar"},
                        {VahShir,         "Vah Shir"},
                        {Froglok,         "Froglok"},
                        {Drakkin,         "Drakkin"},
                        {Campfire,        "Campfire"},
                    };

                    const std::unordered_map<uint32_t, std::string> ShortName =
                    {
                        {Unknown,         "UNK"},
                        {Human,           "HUM"},
                        {Barbarian,       "BAR"},
                        {Erudite,         "ERU"},
                        {WoodElf,         "ELF"},
                        {HighElf,         "HIE"},
                        {DarkElf,         "DEF"},
                        {HalfElf,         "HEF"},
                        {Dwarf,           "DWF"},
                        {Troll,           "TRL"},
                        {Ogre,            "OGR"},
                        {Halfling,        "HFL"},
                        {Gnome,           "GNM"},
                        {InvisibleMan,    "IVM"},
                        {Iksar,           "IKS"},
                        {VahShir,         "VAH"},
                        {Froglok,         "FRG"},
                        {Drakkin,         "DRK"},
                        {Campfire,        "CMP"},
                    };
                }
            }

            namespace Class
            {
                const uint32_t Unknown                = 0;
                const uint32_t Warrior                = 1;
                const uint32_t Cleric                 = 2;
                const uint32_t Paladin                = 3;
                const uint32_t Ranger                 = 4;
                const uint32_t ShadowKnight           = 5;
                const uint32_t Druid                  = 6;
                const uint32_t Monk                   = 7;
                const uint32_t Bard                   = 8;
                const uint32_t Rogue                  = 9;
                const uint32_t Shaman                 = 10;
                const uint32_t Necromancer            = 11;
                const uint32_t Wizard                 = 12;
                const uint32_t Magician               = 13;
                const uint32_t Enchanter              = 14;
                const uint32_t Beastlord              = 15;
                const uint32_t Berserker              = 16;

                const uint32_t Mercenary              = 17;

                const uint32_t WarriorGM              = 20;    // GM = Guildmaster
                const uint32_t ClericGM               = 21;
                const uint32_t PaladinGM              = 22;
                const uint32_t RangerGM               = 23;
                const uint32_t ShadowKnightGM         = 24;
                const uint32_t DruidGM                = 25;
                const uint32_t MonkGM                 = 26;
                const uint32_t BardGM                 = 27;
                const uint32_t RogueGM                = 28;
                const uint32_t ShamanGM               = 29;
                const uint32_t NecromancerGM          = 30;
                const uint32_t WizardGM               = 31;
                const uint32_t MagicianGM             = 32;
                const uint32_t EnchanterGM            = 33;
                const uint32_t BeastlordGM            = 34;
                const uint32_t BerserkerGM            = 35;

                const uint32_t Banker                 = 40;
                const uint32_t Merchant               = 41;

                const uint32_t Object                 = 62;    // auras, controllers, crates, tents, corpses, etc

                const uint32_t GuildBanker            = 66;
                const uint32_t FellowshipRegistrar    = 69;
                const uint32_t FactionMerchant        = 70;
                const uint32_t MercenaryLiaison       = 71;
                const uint32_t RealEstateMerchant     = 72;
                const uint32_t LoyaltyMerchant        = 73;
                const uint32_t TributeMaster          = 74;

                namespace Strings
                {
                    const std::unordered_map<uint32_t, std::string> LongName =
                    {
                        {Unknown,                "Unknown"},
                        {Warrior,                "Warrior"},
                        {Cleric,                 "Cleric"},
                        {Paladin,                "Paladin"},
                        {Ranger,                 "Ranger"},
                        {ShadowKnight,           "Shadow Knight"},
                        {Druid,                  "Druid"},
                        {Monk,                   "Monk"},
                        {Bard,                   "Bard"},
                        {Rogue,                  "Rogue"},
                        {Shaman,                 "Shaman"},
                        {Necromancer,            "Necromancer"},
                        {Wizard,                 "Wizard"},
                        {Magician,               "Magician"},
                        {Enchanter,              "Enchanter"},
                        {Beastlord,              "Beastlord"},
                        {Berserker,              "Berserker"},
                                                 
                        {Mercenary,              "Mercenary"},
                                                 
                        {Banker,                 "Banker"},
                        {Merchant,               "Merchant"},

                        {Object,                 "Object"},
                        {GuildBanker,            "Guild Banker"},
                        {FellowshipRegistrar,    "Fellowship Registrar"},
                        {FactionMerchant,        "Faction Merchant"},
                        {MercenaryLiaison,       "Mercenary Liaison"},
                        {RealEstateMerchant,     "Real Estate Merchant"},
                        {LoyaltyMerchant,        "Loyalty Merchant"},
                        {TributeMaster,          "Tribute Master"},
                    };

                    const std::unordered_map<uint32_t, std::string> ShortName =
                    {
                        {Unknown,         "UNK"},
                        {Warrior,         "WAR"},
                        {Cleric,          "CLR"},
                        {Paladin,         "PAL"},
                        {Ranger,          "RNG"},
                        {ShadowKnight,    "SHD"},
                        {Druid,           "DRU"},
                        {Monk,            "MNK"},
                        {Bard,            "BRD"},
                        {Rogue,           "ROG"},
                        {Shaman,          "SHM"},
                        {Necromancer,     "NEC"},
                        {Wizard,          "WIZ"},
                        {Magician,        "MAG"},
                        {Enchanter,       "ENC"},
                        {Beastlord,       "BST"},
                        {Berserker,       "BER"},

                        {Mercenary,       "MCN"},

                        {Banker,          "BNK"},
                        {Merchant,        "MCT"},

                        {Object,          "OBJ"},
                    };
                }
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

        namespace ColorARGB
        {
            const uint32_t Red          = 0xFFFF0000;
            const uint32_t Orange       = 0xFFFF8000;
            const uint32_t Yellow       = 0xFFFFFF00;
            const uint32_t Green        = 0xFF00FF00;
            const uint32_t Blue         = 0xFF0000FF;
            const uint32_t Purple       = 0xFF8000FF;
            const uint32_t Pink         = 0xFFFF80FF;
            const uint32_t Magenta      = 0xFFFF00FF;
            const uint32_t Teal         = 0xFF00FFFF;
            const uint32_t Brown        = 0xFF804000;
            const uint32_t Jade         = 0xFF00FF80;
            const uint32_t Gray         = 0xFF808080;
            const uint32_t Silver       = 0xFFC0C0C0;
            const uint32_t White        = 0xFFFFFFFF;
            const uint32_t Black        = 0xFF000000;
            const uint32_t DarkRed      = 0xFF800000;
            const uint32_t DarkGreen    = 0xFF008000;
            const uint32_t DarkBlue     = 0xFF000080;
            const uint32_t ToolTip      = 0xC8000040;
        }

        namespace CCamera
        {
            namespace FieldOfView
            {
                const float Default      = 45.0f;
                const float DruidBuff    = 60.0f;    // Spell: Mask of the Hunter, Spell: Mask of the Forest, etc
            }

            namespace Pitch
            {
                const float Default    = -8.5f;      // looking forward, view is centered
                const float Min        = -136.5f;    // looking all the way down
                const float Max        = 119.5f;     // looking all the way up
            }
        }
    }
}

}
