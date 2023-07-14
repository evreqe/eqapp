module;

#include "common.h"

export module eq_classes;

export import eq;

export
{

namespace eq
{
    namespace Classes
    {
        class ChatManager;
        class Spawn;
        class SpawnManager;
        class CEverQuest;
        class CDisplay;
    }
}

//////////////////////////////////////////////////
/* ChatManager */
//////////////////////////////////////////////////

class eq::Classes::ChatManager
{
public:
    void PrintText(const char* text, uint32_t chatTextColor, bool isLogOK, bool doPercentConvert, char* nullString);
};

typedef void (__thiscall* EQ_FUNCTION_TYPE_ChatManager__PrintText)(void* thisPointer, const char* text, uint32_t chatTextColor, bool isLogOK, bool doPercentConvert, char* nullString);
void EQ_FUNCTION_ChatManager__PrintText(void* thisPointer, const char* text, uint32_t chatTextColor, bool isLogOK, bool doPercentConvert, char* nullString)
{
     ((EQ_FUNCTION_TYPE_ChatManager__PrintText)eq::EQGame::Addresses::Functions::ChatManager__PrintText)(thisPointer, text, chatTextColor, isLogOK, doPercentConvert, nullString);
}

//////////////////////////////////////////////////
/* Spawn */
//////////////////////////////////////////////////

class eq::Classes::Spawn
{
public:
    //
};

//////////////////////////////////////////////////
/* SpawnManager */
//////////////////////////////////////////////////

class eq::Classes::SpawnManager
{
public:
    uintptr_t GetSpawnByID(uint32_t spawnID);
    uintptr_t GetSpawnByName(const char* spawnName);
};

typedef uintptr_t (__thiscall* EQ_FUNCTION_TYPE_SpawnManager__GetSpawnByID)(void* thisPointer, uint32_t spawnID);
uintptr_t EQ_FUNCTION_SpawnManager__GetSpawnByID(void* thisPointer, uint32_t spawnID)
{
     return ((EQ_FUNCTION_TYPE_SpawnManager__GetSpawnByID)eq::EQGame::Addresses::Functions::SpawnManager__GetSpawnByID)(thisPointer, spawnID);
}

typedef uintptr_t (__thiscall* EQ_FUNCTION_TYPE_SpawnManager__GetSpawnByName)(void* thisPointer, const char* spawnName);
uintptr_t EQ_FUNCTION_SpawnManager__GetSpawnByName(void* thisPointer, const char* spawnName)
{
     return ((EQ_FUNCTION_TYPE_SpawnManager__GetSpawnByName)eq::EQGame::Addresses::Functions::SpawnManager__GetSpawnByName)(thisPointer, spawnName);
}

//////////////////////////////////////////////////
/* CEverQuest */
//////////////////////////////////////////////////

class eq::Classes::CEverQuest
{
public:
    char* DoPercentConvert(char* text, bool isOutgoing);
    void InterpretCommand(uintptr_t* playerSpawn, const char* text);
    void SetGameState(uint32_t gameState);
};

typedef char* (__thiscall* EQ_FUNCTION_TYPE_CEverQuest__DoPercentConvert)(void* thisPointer, char* text, bool isOutgoing);
char* EQ_FUNCTION_CEverQuest__DoPercentConvert(void* thisPointer, char* text, bool isOutgoing)
{
     return ((EQ_FUNCTION_TYPE_CEverQuest__DoPercentConvert)eq::EQGame::Addresses::Functions::CEverQuest__DoPercentConvert)(thisPointer, text, isOutgoing);
}

typedef void (__thiscall* EQ_FUNCTION_TYPE_CEverQuest__InterpretCommand)(void* thisPointer, uintptr_t* playerSpawn, const char* text);
void EQ_FUNCTION_CEverQuest__InterpretCommand(void* thisPointer, uintptr_t* playerSpawn, const char* text)
{
     ((EQ_FUNCTION_TYPE_CEverQuest__InterpretCommand)eq::EQGame::Addresses::Functions::CEverQuest__InterpretCommand)(thisPointer, playerSpawn, text);
}

typedef void (__thiscall* EQ_FUNCTION_TYPE_CEverQuest__SetGameState)(void* thisPointer, uint32_t gameState);

//////////////////////////////////////////////////
/* CDisplay */
//////////////////////////////////////////////////

class eq::Classes::CDisplay
{
public:
    //
};

//

}
