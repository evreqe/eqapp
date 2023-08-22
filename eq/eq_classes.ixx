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
        class CXWndManager;
        class ChatManager;
        class Spawn;
        class SpawnManager;
        class SoundManager;
        class CEverQuest;
        class CDisplay;
        class CCamera;
        class CRender;
        class CParticleSystem;
    }
}

//////////////////////////////////////////////////
/* CXWndManager */
//////////////////////////////////////////////////

class eq::Classes::CXWndManager
{
public:
    int DrawCursor();
    int DrawWindows();
};

typedef int (__thiscall* EQ_FUNCTION_TYPE_CXWndManager__DrawCursor)(void* thisPointer);
int EQ_FUNCTION_CXWndManager__DrawCursor(void* thisPointer)
{
    return ((EQ_FUNCTION_TYPE_CXWndManager__DrawCursor)eq::EQGame::Addresses::Functions::CXWndManager__DrawCursor)(thisPointer);
}

typedef int (__thiscall* EQ_FUNCTION_TYPE_CXWndManager__DrawWindows)(void* thisPointer);
int EQ_FUNCTION_CXWndManager__DrawWindows(void* thisPointer)
{
    return ((EQ_FUNCTION_TYPE_CXWndManager__DrawWindows)eq::EQGame::Addresses::Functions::CXWndManager__DrawWindows)(thisPointer);
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
    int GetLevel();
};

typedef int (__thiscall* EQ_FUNCTION_TYPE_Spawn__GetLevel)(void* thisPointer);
int EQ_FUNCTION_Spawn__GetLevel(void* thisPointer)
{
    return ((EQ_FUNCTION_TYPE_Spawn__GetLevel)eq::EQGame::Addresses::Functions::Spawn__GetLevel)(thisPointer);
}

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
/* SoundManager */
//////////////////////////////////////////////////

class eq::Classes::SoundManager
{
public:
    void PlaySound(int soundID, void* soundControl);
};

typedef void (__thiscall* EQ_FUNCTION_TYPE_SoundManager__PlaySound)(void* thisPointer, int soundID, void* soundControl);
void EQ_FUNCTION_SoundManager__PlaySound(void* thisPointer, int soundID, void* soundControl)
{
    return ((EQ_FUNCTION_TYPE_SoundManager__PlaySound)eq::EQGame::Addresses::Functions::SoundManager__PlaySound)(thisPointer, soundID, soundControl);
}

//////////////////////////////////////////////////
/* CEverQuest */
//////////////////////////////////////////////////

class eq::Classes::CEverQuest
{
public:
    char* DoPercentConvert(char* text, bool isOutgoing);
    void InterpretCommand(uintptr_t* playerSpawn, const char* text);
    void SetGameState(int gameState);
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

typedef void (__thiscall* EQ_FUNCTION_TYPE_CEverQuest__SetGameState)(void* thisPointer, int gameState);
void EQ_FUNCTION_CEverQuest__SetGameState(void* thisPointer, int gameState)
{
    ((EQ_FUNCTION_TYPE_CEverQuest__SetGameState)eq::EQGame::Addresses::Functions::CEverQuest__SetGameState)(thisPointer, gameState);
}

//////////////////////////////////////////////////
/* CDisplay */
//////////////////////////////////////////////////

class eq::Classes::CDisplay
{
public:
    //
};

//////////////////////////////////////////////////
/* CCamera */
//////////////////////////////////////////////////

class eq::Classes::CCamera
{
public:
    int SetLocation(eq::Location& location, bool canSetLocation);
    int SetOrientation(eq::Orientation& orientation);
    bool WorldSpaceToScreenSpace(eq::Location& location, float& screenX, float& screenY);
};

typedef int (__thiscall* EQ_FUNCTION_TYPE_CCamera__SetLocation)(void* thisPointer, eq::Location& location, bool canSetLocation);
typedef int (__thiscall* EQ_FUNCTION_TYPE_CCamera__SetOrientation)(void* thisPointer, eq::Orientation& orientation);
typedef bool (__thiscall* EQ_FUNCTION_TYPE_CCamera__WorldSpaceToScreenSpace)(void* thisPointer, eq::Location& location, float& screenX, float& screenY);

//////////////////////////////////////////////////
/* CRender */
//////////////////////////////////////////////////

class eq::Classes::CRender
{
public:
    int ResetDevice(bool unknown);
    void DrawLine2D(const eq::Vector3f& vector1, const eq::Vector3f& vector2, uint32_t colorARGB);
    void DrawLine3D(const eq::Vector3f& vector1, const eq::Vector3f& vector2, uint32_t colorARGB);
    void RenderScene();
    void RenderBlind();
    void UpdateDisplay();
};

typedef int (__thiscall* EQ_FUNCTION_TYPE_CRender__ResetDevice)(void* thisPointer, bool unknown);
void EQ_FUNCTION_CRender__ResetDevice(void* thisPointer, bool unknown)
{
    ((EQ_FUNCTION_TYPE_CRender__ResetDevice)eq::EQGraphics::Addresses::Functions::CRender__ResetDevice)(thisPointer, unknown);
}

typedef void (__thiscall* EQ_FUNCTION_TYPE_CRender__DrawLine2D)(void* thisPointer, const eq::Vector3f& vector1, const eq::Vector3f& vector2, uint32_t colorARGB);
typedef void (__thiscall* EQ_FUNCTION_TYPE_CRender__DrawLine3D)(void* thisPointer, const eq::Vector3f& vector1, const eq::Vector3f& vector2, uint32_t colorARGB);

typedef void (__thiscall* EQ_FUNCTION_TYPE_CRender__RenderScene)(void* thisPointer);
void EQ_FUNCTION_CRender__RenderScene(void* thisPointer)
{
    ((EQ_FUNCTION_TYPE_CRender__RenderScene)eq::EQGraphics::Addresses::Functions::CRender__RenderScene)(thisPointer);
}

typedef void (__thiscall* EQ_FUNCTION_TYPE_CRender__RenderBlind)(void* thisPointer);
void EQ_FUNCTION_CRender__RenderBlind(void* thisPointer)
{
    ((EQ_FUNCTION_TYPE_CRender__RenderBlind)eq::EQGraphics::Addresses::Functions::CRender__RenderBlind)(thisPointer);
}

typedef void (__thiscall* EQ_FUNCTION_TYPE_CRender__UpdateDisplay)(void* thisPointer);
void EQ_FUNCTION_CRender__UpdateDisplay(void* thisPointer)
{
    ((EQ_FUNCTION_TYPE_CRender__UpdateDisplay)eq::EQGraphics::Addresses::Functions::CRender__UpdateDisplay)(thisPointer);
}

//////////////////////////////////////////////////
/* CParticleSystem */
//////////////////////////////////////////////////

class eq::Classes::CParticleSystem
{
public:
    int CreateSpellEmitter
    (
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
};

typedef int (__thiscall* EQ_FUNCTION_TYPE_CParticleSystem__CreateSpellEmitter)
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
void EQ_FUNCTION_CParticleSystem__CreateSpellEmitter
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
    ((EQ_FUNCTION_TYPE_CParticleSystem__CreateSpellEmitter)eq::EQGraphics::Addresses::Functions::CParticleSystem__CreateSpellEmitter)
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
