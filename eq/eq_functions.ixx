module;

#include "common.h"

export module eq_functions;

export import eq;
export import eq_classes;

export import eq_functions_h;

export
{

//////////////////////////////////////////////////
/* non-member functions */
//////////////////////////////////////////////////

typedef void (* EQ_FUNCTION_TYPE_DrawNetStatus)(uint32_t x, uint32_t y, uintptr_t worldPointer);

typedef bool (* EQ_FUNCTION_TYPE_ExecuteCommand)(uint32_t commandID, bool keyDown, void* data, void* keyCombo);
bool EQ_FUNCTION_ExecuteCommand(uint32_t commandID, bool keyDown, void* data, void* keyCombo)
{
    return ((EQ_FUNCTION_TYPE_ExecuteCommand)eq::EQGame::Addresses::Functions::ExecuteCommand)(commandID, keyDown, data, keyCombo);
}

typedef uintptr_t (* EQ_FUNCTION_TYPE_GetChatManager)();
uintptr_t EQ_FUNCTION_GetChatManager()
{
    return ((EQ_FUNCTION_TYPE_GetChatManager)eq::EQGame::Addresses::Functions::GetChatManager)();
}

typedef uintptr_t (* EQ_FUNCTION_TYPE_DrawText)(const char* text, uint32_t x, uint32_t y, uint32_t drawTextColor);
uintptr_t EQ_FUNCTION_DrawText(const char* text, uint32_t x, uint32_t y, uint32_t drawTextColor)
{
     return ((EQ_FUNCTION_TYPE_DrawText)eq::EQGame::Addresses::Functions::DrawText)(text, x, y, drawTextColor);
}

//////////////////////////////////////////////////
/* functions */
//////////////////////////////////////////////////

void EQ_ToggleBool(bool& b)
{
    b = !b;
}

void EQ_OutputFiles()
{
    EQ_InterpretCommand("/outputfile achievements");
    EQ_InterpretCommand("/outputfile faction");
    EQ_InterpretCommand("/outputfile guild");
    EQ_InterpretCommand("/outputfile guildbank");
    EQ_InterpretCommand("/outputfile guildhall");
    EQ_InterpretCommand("/outputfile inventory");
    EQ_InterpretCommand("/outputfile missingspells");
    EQ_InterpretCommand("/outputfile raid");
    EQ_InterpretCommand("/outputfile realestate");
    EQ_InterpretCommand("/outputfile spellbook");
}

float EQ_CalculateDistance(float y1, float x1, float y2, float x2)
{
    return std::sqrtf(std::powf(x2 - x1, 2) + std::powf(y2 - y1, 2));
}

float EQ_CalculateDistance3D(float y1, float x1, float z1, float y2, float x2, float z2)
{
    return std::sqrtf(std::powf(x2 - x1, 2) + std::powf(y2 - y1, 2) + std::powf(z2 - z1, 2));
}

bool EQ_IsWithinDistance(float y1, float x1, float y2, float x2, float distance)
{
    return (std::powf(x2 - x1, 2) + std::powf(y2 - y1, 2)) <= std::powf(distance, 2);
}

bool EQ_IsWithinDistance3D(float y1, float x1, float z1, float y2, float x2, float z2, float distance)
{
    return (std::powf(x2 - x1, 2) + std::powf(y2 - y1, 2) + std::powf(z2 - z1, 2)) <= std::powf(distance, 2);
}

float EQ_GetBearing(float y1, float x1, float y2, float x2)
{
    float v4 = 0.0f;
    float v5 = 0.0f;
    float v6 = 0.0f;
    float v7 = 0.0f;
    float v8 = 0.0f;
    float v9 = 0.0f;
    float v10 = 0.0f;
    float v11 = 0.0f;
    float v13 = 0.0f;

    v4 = y2;
    v5 = std::fabsf(y1 - y2);
    v6 = x1;
    v7 = std::fabsf(x1 - x2);

    if (v5 < 0.0000009999999974752427f)
    {
        v5 = 0.0000009999999974752427f;
    }

    v8 = v7;
    v9 = v5;
    v10 = x2;

    v11 = std::atan2(v8, v9) * 180.0f * 0.3183099014828645f;

    if (y1 < v4)
    {
        v13 = y1;
    }
    else
    {
        if (x2 >= v6)
        {
            return (90.0f - v11 + 90.0f) * 511.5f * 0.0027777778f;
        }

        if (x2 <= v6)
        {
            return (v11 + 180.0f) * 511.5f * 0.0027777778f;
        }

        v13 = y1;
        v10 = x2;
    }

    float result = 0.0f;

    if (v13 > v4 || v10 > v6)
    {
        result = v11 * 511.5f * 0.0027777778f;
    }
    else
    {
        result = (90.0f - v11 + 270.0f) * 511.5f * 0.0027777778f;
    }

    return result;
}

float EQ_RoundHeading(float heading)
{
    if (heading == 0.0f)
    {
        return heading;
    }

    heading = std::roundf(heading / eq::Constants::Heading::MaxQuarter) * eq::Constants::Heading::MaxQuarter;

    return heading;
}

float EQ_FixHeading(float heading)
{
    if (heading < 0.0f)
    {
        heading = heading + eq::Constants::Heading::Max;
    }
    else if (heading > eq::Constants::Heading::Max)
    {
        heading = heading - eq::Constants::Heading::Max;
    }

    return heading;
}

float EQ_FixPitch(float pitch)
{
    if (pitch < eq::Constants::Spawn::Pitch::Min)
    {
        pitch = eq::Constants::Spawn::Pitch::Min;
    }
    else if (pitch > eq::Constants::Spawn::Pitch::Max)
    {
        pitch = eq::Constants::Spawn::Pitch::Max;
    }

    return pitch;
}

float EQ_GetRadians(float degrees)
{
    if (degrees == 0.0f)
    {
        return 0.0f;
    }

    return (degrees * eq::Constants::PI) / eq::Constants::Heading::MaxHalf;
}

float EQ_GetDegrees(float radians)
{
    if (radians == 0.0f)
    {
        return 0.0f;
    }

    return (radians * eq::Constants::Heading::MaxHalf) / eq::Constants::PI;
}

void EQ_ApplyForwardMovement(float& y, float& x, float heading, float distance)
{
    heading = heading + 128.0f;

    heading = EQ_FixHeading(heading);

    float headingRadians = EQ_GetRadians(heading);

    float addY = std::sinf(headingRadians);
    float addX = std::cosf(headingRadians);

    y += addY * distance;
    x -= addX * distance;
}

void EQ_ApplyBackwardMovement(float& y, float& x, float heading, float distance)
{
    heading = heading - 128.0f;

    heading = EQ_FixHeading(heading);

    float headingRadians = EQ_GetRadians(heading);

    float addY = std::sinf(headingRadians);
    float addX = std::cosf(headingRadians);

    y += addY * distance;
    x -= addX * distance;
}

void EQ_ApplyLeftwardMovement(float& y, float& x, float heading, float distance)
{
    //heading = heading + 0.0f;

    heading = EQ_FixHeading(heading);

    float headingRadians = EQ_GetRadians(heading);

    float addY = std::sinf(headingRadians);
    float addX = std::cosf(headingRadians);

    y -= addY * distance;
    x += addX * distance;
}

void EQ_ApplyRightwardMovement(float& y, float& x, float heading, float distance)
{
    heading = heading + 256.0f;

    heading = EQ_FixHeading(heading);

    float headingRadians = EQ_GetRadians(heading);

    float addY = std::sinf(headingRadians);
    float addX = std::cosf(headingRadians);

    y -= addY * distance;
    x += addX * distance;
}

glm::vec2 EQ_GetMidpoint2f(glm::vec2 point1, glm::vec2 point2)
{
    glm::vec2 midpoint;

    midpoint.y = std::midpoint(point1.y, point2.y);
    midpoint.x = std::midpoint(point1.x, point2.x);

    return midpoint;
}

glm::vec3 EQ_GetMidpoint3f(glm::vec3 point1, glm::vec3 point2)
{
    glm::vec3 midpoint;

    midpoint.y = std::midpoint(point1.y, point2.y);
    midpoint.x = std::midpoint(point1.x, point2.x);
    midpoint.z = std::midpoint(point1.z, point2.z);

    return midpoint;
}

uintptr_t EQ_GetCEverQuest()
{
    return eq::Memory::Read<uintptr_t>(eq::EQGame::Addresses::Pointers::CEverQuest);
}

uintptr_t EQ_GetCDisplay()
{
    return eq::Memory::Read<uintptr_t>(eq::EQGame::Addresses::Pointers::CDisplay);
}

uintptr_t EQ_GetChatManager()
{
    return EQ_FUNCTION_GetChatManager();
}

uintptr_t EQ_GetSpawnManager()
{
    return eq::Memory::Read<uintptr_t>(eq::EQGame::Addresses::Pointers::SpawnManager);
}

uint32_t EQ_GetGameState()
{
    uintptr_t everquest = eq::Memory::Read<uintptr_t>(eq::EQGame::Addresses::Pointers::CEverQuest);
    if (everquest == NULL)
    {
        return eq::Constants::GameState::Null;
    }

    return eq::Memory::Read<uint32_t>(everquest + eq::Offsets::CEverQuest::GameState);
}

bool EQ_IsInGame()
{
    return EQ_GetGameState() == eq::Constants::GameState::InGame;
}

bool EQ_HasTimeElapsed(uint32_t& timer, uint32_t& timerInterval)
{
    uint32_t currentTimer = (uint32_t)GetTickCount();

    if ((currentTimer - timer) > timerInterval)
    {
        timer = currentTimer;
        return true;
    }

    return false;
}

HWND EQ_GetWindow()
{
    return eq::Memory::Read<HWND>(eq::EQGame::Addresses::WindowHandle);
}

bool EQ_IsWindowInBackground()
{
    HWND eqWindow = EQ_GetWindow();
    if (eqWindow == NULL)
    {
        return true;
    }

    HWND foregroundWindow = GetForegroundWindow();
    if (foregroundWindow == NULL)
    {
        return true;
    }

    return eqWindow != foregroundWindow;
}

POINT EQ_GetMousePosition()
{
    HWND window = EQ_GetWindow();

    POINT point;
    GetCursorPos(&point);

    ScreenToClient(window, &point);

    return point;
}

std::vector<uintptr_t> EQ_GetSpawnList()
{
    std::vector<uintptr_t> spawnList;
    spawnList.reserve(4096);

    uintptr_t spawn = EQ_GetFirstSpawn();
    while (spawn != NULL)
    {
        spawnList.push_back(spawn);

        spawn = EQ_GetSpawnNext(spawn);
    }

    return spawnList;
}

bool EQ_DoesSpawnExist(uint64_t spawn)
{
    if (spawn == NULL)
    {
        return false;
    }

    uintptr_t currentSpawn = EQ_GetFirstSpawn();
    while (currentSpawn != NULL)
    {
        if (currentSpawn == spawn)
        {
            return true;
        }

        currentSpawn = EQ_GetSpawnNext(currentSpawn);
    }

    return false;
}

uint64_t EQ_GetNumSpawnsInZone(uint8_t spawnType)
{
    uint64_t numSpawns = 0;

    uintptr_t spawn = EQ_GetFirstSpawn();
    while (spawn != NULL)
    {
        uint8_t spawnTypeEx = EQ_GetSpawnType(spawn);
        if (spawnTypeEx != spawnType)
        {
            spawn = EQ_GetSpawnNext(spawn);
            continue;
        }

        numSpawns++;

        spawn = EQ_GetSpawnNext(spawn);
    }

    return numSpawns;
}

uint64_t EQ_GetNumNearbySpawns(uint8_t spawnType, float distance, float distanceZ)
{
    uint64_t numSpawns = 0;

    uintptr_t playerSpawn = EQ_GetPlayerSpawn();
    if (playerSpawn == NULL)
    {
        return numSpawns;
    }

    float playerSpawnY = EQ_GetSpawnY(playerSpawn);
    float playerSpawnX = EQ_GetSpawnX(playerSpawn);
    float playerSpawnZ = EQ_GetSpawnZ(playerSpawn);

    uintptr_t spawn = EQ_GetFirstSpawn();
    while (spawn != NULL)
    {
        if (spawn == playerSpawn)
        {
            spawn = EQ_GetSpawnNext(spawn);
            continue;
        }

        float spawnY = EQ_GetSpawnY(spawn);
        float spawnX = EQ_GetSpawnX(spawn);
        float spawnZ = EQ_GetSpawnZ(spawn);

        uint8_t spawnType_ = EQ_GetSpawnType(spawn);
        if (spawnType_ != spawnType)
        {
            spawn = EQ_GetSpawnNext(spawn);
            continue;
        }

        float spawnDistance = EQ_CalculateDistance(playerSpawnY, playerSpawnX, spawnY, spawnX);
        if (spawnDistance > distance)
        {
            spawn = EQ_GetSpawnNext(spawn);
            continue;
        }

        float spawnDistanceZ = std::fabsf(playerSpawnZ - spawnZ);
        if (spawnDistanceZ > distanceZ)
        {
            spawn = EQ_GetSpawnNext(spawn);
            continue;
        }

        numSpawns++;

        spawn = EQ_GetSpawnNext(spawn);
    }

    return numSpawns;
}

uintptr_t EQ_GetSpawnByID(uint32_t spawnID)
{
/*
    uintptr_t* spawnPointer = EQ_CLASS_POINTER_SpawnManager->GetSpawnByID(spawnID);
    if (spawnPointer == NULL)
    {
        return NULL;
    }

    uintptr_t spawn = (uintptr_t)*&spawnPointer;
    if (spawn == NULL)
    {
        return NULL;
    }
*/

    uintptr_t spawnManager = EQ_GetSpawnManager();
    if (spawnManager == NULL)
    {
        return NULL;
    }

    uintptr_t spawn = EQ_FUNCTION_SpawnManager__GetSpawnByID((uintptr_t*)spawnManager, spawnID);
    if (spawn == NULL)
    {
        return NULL;
    }

    return spawn;
}

uintptr_t EQ_GetSpawnByName(const char* spawnName)
{
/*
    uintptr_t* spawnPointer = EQ_CLASS_POINTER_SpawnManager->GetSpawnByName(spawnName);
    if (spawnPointer == NULL)
    {
        return NULL;
    }

    uintptr_t spawn = (uintptr_t)*&spawnPointer;
    if (spawn == NULL)
    {
        return NULL;
    }
*/

    uintptr_t spawnManager = EQ_GetSpawnManager();
    if (spawnManager == NULL)
    {
        return NULL;
    }

    uintptr_t spawn = EQ_FUNCTION_SpawnManager__GetSpawnByName((uintptr_t*)spawnManager, spawnName);
    if (spawn == NULL)
    {
        return NULL;
    }

    return spawn;
}

uintptr_t EQ_GetFirstSpawn()
{
    uintptr_t spawnManager = eq::Memory::Read<uintptr_t>(eq::EQGame::Addresses::Pointers::SpawnManager);
    if (spawnManager == NULL)
    {
        return NULL;
    }

    return eq::Memory::Read<uintptr_t>(spawnManager + eq::Offsets::SpawnManager::FirstSpawn);
}

uintptr_t EQ_GetLastSpawn()
{
    uintptr_t spawnManager = eq::Memory::Read<uintptr_t>(eq::EQGame::Addresses::Pointers::SpawnManager);
    if (spawnManager == NULL)
    {
        return NULL;
    }

    return eq::Memory::Read<uintptr_t>(spawnManager + eq::Offsets::SpawnManager::LastSpawn);
}

uintptr_t EQ_GetControlledSpawn()
{
    return eq::Memory::Read<uintptr_t>(eq::EQGame::Addresses::Pointers::ControlledSpawn);
}

uintptr_t EQ_GetPlayerSpawn()
{
    return eq::Memory::Read<uintptr_t>(eq::EQGame::Addresses::Pointers::PlayerSpawn);
}

uintptr_t EQ_GetTargetSpawn()
{
    return eq::Memory::Read<uintptr_t>(eq::EQGame::Addresses::Pointers::TargetSpawn);
}

void EQ_SetTargetSpawn(uintptr_t spawn)
{
    eq::Memory::Write<uintptr_t>(eq::EQGame::Addresses::Pointers::TargetSpawn, spawn);
}

void EQ_SetTargetSpawnByID(uint32_t spawnID)
{
    uintptr_t spawn = EQ_GetSpawnByID(spawnID);
    if (spawn == NULL)
    {
        return;
    }

    EQ_SetTargetSpawn(spawn);
}

void EQ_SetTargetSpawnByName(const char* spawnName)
{
    uintptr_t spawn = EQ_GetSpawnByName(spawnName);
    if (spawn == NULL)
    {
        return;
    }

    EQ_SetTargetSpawn(spawn);
}

uint32_t EQ_GetPlayerSpawnID()
{
    uintptr_t playerSpawn = EQ_GetPlayerSpawn();
    if (playerSpawn == NULL)
    {
        return 0xFFFFFFFF;
    }

    return EQ_GetSpawnID(playerSpawn);
}

std::string EQ_GetPlayerSpawnNameNumbered()
{
    uintptr_t playerSpawn = EQ_GetPlayerSpawn();
    if (playerSpawn == NULL)
    {
        return std::string();
    }

    return EQ_GetSpawnNameNumbered(playerSpawn);
}

std::string EQ_GetPlayerSpawnName()
{
    uintptr_t playerSpawn = EQ_GetPlayerSpawn();
    if (playerSpawn == NULL)
    {
        return std::string();
    }

    return EQ_GetSpawnName(playerSpawn);
}

std::string EQ_GetPlayerSpawnLastName()
{
    uintptr_t playerSpawn = EQ_GetPlayerSpawn();
    if (playerSpawn == NULL)
    {
        return std::string();
    }

    return EQ_GetSpawnLastName(playerSpawn);
}

uint32_t EQ_GetTargetSpawnID()
{
    uintptr_t targetSpawn = EQ_GetTargetSpawn();
    if (targetSpawn == NULL)
    {
        return 0xFFFFFFFF;
    }

    return EQ_GetSpawnID(targetSpawn);
}

std::string EQ_GetTargetSpawnNameNumbered()
{
    uintptr_t targetSpawn = EQ_GetTargetSpawn();
    if (targetSpawn == NULL)
    {
        return std::string();
    }

    return EQ_GetSpawnNameNumbered(targetSpawn);
}

std::string EQ_GetTargetSpawnName()
{
    uintptr_t targetSpawn = EQ_GetTargetSpawn();
    if (targetSpawn == NULL)
    {
        return std::string();
    }

    return EQ_GetSpawnName(targetSpawn);
}

std::string EQ_GetTargetSpawnLastName()
{
    uintptr_t targetSpawn = EQ_GetTargetSpawn();
    if (targetSpawn == NULL)
    {
        return std::string();
    }

    return EQ_GetSpawnLastName(targetSpawn);
}

float EQ_GetSpawnDistance(uintptr_t spawn)
{
    uintptr_t playerSpawn = EQ_GetPlayerSpawn();
    if (playerSpawn == NULL)
    {
        return -1.0f;
    }

    float playerSpawnY = EQ_GetSpawnY(playerSpawn);
    float playerSpawnX = EQ_GetSpawnX(playerSpawn);

    float spawnY = EQ_GetSpawnY(spawn);
    float spawnX = EQ_GetSpawnX(spawn);

    float spawnDistance = EQ_CalculateDistance(playerSpawnY, playerSpawnX, spawnY, spawnX);

    return spawnDistance;
}

float EQ_GetSpawnDistance3D(uintptr_t spawn)
{
    uintptr_t playerSpawn = EQ_GetPlayerSpawn();
    if (playerSpawn == NULL)
    {
        return -1.0f;
    }

    float playerSpawnY = EQ_GetSpawnY(playerSpawn);
    float playerSpawnX = EQ_GetSpawnX(playerSpawn);
    float playerSpawnZ = EQ_GetSpawnZ(playerSpawn);

    float spawnY = EQ_GetSpawnY(spawn);
    float spawnX = EQ_GetSpawnX(spawn);
    float spawnZ = EQ_GetSpawnZ(spawn);

    float spawnDistance = EQ_CalculateDistance3D(playerSpawnY, playerSpawnX, playerSpawnZ, spawnY, spawnX, spawnZ);

    return spawnDistance;
}

bool EQ_IsSpawnWithinDistance(uintptr_t spawn, float distance)
{
    uintptr_t playerSpawn = EQ_GetPlayerSpawn();
    if (playerSpawn == NULL)
    {
        return false;
    }

    float playerSpawnY = EQ_GetSpawnY(playerSpawn);
    float playerSpawnX = EQ_GetSpawnX(playerSpawn);

    float spawnY = EQ_GetSpawnY(spawn);
    float spawnX = EQ_GetSpawnX(spawn);

    bool isWithinDistance = EQ_IsWithinDistance(playerSpawnY, playerSpawnX, spawnY, spawnX, distance);

    return isWithinDistance;
}

bool EQ_IsSpawnWithinDistance3D(uintptr_t spawn, float distance)
{
    uintptr_t playerSpawn = EQ_GetPlayerSpawn();
    if (playerSpawn == NULL)
    {
        return false;
    }

    float playerSpawnY = EQ_GetSpawnY(playerSpawn);
    float playerSpawnX = EQ_GetSpawnX(playerSpawn);
    float playerSpawnZ = EQ_GetSpawnZ(playerSpawn);

    float spawnY = EQ_GetSpawnY(spawn);
    float spawnX = EQ_GetSpawnX(spawn);
    float spawnZ = EQ_GetSpawnZ(spawn);

    bool isWithinDistance = EQ_IsWithinDistance3D(playerSpawnY, playerSpawnX, playerSpawnZ, spawnY, spawnX, spawnZ, distance);

    return isWithinDistance;
}

bool EQ_IsSpawnWithinDistanceOfLocation(uintptr_t spawn, float y, float x, float z, float distance)
{
    float spawnY = EQ_GetSpawnY(spawn);
    float spawnX = EQ_GetSpawnX(spawn);

    float diffY = std::fabsf(spawnY - y);
    float diffX = std::fabsf(spawnX - x);

    bool isWithinDistance = (diffY < distance && diffX < distance);

    return isWithinDistance;
}

bool EQ_IsSpawnWithinDistance3DOfLocation(uintptr_t spawn, float y, float x, float z, float distance)
{
    float spawnY = EQ_GetSpawnY(spawn);
    float spawnX = EQ_GetSpawnX(spawn);
    float spawnZ = EQ_GetSpawnZ(spawn);

    float diffY = std::fabsf(spawnY - y);
    float diffX = std::fabsf(spawnX - x);
    float diffZ = std::fabsf(spawnZ - z);

    bool isWithinDistance = (diffY < distance && diffX < distance && diffZ < distance);

    return isWithinDistance;
}

bool EQ_IsSpawnWithinDistanceOfSpawn(uintptr_t spawn1, uintptr_t spawn2, float distance)
{
    float spawnY = EQ_GetSpawnY(spawn2);
    float spawnX = EQ_GetSpawnX(spawn2);
    float spawnZ = EQ_GetSpawnZ(spawn2);

    bool isWithinDistance = EQ_IsSpawnWithinDistanceOfLocation(spawn1, distance, spawnY, spawnX, spawnZ);

    return isWithinDistance;
}

bool EQ_IsSpawnWithinDistance3DOfSpawn(uintptr_t spawn1, uintptr_t spawn2, float distance)
{
    float spawnY = EQ_GetSpawnY(spawn2);
    float spawnX = EQ_GetSpawnX(spawn2);
    float spawnZ = EQ_GetSpawnZ(spawn2);

    bool isWithinDistance = EQ_IsSpawnWithinDistance3DOfLocation(spawn1, distance, spawnY, spawnX, spawnZ);

    return isWithinDistance;
}

bool EQ_IsSpawnTargetable(uintptr_t spawn)
{
    return eq::Memory::Read<uint8_t>(spawn + eq::Offsets::Spawn::IsTargetable);
}

bool EQ_IsSpawnClickThrough(uintptr_t spawn)
{
    return eq::Memory::Read<uint8_t>(spawn + eq::Offsets::Spawn::IsClickThrough);
}

bool EQ_IsSpawnSwimming(uintptr_t spawn)
{
    uint32_t underwater = eq::Memory::Read<uint32_t>(spawn + eq::Offsets::Spawn::LastHeadEnvironmentType);
    if (underwater != 0)
    {
        return true;
    }

    uint8_t head = eq::Memory::Read<uint8_t>(spawn + eq::Offsets::Spawn::HeadEnvironmentType);
    if (head != 0)
    {
        return true;
    }

    uint8_t body = eq::Memory::Read<uint8_t>(spawn + eq::Offsets::Spawn::BodyEnvironmentType);
    uint8_t feet = eq::Memory::Read<uint8_t>(spawn + eq::Offsets::Spawn::FeetEnvironmentType);
    if (body != 0 && feet != 0)
    {
        return true;
    }

    return false;
}

bool EQ_IsSpawnSwimmingUnderwater(uintptr_t spawn)
{
    uint32_t underwater = eq::Memory::Read<uint32_t>(spawn + eq::Offsets::Spawn::LastHeadEnvironmentType);
    if (underwater != 0)
    {
        return true;
    }

    uint8_t head = eq::Memory::Read<uint8_t>(spawn + eq::Offsets::Spawn::HeadEnvironmentType);
    uint8_t body = eq::Memory::Read<uint8_t>(spawn + eq::Offsets::Spawn::BodyEnvironmentType);
    uint8_t feet = eq::Memory::Read<uint8_t>(spawn + eq::Offsets::Spawn::FeetEnvironmentType);
    if (head != 0 && body != 0 && feet != 0)
    {
        return true;
    }

    return false;
}

bool EQ_IsSpawnMoving(uintptr_t spawn)
{
    float spawnMovementSpeed = EQ_GetSpawnMovementSpeed(spawn);

    return (spawnMovementSpeed != 0.0f);
}

bool EQ_IsPlayerMoving()
{
    uintptr_t playerSpawn = EQ_GetPlayerSpawn();
    if (playerSpawn == NULL)
    {
        return false;
    }

    return EQ_IsSpawnMoving(playerSpawn);
}

bool EQ_IsTargetMoving()
{
    uintptr_t targetSpawn = EQ_GetTargetSpawn();
    if (targetSpawn == NULL)
    {
        return false;
    }

    return EQ_IsSpawnMoving(targetSpawn);
}

bool EQ_IsSpawnBehindSpawn(uintptr_t spawn1, uintptr_t spawn2)
{
    // use 512 / 8 = 64 for tighter angle
    // use 512 / 4 = 128 for wider angle

    if (EQ_IsSpawnBehindSpawnEx(spawn1, spawn2, 64.0f) == true)
    {
        return true;
    }

    return false;
}

bool EQ_IsSpawnBehindSpawnEx(uintptr_t spawn1, uintptr_t spawn2, float angle)
{
    float heading1 = EQ_GetSpawnHeading(spawn1);
    float heading2 = EQ_GetSpawnHeading(spawn2);

    float headingDifference = std::fabsf(heading1 - heading2);

    if (headingDifference <= angle)
    {
        return true;
    }

    return false;
}

bool EQ_IsPlayerBehindSpawn(uintptr_t spawn)
{
    uintptr_t playerSpawn = EQ_GetPlayerSpawn();
    if (playerSpawn == NULL)
    {
        return false;
    }

    if (spawn == NULL)
    {
        return false;
    }

    if (EQ_IsSpawnBehindSpawn(playerSpawn, spawn) == true)
    {
        return true;
    }

    return false;
}

bool EQ_IsPlayerBehindTarget()
{
    uintptr_t targetSpawn = EQ_GetTargetSpawn();
    if (targetSpawn == NULL)
    {
        return false;
    }

    return EQ_IsPlayerBehindSpawn(targetSpawn);
}

bool EQ_IsSpawnBehindPlayer(uintptr_t spawn)
{
    uintptr_t playerSpawn = EQ_GetPlayerSpawn();
    if (playerSpawn == NULL)
    {
        return false;
    }

    if (spawn == NULL)
    {
        return false;
    }

    if (EQ_IsSpawnBehindSpawn(spawn, playerSpawn) == true)
    {
        return true;
    }

    return false;
}

bool EQ_IsTargetBehindPlayer()
{
    uintptr_t targetSpawn = EQ_GetTargetSpawn();
    if (targetSpawn == NULL)
    {
        return false;
    }

    return EQ_IsSpawnBehindPlayer(targetSpawn);
}

std::string EQ_GetSpawnNameNumbered(uintptr_t spawn)
{
    if (spawn == NULL)
    {
        return std::string();
    }

    char spawnNameNumbered[eq::Offsets::Spawn::SizeOf::NameNumbered];
    std::memcpy(spawnNameNumbered, (void*)(spawn + eq::Offsets::Spawn::NameNumbered), sizeof(spawnNameNumbered));

    return spawnNameNumbered;
}

std::string EQ_GetSpawnName(uintptr_t spawn)
{
    if (spawn == NULL)
    {
        return std::string();
    }

    char spawnName[eq::Offsets::Spawn::SizeOf::Name];
    std::memcpy(spawnName, (void*)(spawn + eq::Offsets::Spawn::Name), sizeof(spawnName));

    return spawnName;
}

std::string EQ_GetSpawnLastName(uintptr_t spawn)
{
    if (spawn == NULL)
    {
        return std::string();
    }

    char spawnLastName[eq::Offsets::Spawn::SizeOf::LastName];
    std::memcpy(spawnLastName, (void*)(spawn + eq::Offsets::Spawn::LastName), sizeof(spawnLastName));

    return spawnLastName;
}

uintptr_t EQ_GetSpawnPrevious(uintptr_t spawn)
{
    return eq::Memory::Read<uintptr_t>(spawn + eq::Offsets::Spawn::Previous);
}

uintptr_t EQ_GetSpawnNext(uintptr_t spawn)
{
    return eq::Memory::Read<uintptr_t>(spawn + eq::Offsets::Spawn::Next);
}

float EQ_GetSpawnJumpStrength(uintptr_t spawn)
{
    return eq::Memory::Read<float>(spawn + eq::Offsets::Spawn::JumpStrength);
}

float EQ_GetSpawnSwimStrength(uintptr_t spawn)
{
    return eq::Memory::Read<float>(spawn + eq::Offsets::Spawn::SwimStrength);
}

float EQ_GetSpawnMovementSpeedBonus(uintptr_t spawn)
{
    return eq::Memory::Read<float>(spawn + eq::Offsets::Spawn::SpeedMultiplier);
}

float EQ_GetSpawnAreaFriction(uintptr_t spawn)
{
    return eq::Memory::Read<float>(spawn + eq::Offsets::Spawn::AreaFriction);
}

float EQ_GetSpawnAccelerationFriction(uintptr_t spawn)
{
    return eq::Memory::Read<float>(spawn + eq::Offsets::Spawn::AccelerationFriction);
}

uint32_t EQ_GetSpawnCollideWithCActorInterfaceType(uintptr_t spawn)
{
    return eq::Memory::Read<uint32_t>(spawn + eq::Offsets::Spawn::CollideWithCActorInterfaceType);
}

float EQ_GetSpawnFloorZ(uintptr_t spawn)
{
    return eq::Memory::Read<float>(spawn + eq::Offsets::Spawn::FloorZ);
}

float EQ_GetSpawnY(uintptr_t spawn)
{
    return eq::Memory::Read<float>(spawn + eq::Offsets::Spawn::Y);
}

float EQ_GetSpawnX(uintptr_t spawn)
{
    return eq::Memory::Read<float>(spawn + eq::Offsets::Spawn::X);
}

float EQ_GetSpawnZ(uintptr_t spawn)
{
    return eq::Memory::Read<float>(spawn + eq::Offsets::Spawn::Z);
}

float EQ_GetSpawnSpeedY(uintptr_t spawn)
{
    return eq::Memory::Read<float>(spawn + eq::Offsets::Spawn::SpeedY);
}

float EQ_GetSpawnSpeedX(uintptr_t spawn)
{
    return eq::Memory::Read<float>(spawn + eq::Offsets::Spawn::SpeedX);
}

float EQ_GetSpawnSpeedZ(uintptr_t spawn)
{
    return eq::Memory::Read<float>(spawn + eq::Offsets::Spawn::SpeedZ);
}

float EQ_GetSpawnMovementSpeed(uintptr_t spawn)
{
    return eq::Memory::Read<float>(spawn + eq::Offsets::Spawn::MovementSpeed);
}

float EQ_GetSpawnHeading(uintptr_t spawn)
{
    return eq::Memory::Read<float>(spawn + eq::Offsets::Spawn::Heading);
}

float EQ_GetSpawnHeadingSpeed(uintptr_t spawn)
{
    return eq::Memory::Read<float>(spawn + eq::Offsets::Spawn::HeadingSpeed);
}

float EQ_GetSpawnPitch(uintptr_t spawn)
{
    return eq::Memory::Read<float>(spawn + eq::Offsets::Spawn::Pitch);
}

uint32_t EQ_GetSpawnLastHeadEnvironmentType(uintptr_t spawn)
{
    return eq::Memory::Read<uint32_t>(spawn + eq::Offsets::Spawn::LastHeadEnvironmentType);
}

uint32_t EQ_GetSpawnLastBodyEnvironmentType(uintptr_t spawn)
{
    return eq::Memory::Read<uint32_t>(spawn + eq::Offsets::Spawn::LastBodyEnvironmentType);
}

uint32_t EQ_GetSpawnLastFeetEnvironmentType(uintptr_t spawn)
{
    return eq::Memory::Read<uint32_t>(spawn + eq::Offsets::Spawn::LastFeetEnvironmentType);
}

uint8_t EQ_GetSpawnHeadEnvironmentType(uintptr_t spawn)
{
    return eq::Memory::Read<uint8_t>(spawn + eq::Offsets::Spawn::HeadEnvironmentType);
}

uint8_t EQ_GetSpawnFeetEnvironmentType(uintptr_t spawn)
{
    return eq::Memory::Read<uint8_t>(spawn + eq::Offsets::Spawn::FeetEnvironmentType);
}

uint8_t EQ_GetSpawnBodyEnvironmentType(uintptr_t spawn)
{
    return eq::Memory::Read<uint8_t>(spawn + eq::Offsets::Spawn::BodyEnvironmentType);
}

uint8_t EQ_GetSpawnBodyEnvironmentType2(uintptr_t spawn)
{
    return eq::Memory::Read<uint8_t>(spawn + eq::Offsets::Spawn::BodyEnvironmentType2);
}

uint8_t EQ_GetSpawnType(uintptr_t spawn)
{
    return eq::Memory::Read<uint8_t>(spawn + eq::Offsets::Spawn::Type);
}

float EQ_GetSpawnHeightZ(uintptr_t spawn)
{
    return eq::Memory::Read<float>(spawn + eq::Offsets::Spawn::HeightZ);
}

float EQ_GetSpawnHeight(uintptr_t spawn)
{
    return eq::Memory::Read<float>(spawn + eq::Offsets::Spawn::Height);
}

float EQ_GetSpawnWidth(uintptr_t spawn)
{
    return eq::Memory::Read<float>(spawn + eq::Offsets::Spawn::Width);
}

float EQ_GetSpawnLength(uintptr_t spawn)
{
    return eq::Memory::Read<float>(spawn + eq::Offsets::Spawn::Length);
}

uint32_t EQ_GetSpawnID(uintptr_t spawn)
{
    return eq::Memory::Read<uint32_t>(spawn + eq::Offsets::Spawn::ID);
}

uint32_t EQ_GetSpawnState(uintptr_t spawn)
{
    return eq::Memory::Read<uint32_t>(spawn + eq::Offsets::Spawn::State);
}

uintptr_t EQ_GetSpawnVehicleSpawn(uintptr_t spawn)
{
    return eq::Memory::Read<uintptr_t>(spawn + eq::Offsets::Spawn::VehicleSpawn);
}

uintptr_t EQ_GetSpawnMountSpawn(uintptr_t spawn)
{
    return eq::Memory::Read<uintptr_t>(spawn + eq::Offsets::Spawn::MountSpawn);
}

uintptr_t EQ_GetSpawnMountRiderSpawn(uintptr_t spawn)
{
    return eq::Memory::Read<uintptr_t>(spawn + eq::Offsets::Spawn::MountRiderSpawn);
}

uint32_t EQ_GetSpawnDirection(uintptr_t spawn)
{
    float spawnHeading = EQ_GetSpawnHeading(spawn);

    uint32_t spawnHeadingAsInteger = (uint32_t)spawnHeading;

    uint32_t direction = eq::Constants::Direction::Unknown;

    if (spawnHeading == eq::Constants::Heading::North)
    {
        direction = eq::Constants::Direction::North;
    }
    else if (spawnHeading == eq::Constants::Heading::NorthEast)
    {
        direction = eq::Constants::Direction::NorthEast;
    }
    else if (spawnHeading == eq::Constants::Heading::East)
    {
        direction = eq::Constants::Direction::East;
    }
    else if (spawnHeading == eq::Constants::Heading::SouthEast)
    {
        direction = eq::Constants::Direction::SouthEast;
    }
    else if (spawnHeading == eq::Constants::Heading::South)
    {
        direction = eq::Constants::Direction::South;
    }
    else if (spawnHeading == eq::Constants::Heading::SouthWest)
    {
        direction = eq::Constants::Direction::SouthWest;
    }
    else if (spawnHeading == eq::Constants::Heading::West)
    {
        direction = eq::Constants::Direction::West;
    }
    else if (spawnHeading == eq::Constants::Heading::NorthWest)
    {
        direction = eq::Constants::Direction::NorthWest;
    }
    else
    {
        if (spawnHeadingAsInteger > 479)
        {
            direction = eq::Constants::Direction::North;
        }
        else if (spawnHeadingAsInteger > 416)
        {
            direction = eq::Constants::Direction::NorthEast;
        }
        else if (spawnHeadingAsInteger > 352)
        {
            direction = eq::Constants::Direction::East;
        }
        else if (spawnHeadingAsInteger > 288)
        {
            direction = eq::Constants::Direction::SouthEast;
        }
        else if (spawnHeadingAsInteger > 224)
        {
            direction = eq::Constants::Direction::South;
        }
        else if (spawnHeadingAsInteger > 160)
        {
            direction = eq::Constants::Direction::SouthWest;
        }
        else if (spawnHeadingAsInteger > 96)
        {
            direction = eq::Constants::Direction::West;
        }
        else if (spawnHeadingAsInteger > 32)
        {
            direction = eq::Constants::Direction::NorthWest;
        }
        else
        {
            direction = eq::Constants::Direction::North;
        }
    }

    return direction;
}

void EQ_SetSpawnSpeedY(uintptr_t spawn, float speed)
{
    eq::Memory::Write<float>(spawn + eq::Offsets::Spawn::SpeedY, speed);
}

void EQ_SetSpawnSpeedX(uintptr_t spawn, float speed)
{
    eq::Memory::Write<float>(spawn + eq::Offsets::Spawn::SpeedX, speed);
}

void EQ_SetSpawnSpeedZ(uintptr_t spawn, float speed)
{
    eq::Memory::Write<float>(spawn + eq::Offsets::Spawn::SpeedZ, speed);
}

void EQ_SetSpawnAreaFriction(uintptr_t spawn, float friction)
{
    eq::Memory::Write<float>(spawn + eq::Offsets::Spawn::AreaFriction, friction);

    uintptr_t spawnMountSpawn = EQ_GetSpawnMountSpawn(spawn);
    if (spawnMountSpawn != NULL)
    {
        eq::Memory::Write<float>(spawnMountSpawn + eq::Offsets::Spawn::AreaFriction, friction);
    }
}

void EQ_SetSpawnAccelerationFriction(uintptr_t spawn, float friction)
{
    eq::Memory::Write<float>(spawn + eq::Offsets::Spawn::AccelerationFriction, friction);

    uintptr_t spawnMountSpawn = EQ_GetSpawnMountSpawn(spawn);
    if (spawnMountSpawn != NULL)
    {
        eq::Memory::Write<float>(spawnMountSpawn + eq::Offsets::Spawn::AccelerationFriction, friction);
    }
}

void EQ_SetSpawnHeading(uintptr_t spawn, float heading)
{
    eq::Memory::Write<float>(spawn + eq::Offsets::Spawn::Heading, heading);

    uintptr_t spawnMountSpawn = EQ_GetSpawnMountSpawn(spawn);
    if (spawnMountSpawn != NULL)
    {
        eq::Memory::Write<float>(spawnMountSpawn + eq::Offsets::Spawn::Heading, heading);
    }
}

void EQ_SetSpawnPitch(uintptr_t spawn, float pitch)
{
    eq::Memory::Write<float>(spawn + eq::Offsets::Spawn::Pitch, pitch);

    uintptr_t spawnMountSpawn = EQ_GetSpawnMountSpawn(spawn);
    if (spawnMountSpawn != NULL)
    {
        eq::Memory::Write<float>(spawnMountSpawn + eq::Offsets::Spawn::Pitch, pitch);
    }
}

void EQ_SetPlayerSpawnHeadingNorth()
{
    uintptr_t playerSpawn = EQ_GetPlayerSpawn();
    if (playerSpawn == NULL)
    {
        return;
    }

    EQ_SetSpawnHeading(playerSpawn, eq::Constants::Heading::North);
}

void EQ_SetPlayerSpawnHeadingNorthWest()
{
    uintptr_t playerSpawn = EQ_GetPlayerSpawn();
    if (playerSpawn == NULL)
    {
        return;
    }

    EQ_SetSpawnHeading(playerSpawn, eq::Constants::Heading::NorthWest);
}

void EQ_SetPlayerSpawnHeadingWest()
{
    uintptr_t playerSpawn = EQ_GetPlayerSpawn();
    if (playerSpawn == NULL)
    {
        return;
    }

    EQ_SetSpawnHeading(playerSpawn, eq::Constants::Heading::West);
}

void EQ_SetPlayerSpawnHeadingSouthWest()
{
    uintptr_t playerSpawn = EQ_GetPlayerSpawn();
    if (playerSpawn == NULL)
    {
        return;
    }

    EQ_SetSpawnHeading(playerSpawn, eq::Constants::Heading::SouthWest);
}

void EQ_SetPlayerSpawnHeadingSouth()
{
    uintptr_t playerSpawn = EQ_GetPlayerSpawn();
    if (playerSpawn == NULL)
    {
        return;
    }

    EQ_SetSpawnHeading(playerSpawn, eq::Constants::Heading::South);
}

void EQ_SetPlayerSpawnHeadingSouthEast()
{
    uintptr_t playerSpawn = EQ_GetPlayerSpawn();
    if (playerSpawn == NULL)
    {
        return;
    }

    EQ_SetSpawnHeading(playerSpawn, eq::Constants::Heading::SouthEast);
}

void EQ_SetPlayerSpawnHeadingEast()
{
    uintptr_t playerSpawn = EQ_GetPlayerSpawn();
    if (playerSpawn == NULL)
    {
        return;
    }

    EQ_SetSpawnHeading(playerSpawn, eq::Constants::Heading::East);
}

void EQ_SetPlayerSpawnHeadingNorthEast()
{
    uintptr_t playerSpawn = EQ_GetPlayerSpawn();
    if (playerSpawn == NULL)
    {
        return;
    }

    EQ_SetSpawnHeading(playerSpawn, eq::Constants::Heading::NorthEast);
}

void EQ_SetPlayerSpawnFrictionToDefault()
{
    uintptr_t playerSpawn = EQ_GetPlayerSpawn();
    if (playerSpawn == NULL)
    {
        return;
    }

    EQ_SetSpawnAreaFriction(playerSpawn, eq::Constants::Spawn::AreaFriction::Default);
    EQ_SetSpawnAccelerationFriction(playerSpawn, eq::Constants::Spawn::AccelerationFriction::Default);
}

void EQ_TurnLeft()
{
    uintptr_t playerSpawn = EQ_GetPlayerSpawn();
    if (playerSpawn == NULL)
    {
        return;
    }

    float playerHeading = EQ_GetSpawnHeading(playerSpawn);

    playerHeading = EQ_RoundHeading(playerHeading);

    playerHeading = playerHeading + eq::Constants::Heading::MaxQuarter;

    playerHeading = EQ_FixHeading(playerHeading);

    EQ_SetSpawnHeading(playerSpawn, playerHeading);
}

void EQ_TurnRight()
{
    uintptr_t playerSpawn = EQ_GetPlayerSpawn();
    if (playerSpawn == NULL)
    {
        return;
    }

    float playerHeading = EQ_GetSpawnHeading(playerSpawn);

    playerHeading = EQ_RoundHeading(playerHeading);

    playerHeading = playerHeading - eq::Constants::Heading::MaxQuarter;

    playerHeading = EQ_FixHeading(playerHeading);

    EQ_SetSpawnHeading(playerSpawn, playerHeading);
}

void EQ_TurnAround()
{
    uintptr_t playerSpawn = EQ_GetPlayerSpawn();
    if (playerSpawn == NULL)
    {
        return;
    }

    float playerHeading = EQ_GetSpawnHeading(playerSpawn);

    playerHeading = EQ_RoundHeading(playerHeading);

    playerHeading = playerHeading + eq::Constants::Heading::MaxHalf;

    playerHeading = EQ_FixHeading(playerHeading);

    EQ_SetSpawnHeading(playerSpawn, playerHeading);
}

void EQ_TurnSpawnTowardsLocation(uintptr_t spawn, float y, float x)
{
    float spawnY = EQ_GetSpawnY(spawn);
    float spawnX = EQ_GetSpawnX(spawn);

    float heading = EQ_GetBearing(spawnY, spawnX, y, x);

    EQ_SetSpawnHeading(spawn, heading);
}

void EQ_TurnSpawnAwayFromLocation(uintptr_t spawn, float y, float x)
{
    float spawnY = EQ_GetSpawnY(spawn);
    float spawnX = EQ_GetSpawnX(spawn);

    float heading = EQ_GetBearing(y, x, spawnY, spawnX);

    EQ_SetSpawnHeading(spawn, heading);
}

void EQ_TurnSpawnTowardsSpawn(uintptr_t spawn1, uintptr_t spawn2)
{
    float spawnY = EQ_GetSpawnY(spawn2);
    float spawnX = EQ_GetSpawnX(spawn2);

    EQ_TurnSpawnTowardsLocation(spawn1, spawnY, spawnX);
}

void EQ_TurnSpawnAwayFromSpawn(uintptr_t spawn1, uintptr_t spawn2)
{
    float spawnY = EQ_GetSpawnY(spawn2);
    float spawnX = EQ_GetSpawnX(spawn2);

    EQ_TurnSpawnAwayFromLocation(spawn1, spawnY, spawnX);
}

void EQ_TurnPlayerTowardsSpawn(uintptr_t spawn)
{
    uintptr_t playerSpawn = EQ_GetPlayerSpawn();
    if (playerSpawn == NULL)
    {
        return;
    }

    float spawnY = EQ_GetSpawnY(spawn);
    float spawnX = EQ_GetSpawnX(spawn);

    EQ_TurnSpawnTowardsLocation(playerSpawn, spawnY, spawnX);
}

void EQ_TurnPlayerAwayFromSpawn(uintptr_t spawn)
{
    uintptr_t playerSpawn = EQ_GetPlayerSpawn();
    if (playerSpawn == NULL)
    {
        return;
    }

    float spawnY = EQ_GetSpawnY(spawn);
    float spawnX = EQ_GetSpawnX(spawn);

    EQ_TurnSpawnAwayFromLocation(playerSpawn, spawnY, spawnX);
}

void EQ_TurnPlayerTowardsTarget()
{
    uintptr_t targetSpawn = EQ_GetTargetSpawn();
    if (targetSpawn == NULL)
    {
        return;
    }

    EQ_TurnPlayerTowardsSpawn(targetSpawn);
}

void EQ_TurnPlayerAwayFromTarget()
{
    uintptr_t targetSpawn = EQ_GetTargetSpawn();
    if (targetSpawn == NULL)
    {
        return;
    }

    EQ_TurnPlayerAwayFromSpawn(targetSpawn);
}

void EQ_InterpretCommand(const char* text)
{
    uintptr_t xCEverQuest = EQ_GetCEverQuest();
    if (xCEverQuest == NULL)
    {
        return;
    }

    uintptr_t playerSpawn = EQ_GetPlayerSpawn();
    if (playerSpawn == NULL)
    {
        return;
    }

    EQ_FUNCTION_CEverQuest__InterpretCommand((uintptr_t*)xCEverQuest, (uintptr_t*)playerSpawn, text);
}

void EQ_ExecuteCommand(uint32_t commandID)
{
    EQ_FUNCTION_ExecuteCommand(commandID, false, nullptr, nullptr);
}

void EQ_ExecuteCommandEx(uint32_t commandID, bool keyDown)
{
    EQ_FUNCTION_ExecuteCommand(commandID, keyDown, nullptr, nullptr);
}

void EQ_PrintTextToChat(const char* text)
{
    uintptr_t chatManager = EQ_GetChatManager();
    if (chatManager == NULL)
    {
        return;
    }

    EQ_FUNCTION_ChatManager__PrintText((uintptr_t*)chatManager, text, eq::Constants::ChatTextColor::White, true, true, NULL);
}

void EQ_PrintTextToChatByColor(const char* text, uint32_t chatTextColor)
{
    uintptr_t chatManager = EQ_GetChatManager();
    if (chatManager == NULL)
    {
        return;
    }

    EQ_FUNCTION_ChatManager__PrintText((uintptr_t*)chatManager, text, chatTextColor, true, true, NULL);
}

void EQ_DrawText(const char* text, uint32_t x, uint32_t y)
{
    EQ_FUNCTION_DrawText(text, x, y, eq::Constants::DrawTextColor::Gray);
}

void EQ_DrawTextByColor(const char* text, uint32_t x, uint32_t y, uint32_t drawTextColor)
{
    EQ_FUNCTION_DrawText(text, x, y, drawTextColor);
}

void EQ_PlaySound(const std::string& fileName)
{
    std::string filePath = std::format("sounds/{}", fileName);

    PlaySoundA(filePath.c_str(), NULL, SND_FILENAME | SND_NODEFAULT | SND_ASYNC);
}

void EQ_StopSound()
{
    PlaySoundA(NULL, NULL, NULL);
}

void EQ_CenterView()
{
    uintptr_t playerSpawn = EQ_GetPlayerSpawn();
    if (playerSpawn == NULL)
    {
        return;
    }

    EQ_SetSpawnPitch(playerSpawn, eq::Constants::Spawn::Pitch::Default);
}

void EQ_LookUp()
{
    uintptr_t playerSpawn = EQ_GetPlayerSpawn();
    if (playerSpawn == NULL)
    {
        return;
    }

    EQ_SetSpawnPitch(playerSpawn, eq::Constants::Spawn::Pitch::Max);
}

void EQ_LookDown()
{
    uintptr_t playerSpawn = EQ_GetPlayerSpawn();
    if (playerSpawn == NULL)
    {
        return;
    }

    EQ_SetSpawnPitch(playerSpawn, eq::Constants::Spawn::Pitch::Min);
}

void EQ_ClearTarget()
{
    EQ_SetTargetSpawn(NULL);
}

std::string EQ_StringMap_GetValueByKey(const std::unordered_map<uint32_t, std::string>& stringMap, uint32_t key)
{
    auto it = stringMap.find(key);
    if (it != stringMap.end())
    {
        return it->second;
    }

    return std::string();
}

uint32_t EQ_StringMap_GetKeyByValue(const std::unordered_map<uint32_t, std::string>& stringMap, const std::string& value)
{
    for (auto it = stringMap.begin(); it != stringMap.end(); ++it)
    {
        if (it->second == value)
        {
            return it->first;
        }
    }

    return 0xFFFFFFFF;
}

void EQ_UseAlternateAbility(uint32_t alternateAbilityID)
{
    std::string commandText = std::format("/alt activate {}", alternateAbilityID);

    EQ_InterpretCommand(commandText.c_str());
}

void EQ_UseDiscipline(const std::string& disciplineName)
{
    std::string commandText = std::format("/discipline {}", disciplineName);

    EQ_InterpretCommand(commandText.c_str());
}

void EQ_UseAbility(uint64_t abilityNumber)
{
    std::string commandText = std::format("/doability {}", abilityNumber);

    EQ_InterpretCommand(commandText.c_str());
}

void EQ_UseItem(const std::string& itemName)
{
    std::string commandText = std::format("/useitem {}", itemName);

    EQ_InterpretCommand(commandText.c_str());
}

}
