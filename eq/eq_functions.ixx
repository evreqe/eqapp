module;

#include "common.h"

export module eq_functions;

export import eq;
export import eq_classes;

export import eq_functions_h;

export
{

//////////////////////////////////////////////////
/* log functions */
//////////////////////////////////////////////////

void EQ_Log(const std::string& text)
{
    std::fstream file;
    file.open("eqapp/eq_log.txt", std::ios::out | std::ios::app);
    file << text;
    file.close();
}

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
/* class virtual table functions */
//////////////////////////////////////////////////

bool EQ_FUNCTION_CCamera__WorldSpaceToScreenSpace(void* thisPointer, eq::Location& location, float& screenX, float& screenY)
{
    uintptr_t camera = EQ_GetCCamera();
    if (camera == NULL)
    {
        return false;
    }

    uintptr_t cameraVirtualFunctionTable = eq::Memory::Read<uintptr_t>(camera + eq::Offsets::CCamera::VirtualFunctionTable_);
    if (cameraVirtualFunctionTable == NULL)
    {
        return false;
    }

    uintptr_t cameraWorldSpaceToScreenSpace = eq::Memory::Read<uintptr_t>(cameraVirtualFunctionTable + eq::Offsets::CCamera::VirtualFunctionTable::WorldSpaceToScreenSpace);
    if (cameraWorldSpaceToScreenSpace == NULL)
    {
        return false;
    }

    return ((EQ_FUNCTION_TYPE_CCamera__WorldSpaceToScreenSpace)cameraWorldSpaceToScreenSpace)(thisPointer, location, screenX, screenY);
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

float EQ_GetDistance2D(float y1, float x1, float y2, float x2)
{
    return std::sqrtf(std::powf(x2 - x1, 2.0f) + std::powf(y2 - y1, 2.0f));
}

float EQ_GetDistance3D(float y1, float x1, float z1, float y2, float x2, float z2)
{
    return std::sqrtf(std::powf(x2 - x1, 2.0f) + std::powf(y2 - y1, 2.0f) + std::powf(z2 - z1, 2.0f));
}

bool EQ_IsWithinDistance2D(float y1, float x1, float y2, float x2, float distance)
{
    return (std::powf(x2 - x1, 2.0f) + std::powf(y2 - y1, 2.0f)) <= std::powf(distance, 2.0f);
}

bool EQ_IsWithinDistance3D(float y1, float x1, float z1, float y2, float x2, float z2, float distance)
{
    return (std::powf(x2 - x1, 2.0f) + std::powf(y2 - y1, 2.0f) + std::powf(z2 - z1, 2.0f)) <= std::powf(distance, 2.0f);
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

float EQ_GetDistanceByLocation2D(const eq::Location& location1, const eq::Location& location2)
{
    return EQ_GetDistance2D(location1.Y, location1.X, location2.Y, location2.X);
}

float EQ_GetDistanceByLocation3D(const eq::Location& location1, const eq::Location& location2)
{
    return EQ_GetDistance3D(location1.Y, location1.X, location1.Z, location2.Y, location2.X, location2.Z);
}

bool EQ_IsWithinDistanceByLocation2D(const eq::Location& location1, const eq::Location& location2, float distance)
{
    return EQ_IsWithinDistance2D(location1.Y, location1.X, location2.Y, location2.X, distance);
}

bool EQ_IsWithinDistanceByLocation3D(const eq::Location& location1, const eq::Location& location2, float distance)
{
    return EQ_IsWithinDistance3D(location1.Y, location1.X, location1.Z, location2.Y, location2.X, location2.Z, distance);
}

float EQ_GetBearingByLocation(const eq::Location& location1, const eq::Location& location2)
{
    return EQ_GetBearing(location1.Y, location1.X, location2.Y, location2.X);
}

eq::Location EQ_GetMidpointByLocation(const eq::Location& location1, const eq::Location& location2)
{
    eq::Location midpoint;

    midpoint.Y = std::midpoint(location1.Y, location2.Y);
    midpoint.X = std::midpoint(location1.X, location2.X);
    midpoint.Z = std::midpoint(location1.Z, location2.Z);

    return midpoint;
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

uintptr_t EQ_GetCEverQuest()
{
    return eq::Memory::Read<uintptr_t>(eq::EQGame::Addresses::Pointers::CEverQuest);
}

uintptr_t EQ_GetCDisplay()
{
    return eq::Memory::Read<uintptr_t>(eq::EQGame::Addresses::Pointers::CDisplay);
}

uintptr_t EQ_GetCCamera()
{
    uintptr_t display = EQ_GetCDisplay();
    if (display == NULL)
    {
        return NULL;
    }

    return eq::Memory::Read<uintptr_t>(display + eq::Offsets::CDisplay::CCamera);
}

uintptr_t EQ_GetChatManager()
{
    return EQ_FUNCTION_GetChatManager();
}

uintptr_t EQ_GetSpawnManager()
{
    return eq::Memory::Read<uintptr_t>(eq::EQGame::Addresses::Pointers::SpawnManager);
}

uintptr_t EQ_GetSoundManager()
{
    return eq::Memory::Read<uintptr_t>(eq::EQGame::Addresses::Pointers::SoundManager);
}

uintptr_t EQ_GetSGraphicsEngine()
{
    return eq::Memory::Read<uintptr_t>(eq::EQGame::Addresses::Pointers::SGraphicsEngine);
}

uintptr_t EQ_GetCRender()
{
    uintptr_t graphicsEngine = EQ_GetSGraphicsEngine();
    if (graphicsEngine == NULL)
    {
        return NULL;
    }

    return eq::Memory::Read<uintptr_t>(graphicsEngine + eq::Offsets::SGraphicsEngine::CRender);
}

uintptr_t EQ_GetCParticleSystem()
{
    uintptr_t graphicsEngine = EQ_GetSGraphicsEngine();
    if (graphicsEngine == NULL)
    {
        return NULL;
    }

    return eq::Memory::Read<uintptr_t>(graphicsEngine + eq::Offsets::SGraphicsEngine::CParticleSystem);
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

bool EQ_SetWindowTitle(const std::string& windowTitle)
{
    HWND window = EQ_GetWindow();
    if (window == NULL)
    {
        return false;
    }

    SetWindowTextA(window, windowTitle.c_str());

    return true;
}

POINT EQ_GetMousePosition()
{
    HWND window = EQ_GetWindow();

    POINT point;
    GetCursorPos(&point);

    ScreenToClient(window, &point);

    return point;
}

eq::SpawnList EQ_GetSpawnList()
{
    eq::SpawnList spawnList;
    spawnList.reserve(4096);

    uintptr_t spawn = EQ_GetFirstSpawn();
    while (spawn != NULL)
    {
        spawnList.push_back(spawn);

        spawn = EQ_GetSpawnNextSpawn(spawn);
    }

    return spawnList;
}

bool EQ_DoesSpawnExist(uintptr_t spawn)
{
    if (spawn == NULL)
    {
        return false;
    }

    uintptr_t spawn_ = EQ_GetFirstSpawn();
    while (spawn_ != NULL)
    {
        if (spawn_ == spawn)
        {
            return true;
        }

        spawn_ = EQ_GetSpawnNextSpawn(spawn_);
    }

    return false;
}

uint64_t EQ_GetNumPlayersInZone()
{
    uint64_t numSpawns = 0;

    uintptr_t spawn = EQ_GetFirstSpawn();
    while (spawn != NULL)
    {
        uint8_t spawnType_ = EQ_GetSpawnType(spawn);
        if (spawnType_ != eq::Constants::Spawn::Type::Player)
        {
            spawn = EQ_GetSpawnNextSpawn(spawn);
            continue;
        }

        numSpawns++;

        spawn = EQ_GetSpawnNextSpawn(spawn);
    }

    return numSpawns;
}

uint64_t EQ_GetNumNearbyPlayers(float distance, float distanceZ)
{
    uint64_t numSpawns = 0;

    uintptr_t playerSpawn = EQ_GetPlayerSpawn();
    if (playerSpawn == NULL)
    {
        return numSpawns;
    }

    eq::Location playerSpawnLocation = EQ_GetSpawnLocation(playerSpawn);

    uintptr_t spawn = EQ_GetFirstSpawn();
    while (spawn != NULL)
    {
        if (spawn == playerSpawn)
        {
            spawn = EQ_GetSpawnNextSpawn(spawn);
            continue;
        }

        eq::Location spawnLocation = EQ_GetSpawnLocation(spawn);

        uint8_t spawnType_ = EQ_GetSpawnType(spawn);
        if (spawnType_ != eq::Constants::Spawn::Type::Player)
        {
            spawn = EQ_GetSpawnNextSpawn(spawn);
            continue;
        }

        float spawnDistance = EQ_GetDistanceByLocation2D(playerSpawnLocation, spawnLocation);
        if (spawnDistance > distance)
        {
            spawn = EQ_GetSpawnNextSpawn(spawn);
            continue;
        }

        float spawnZ = EQ_GetSpawnZ(spawn);

        float spawnDistanceZ = std::fabsf(playerSpawnLocation.Z - spawnLocation.Z);
        if (spawnDistanceZ > distanceZ)
        {
            spawn = EQ_GetSpawnNextSpawn(spawn);
            continue;
        }

        numSpawns++;

        spawn = EQ_GetSpawnNextSpawn(spawn);
    }

    return numSpawns;
}

uintptr_t EQ_GetSpawnByID(uint32_t spawnID)
{
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

uintptr_t EQ_GetSpawnByNameNumbered(const std::string& spawnName)
{
    uintptr_t spawnManager = EQ_GetSpawnManager();
    if (spawnManager == NULL)
    {
        return NULL;
    }

    uintptr_t spawn = EQ_FUNCTION_SpawnManager__GetSpawnByName((uintptr_t*)spawnManager, spawnName.c_str());
    if (spawn == NULL)
    {
        return NULL;
    }

    return spawn;
}

uintptr_t EQ_GetSpawnByNameOrNameNumbered(const std::string& spawnName)
{
    uintptr_t spawn = EQ_GetFirstSpawn();

    while (spawn != NULL)
    {
        std::string spawnName_ = EQ_GetSpawnName(spawn);

        if (spawnName_ == spawnName)
        {
            return spawn;
        }

        std::string spawnNameNumbered_ = EQ_GetSpawnNameNumbered(spawn);

        if (spawnNameNumbered_ == spawnName)
        {
            return spawn;
        }

        spawn = EQ_GetSpawnNextSpawn(spawn);
    }

    return NULL;
}

uintptr_t EQ_GetSpawnByName(const std::string& spawnName)
{
    uintptr_t spawn = EQ_GetFirstSpawn();

    while (spawn != NULL)
    {
        std::string spawnName_ = EQ_GetSpawnName(spawn);

        if (spawnName_ == spawnName)
        {
            return spawn;
        }

        spawn = EQ_GetSpawnNextSpawn(spawn);
    }

    return NULL;
}

uintptr_t EQ_GetFirstSpawn()
{
    uintptr_t spawnManager = EQ_GetSpawnManager();
    if (spawnManager == NULL)
    {
        return NULL;
    }

    return eq::Memory::Read<uintptr_t>(spawnManager + eq::Offsets::SpawnManager::FirstSpawn);
}

uintptr_t EQ_GetLastSpawn()
{
    uintptr_t spawnManager = EQ_GetSpawnManager();
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

void EQ_SetTargetSpawnByName(const std::string& spawnName)
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
        return eq::Constants::Spawn::ID::Null;
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
        return eq::Constants::Spawn::ID::Null;
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

float EQ_GetSpawnDistance2D(uintptr_t spawn)
{
    uintptr_t playerSpawn = EQ_GetPlayerSpawn();
    if (playerSpawn == NULL)
    {
        return eq::Constants::FloatInfinity;
    }

    eq::Location playerSpawnLocation = EQ_GetSpawnLocation(playerSpawn);

    eq::Location spawnLocation = EQ_GetSpawnLocation(spawn);

    float spawnDistance = EQ_GetDistanceByLocation2D(playerSpawnLocation, spawnLocation);

    return spawnDistance;
}

float EQ_GetSpawnDistance3D(uintptr_t spawn)
{
    uintptr_t playerSpawn = EQ_GetPlayerSpawn();
    if (playerSpawn == NULL)
    {
        return eq::Constants::FloatInfinity;
    }

    eq::Location playerSpawnLocation = EQ_GetSpawnLocation(playerSpawn);

    eq::Location spawnLocation = EQ_GetSpawnLocation(spawn);

    float spawnDistance = EQ_GetDistanceByLocation3D(playerSpawnLocation, spawnLocation);

    return spawnDistance;
}

eq::Location EQ_GetSpawnLocation(uintptr_t spawn)
{
    eq::Location location;
    location.Y = eq::Constants::FloatInfinity;
    location.X = eq::Constants::FloatInfinity;
    location.Z = eq::Constants::FloatInfinity;

    if (spawn == NULL)
    {
        return location;
    }

    location.Y = eq::Memory::Read<float>(spawn + eq::Offsets::Spawn::Y);
    location.X = eq::Memory::Read<float>(spawn + eq::Offsets::Spawn::X);
    location.Z = eq::Memory::Read<float>(spawn + eq::Offsets::Spawn::Z);

    return location;
}

bool EQ_IsSpawnWithinDistance2D(uintptr_t spawn, float distance)
{
    uintptr_t playerSpawn = EQ_GetPlayerSpawn();
    if (playerSpawn == NULL)
    {
        return false;
    }

    eq::Location playerSpawnLocation = EQ_GetSpawnLocation(playerSpawn);

    eq::Location spawnLocation = EQ_GetSpawnLocation(spawn);

    bool isWithinDistance = EQ_IsWithinDistanceByLocation2D(playerSpawnLocation, spawnLocation, distance);

    return isWithinDistance;
}

bool EQ_IsSpawnWithinDistance3D(uintptr_t spawn, float distance)
{
    uintptr_t playerSpawn = EQ_GetPlayerSpawn();
    if (playerSpawn == NULL)
    {
        return false;
    }

    eq::Location playerSpawnLocation = EQ_GetSpawnLocation(playerSpawn);

    eq::Location spawnLocation = EQ_GetSpawnLocation(spawn);

    bool isWithinDistance = EQ_IsWithinDistanceByLocation3D(playerSpawnLocation, spawnLocation, distance);

    return isWithinDistance;
}

bool EQ_IsSpawnWithinDistanceOfLocation2D(uintptr_t spawn, const eq::Location& location, float distance)
{
    eq::Location spawnLocation = EQ_GetSpawnLocation(spawn);

    float diffY = std::fabsf(spawnLocation.Y - location.Y);
    float diffX = std::fabsf(spawnLocation.X - location.X);

    bool isWithinDistance = (diffY < distance && diffX < distance);

    return isWithinDistance;
}

bool EQ_IsSpawnWithinDistanceOfLocation3D(uintptr_t spawn, const eq::Location& location, float distance)
{
    eq::Location spawnLocation = EQ_GetSpawnLocation(spawn);

    float diffY = std::fabsf(spawnLocation.Y - location.Y);
    float diffX = std::fabsf(spawnLocation.X - location.X);
    float diffZ = std::fabsf(spawnLocation.Z - location.Z);

    bool isWithinDistance = (diffY < distance && diffX < distance && diffZ < distance);

    return isWithinDistance;
}

bool EQ_IsSpawnWithinDistanceOfSpawn2D(uintptr_t spawn1, uintptr_t spawn2, float distance)
{
    eq::Location spawn2Location = EQ_GetSpawnLocation(spawn2);

    bool isWithinDistance = EQ_IsSpawnWithinDistanceOfLocation2D(spawn1, spawn2Location, distance);

    return isWithinDistance;
}

bool EQ_IsSpawnWithinDistanceOfSpawn3D(uintptr_t spawn1, uintptr_t spawn2, float distance)
{
    eq::Location spawn2Location = EQ_GetSpawnLocation(spawn2);

    bool isWithinDistance = EQ_IsSpawnWithinDistanceOfLocation3D(spawn1, spawn2Location, distance);

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

    if (EQ_IsSpawnBehindSpawnEx(spawn1, spawn2, 96.0f) == true)
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

int EQ_GetSpawnLevel(uintptr_t spawn)
{
    if (spawn == NULL)
    {
        return -1;
    }

    return EQ_FUNCTION_Spawn__GetLevel((uintptr_t*)spawn);
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

uintptr_t EQ_GetSpawnPreviousSpawn(uintptr_t spawn)
{
    return eq::Memory::Read<uintptr_t>(spawn + eq::Offsets::Spawn::PreviousSpawn);
}

uintptr_t EQ_GetSpawnNextSpawn(uintptr_t spawn)
{
    return eq::Memory::Read<uintptr_t>(spawn + eq::Offsets::Spawn::NextSpawn);
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

void EQ_TurnCameraTowardsLocation(const eq::Location& location)
{
    uintptr_t playerSpawn = EQ_GetPlayerSpawn();
    if (playerSpawn == NULL)
    {
        return;
    }

    eq::Location cameraLocation = EQ_GetCameraLocation();

    float heading = EQ_GetBearingByLocation(cameraLocation, location);

    EQ_SetSpawnHeading(playerSpawn, heading);
}

void EQ_TurnSpawnTowardsLocation(uintptr_t spawn, const eq::Location& location)
{
    eq::Location spawnLocation = EQ_GetSpawnLocation(spawn);

    float heading = EQ_GetBearingByLocation(spawnLocation, location);

    EQ_SetSpawnHeading(spawn, heading);
}

void EQ_TurnSpawnAwayFromLocation(uintptr_t spawn, const eq::Location& location)
{
    eq::Location spawnLocation = EQ_GetSpawnLocation(spawn);

    float heading = EQ_GetBearingByLocation(location, spawnLocation);

    EQ_SetSpawnHeading(spawn, heading);
}

void EQ_TurnSpawnTowardsSpawn(uintptr_t spawn1, uintptr_t spawn2)
{
    eq::Location spawn2Location = EQ_GetSpawnLocation(spawn2);

    EQ_TurnSpawnTowardsLocation(spawn1, spawn2Location);
}

void EQ_TurnSpawnAwayFromSpawn(uintptr_t spawn1, uintptr_t spawn2)
{
    eq::Location spawn2Location = EQ_GetSpawnLocation(spawn2);

    EQ_TurnSpawnAwayFromLocation(spawn1, spawn2Location);
}

void EQ_TurnPlayerTowardsSpawn(uintptr_t spawn)
{
    uintptr_t playerSpawn = EQ_GetPlayerSpawn();
    if (playerSpawn == NULL)
    {
        return;
    }

    eq::Location spawnLocation = EQ_GetSpawnLocation(spawn);

    EQ_TurnSpawnTowardsLocation(playerSpawn, spawnLocation);
}

void EQ_TurnPlayerAwayFromSpawn(uintptr_t spawn)
{
    uintptr_t playerSpawn = EQ_GetPlayerSpawn();
    if (playerSpawn == NULL)
    {
        return;
    }

    eq::Location spawnLocation = EQ_GetSpawnLocation(spawn);

    EQ_TurnSpawnAwayFromLocation(playerSpawn, spawnLocation);
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

void EQ_LookCameraAtLocation(const eq::Location& location)
{
    uintptr_t playerSpawn = EQ_GetPlayerSpawn();
    if (playerSpawn == NULL)
    {
        return;
    }

    EQ_SetCameraViewToFirstPerson();

    eq::Location cameraLocation = EQ_GetCameraLocation();

    float opposite = EQ_GetDistanceByLocation2D(cameraLocation, location);

    float adjacent = std::fabsf(cameraLocation.Z - location.Z);

    float hypotenuse = std::sqrtf(std::powf(opposite, 2.0f) + std::powf(adjacent, 2.0f));

    if (adjacent == 0.0f || hypotenuse == 0.0f)
    {
        return;
    }

    float angleRadians = std::acosf(adjacent / hypotenuse); // SOHCAHTOA

    float angleDegrees = EQ_GetDegrees(angleRadians);

    EQ_TurnCameraTowardsLocation(location);

    if (location.Z > cameraLocation.Z)
    {
        EQ_SetSpawnPitch(playerSpawn, eq::Constants::Spawn::Pitch::Max - angleDegrees);
    }
    else if (location.Z < cameraLocation.Z)
    {
        EQ_SetSpawnPitch(playerSpawn, eq::Constants::Spawn::Pitch::Min + angleDegrees);
    }
}

void EQ_LookCameraAtSpawn(uintptr_t spawn)
{
    eq::Location spawnLocation = EQ_GetSpawnLocation(spawn);

    EQ_LookCameraAtLocation(spawnLocation);
}


void EQ_LookCameraAtTarget()
{
    uintptr_t targetSpawn = EQ_GetTargetSpawn();
    if (targetSpawn == NULL)
    {
        return;
    }

    EQ_LookCameraAtSpawn(targetSpawn);
}

void EQ_LookPlayerAtLocation(const eq::Location& location)
{
    uintptr_t playerSpawn = EQ_GetPlayerSpawn();
    if (playerSpawn == NULL)
    {
        return;
    }

    EQ_SetCameraViewToFirstPerson();

    EQ_TurnSpawnTowardsLocation(playerSpawn, location);

    eq::Location cameraLocation = EQ_GetCameraLocation();

    float opposite = EQ_GetDistanceByLocation2D(cameraLocation, location);

    float adjacent = std::fabsf(cameraLocation.Z - location.Z);

    float hypotenuse = std::sqrtf(std::powf(opposite, 2.0f) + std::powf(adjacent, 2.0f));

    if (adjacent == 0.0f || hypotenuse == 0.0f)
    {
        return;
    }

    float angleRadians = std::acosf(adjacent / hypotenuse); // SOHCAHTOA

    float angleDegrees = EQ_GetDegrees(angleRadians);

    if (location.Z < cameraLocation.Z)
    {
        EQ_SetSpawnPitch(playerSpawn, eq::Constants::Spawn::Pitch::Min + angleDegrees);
    }
    else if (location.Z > cameraLocation.Z)
    {
        EQ_SetSpawnPitch(playerSpawn, eq::Constants::Spawn::Pitch::Max - angleDegrees);
    }

}

void EQ_LookPlayerAtSpawn(uintptr_t spawn)
{
    eq::Location spawnLocation = EQ_GetSpawnLocation(spawn);

    EQ_LookPlayerAtLocation(spawnLocation);
}

void EQ_LookPlayerAtTarget()
{
    uintptr_t targetSpawn = EQ_GetTargetSpawn();
    if (targetSpawn == NULL)
    {
        return;
    }

    EQ_LookPlayerAtSpawn(targetSpawn);
}

void EQ_InterpretCommand(const std::string& text)
{
    uintptr_t everquest = EQ_GetCEverQuest();
    if (everquest == NULL)
    {
        return;
    }

    uintptr_t playerSpawn = EQ_GetPlayerSpawn();
    if (playerSpawn == NULL)
    {
        return;
    }

    EQ_FUNCTION_CEverQuest__InterpretCommand((uintptr_t*)everquest, (uintptr_t*)playerSpawn, text.c_str());
}

void EQ_ExecuteCommand(uint32_t commandID)
{
    EQ_FUNCTION_ExecuteCommand(commandID, false, nullptr, nullptr);
}

void EQ_ExecuteCommandEx(uint32_t commandID, bool keyDown)
{
    EQ_FUNCTION_ExecuteCommand(commandID, keyDown, nullptr, nullptr);
}

void EQ_PrintTextToChat(const std::string& text)
{
    uintptr_t chatManager = EQ_GetChatManager();
    if (chatManager == NULL)
    {
        return;
    }

    EQ_FUNCTION_ChatManager__PrintText((uintptr_t*)chatManager, text.c_str(), eq::Constants::ChatTextColor::White, true, true, NULL);
}

void EQ_PrintTextToChatByColor(const std::string& text, uint32_t chatTextColor)
{
    uintptr_t chatManager = EQ_GetChatManager();
    if (chatManager == NULL)
    {
        return;
    }

    EQ_FUNCTION_ChatManager__PrintText((uintptr_t*)chatManager, text.c_str(), chatTextColor, true, true, NULL);
}

void EQ_DrawText(const std::string& text, uint32_t x, uint32_t y)
{
    EQ_FUNCTION_DrawText(text.c_str(), x, y, eq::Constants::DrawTextColor::Gray);
}

void EQ_DrawTextByColor(const std::string& text, uint32_t x, uint32_t y, uint32_t drawTextColor)
{
    EQ_FUNCTION_DrawText(text.c_str(), x, y, drawTextColor);
}

void EQ_SetCameraViewToFirstPerson()
{
    EQ_ExecuteCommandEx(eq::Constants::ExecuteCommand::FIRST_PERSON_CAMERA, true);
}

eq::Location EQ_GetCameraLocation()
{
    eq::Location location;
    location.Y = eq::Constants::FloatInfinity;
    location.X = eq::Constants::FloatInfinity;
    location.Z = eq::Constants::FloatInfinity;

    uintptr_t camera = EQ_GetCCamera();
    if (camera == NULL)
    {
        return location;
    }

    location.Y = eq::Memory::Read<float>(camera + eq::Offsets::CCamera::Y);
    location.X = eq::Memory::Read<float>(camera + eq::Offsets::CCamera::X);
    location.Z = eq::Memory::Read<float>(camera + eq::Offsets::CCamera::Z);

    return location;
}

eq::Orientation EQ_GetCameraOrientation()
{
    eq::Orientation orientation;
    orientation.Heading = eq::Constants::FloatInfinity;
    orientation.Pitch = eq::Constants::FloatInfinity;
    orientation.Rotation = eq::Constants::FloatInfinity;

    uintptr_t camera = EQ_GetCCamera();
    if (camera == NULL)
    {
        return orientation;
    }

    orientation.Heading = eq::Memory::Read<float>(camera + eq::Offsets::CCamera::Heading);
    orientation.Pitch = eq::Memory::Read<float>(camera + eq::Offsets::CCamera::Pitch);
    orientation.Rotation = eq::Memory::Read<float>(camera + eq::Offsets::CCamera::Rotation);

    return orientation;
}

float EQ_GetCameraPitch()
{
    uintptr_t camera = EQ_GetCCamera();
    if (camera == NULL)
    {
        return eq::Constants::FloatInfinity;
    }

    return eq::Memory::Read<float>(camera + eq::Offsets::CCamera::Pitch);
}

float EQ_GetCameraFieldOfView()
{
    uintptr_t camera = EQ_GetCCamera();
    if (camera == NULL)
    {
        return eq::Constants::FloatInfinity;
    }

    return eq::Memory::Read<float>(camera + eq::Offsets::CCamera::FieldOfView);
}

float EQ_GetCameraDrawDistance()
{
    uintptr_t camera = EQ_GetCCamera();
    if (camera == NULL)
    {
        return eq::Constants::FloatInfinity;
    }

    return eq::Memory::Read<float>(camera + eq::Offsets::CCamera::DrawDistance);
}

float EQ_GetCameraFarClipPlane()
{
    uintptr_t camera = EQ_GetCCamera();
    if (camera == NULL)
    {
        return eq::Constants::FloatInfinity;
    }

    return eq::Memory::Read<float>(camera + eq::Offsets::CCamera::FarClipPlane);
}

void EQ_SetCameraLocation(eq::Location& location)
{
    uintptr_t camera = EQ_GetCCamera();
    if (camera == NULL)
    {
        return;
    }

    eq::Memory::Write<float>(camera + eq::Offsets::CCamera::Y, location.Y);
    eq::Memory::Write<float>(camera + eq::Offsets::CCamera::X, location.X);
    eq::Memory::Write<float>(camera + eq::Offsets::CCamera::Z, location.Z);
}

void EQ_SetCameraPitch(float pitch)
{
    uintptr_t camera = EQ_GetCCamera();
    if (camera == NULL)
    {
        return;
    }

    eq::Memory::WriteProtected<float>(camera + eq::Offsets::CCamera::Pitch, pitch);
}

void EQ_SetCameraFieldOfView(float fieldOfView)
{
    uintptr_t camera = EQ_GetCCamera();
    if (camera == NULL)
    {
        return;
    }

    eq::Memory::Write<float>(camera + eq::Offsets::CCamera::FieldOfView, fieldOfView);
}

void EQ_SetCameraDrawDistance(float distance)
{
    uintptr_t camera = EQ_GetCCamera();
    if (camera == NULL)
    {
        return;
    }

    eq::Memory::Write<float>(camera + eq::Offsets::CCamera::DrawDistance, distance);
}

void EQ_SetCameraFarClipPlane(float distance)
{
    uintptr_t camera = EQ_GetCCamera();
    if (camera == NULL)
    {
        return;
    }

    eq::Memory::Write<float>(camera + eq::Offsets::CCamera::FarClipPlane, distance);
}

bool EQ_GetScreenCoordinatesByLocation(eq::Location& location, eq::ScreenCoordinates& screenCoordinates)
{
   uintptr_t camera = EQ_GetCCamera();
    if (camera == NULL)
    {
        return false;
    }

    float screenX = -1.0f;
    float screenY = -1.0f;
    bool isLocationOnScreen = EQ_FUNCTION_CCamera__WorldSpaceToScreenSpace((uintptr_t*)camera, location, screenX, screenY);
    if (isLocationOnScreen == true)
    {
        screenCoordinates.X = (uint32_t)screenX;
        screenCoordinates.Y = (uint32_t)screenY;
    }

    return isLocationOnScreen;
}

bool EQ_IsLocationOnScreen(eq::Location& location)
{
    eq::ScreenCoordinates screenCoordinates;
    return EQ_GetScreenCoordinatesByLocation(location, screenCoordinates);
}

bool EQ_IsSpawnOnScreen(uintptr_t spawn)
{
    eq::Location spawnLocation = EQ_GetSpawnLocation(spawn);

    return EQ_IsLocationOnScreen(spawnLocation);
}

void EQ_PlaySound(const std::string& fileName)
{
    std::string filePath = std::format("sounds/{}", fileName);

    if (filePath.ends_with(".wav") == false)
    {
        return;
    }

    if (std::filesystem::exists(filePath) == false)
    {
        return;
    }

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

}
