module;

#include "common.h"

export module eq_functions_h;

export import eq;
export import eq_classes;

export
{

void EQ_ToggleBool(bool& b);
void EQ_OutputFiles();
float EQ_GetDistance2D(float y1, float x1, float y2, float x2);
float EQ_GetDistance3D(float y1, float x1, float z1, float y2, float x2, float z2);
bool EQ_IsWithinDistance2D(float y1, float x1, float y2, float x2, float distance);
bool EQ_IsWithinDistance3D(float y1, float x1, float z1, float y2, float x2, float z2, float distance);
float EQ_GetBearing(float y1, float x1, float y2, float x2);
float EQ_GetDistanceByLocation2D(const eq::Location& location1, const eq::Location& location2);
float EQ_GetDistanceByLocation3D(const eq::Location& location1, const eq::Location& location2);
bool EQ_IsWithinDistanceByLocation2D(const eq::Location& location1, const eq::Location& location2, float distance);
bool EQ_IsWithinDistanceByLocation3D(const eq::Location& location1, const eq::Location& location2, float distance);
float EQ_GetBearingByLocation(const eq::Location& location1, const eq::Location& location2);
eq::Location EQ_GetMidpointByLocation(const eq::Location& location1, const eq::Location& location2);
float EQ_RoundHeading(float heading);
float EQ_FixHeading(float heading);
float EQ_FixPitch(float pitch);
float EQ_GetRadians(float degrees);
float EQ_GetDegrees(float radians);
void EQ_ApplyForwardMovementToLocation(eq::Location& location, eq::Orientation& orientation, float speed);
void EQ_ApplyBackwardMovementToLocation(eq::Location& location, eq::Orientation& orientation, float speed);
void EQ_ApplyLeftwardMovementToLocation(eq::Location& location, eq::Orientation& orientation, float speed);
void EQ_ApplyRightwardMovementToLocation(eq::Location& location, eq::Orientation& orientation, float speed);
uintptr_t EQ_GetCEverQuest();
uintptr_t EQ_GetCDisplay();
uintptr_t EQ_GetCCamera();
uintptr_t EQ_GetCCameraSetLocation();
uintptr_t EQ_GetCCameraSetOrientation();
uintptr_t EQ_GetCCameraWorldSpaceToScreenSpace();
uintptr_t EQ_GetCCameraScreenSpaceToWorldSpace();
uintptr_t EQ_GetChatManager();
uintptr_t EQ_GetSpawnManager();
uintptr_t EQ_GetSoundManager();
uintptr_t EQ_GetSGraphicsEngine();
uintptr_t EQ_GetCRender();
uintptr_t EQ_GetCRenderDrawLine2D();
uintptr_t EQ_GetCRenderDrawLine3D();
uintptr_t EQ_GetCParticleSystem();
uint32_t EQ_GetGameState();
bool EQ_IsInGame();
bool EQ_HasTimeElapsed(uint32_t& timer, uint32_t& timerInterval);
HWND EQ_GetWindow();
bool EQ_IsWindowInBackground();
bool EQ_SetWindowTitle(const std::string& windowTitle);
POINT EQ_GetMousePosition();
eq::SpawnList EQ_GetSpawnList();
bool EQ_DoesSpawnExist(uintptr_t spawn);
uint64_t EQ_GetNumPlayersInZone();
uint64_t EQ_GetNumNearbyPlayers(float distance, float distanceZ);
uintptr_t EQ_GetSpawnByID(uint32_t spawnID);
uintptr_t EQ_GetSpawnByNameNumbered(const std::string& spawnName);
uintptr_t EQ_GetSpawnByNameOrNameNumbered(const std::string& spawnName);
uintptr_t EQ_GetSpawnByName(const std::string& spawnName);
uintptr_t EQ_GetFirstSpawn();
uintptr_t EQ_GetLastSpawn();
uintptr_t EQ_GetControlledSpawn();
uintptr_t EQ_GetPlayerSpawn();
uintptr_t EQ_GetTargetSpawn();
void EQ_SetTargetSpawn(uintptr_t spawn);
void EQ_SetTargetSpawnByID(uint32_t spawnID);
void EQ_SetTargetSpawnByName(const std::string& spawnName);
uint32_t EQ_GetPlayerSpawnID();
std::string EQ_GetPlayerSpawnNameNumbered();
std::string EQ_GetPlayerSpawnName();
std::string EQ_GetPlayerSpawnLastName();
uint32_t EQ_GetTargetSpawnID();
std::string EQ_GetTargetSpawnNameNumbered();
std::string EQ_GetTargetSpawnName();
std::string EQ_GetTargetSpawnLastName();
float EQ_GetSpawnDistance2D(uintptr_t spawn);
float EQ_GetSpawnDistance3D(uintptr_t spawn);
eq::Location EQ_GetSpawnLocation(uintptr_t spawn);
bool EQ_IsSpawnWithinDistance2D(uintptr_t spawn, float distance);
bool EQ_IsSpawnWithinDistance3D(uintptr_t spawn, float distance);
bool EQ_IsSpawnWithinDistanceOfLocation2D(uintptr_t spawn, const eq::Location& location, float distance);
bool EQ_IsSpawnWithinDistanceOfLocation3D(uintptr_t spawn, const eq::Location& location, float distance);
bool EQ_IsSpawnWithinDistanceOfSpawn2D(uintptr_t spawn1, uintptr_t spawn2, float distance);
bool EQ_IsSpawnWithinDistanceOfSpawn3D(uintptr_t spawn1, uintptr_t spawn2, float distance);
bool EQ_IsSpawnTargetable(uintptr_t spawn);
bool EQ_IsSpawnClickThrough(uintptr_t spawn);
bool EQ_IsSpawnSwimming(uintptr_t spawn);
bool EQ_IsSpawnSwimmingUnderwater(uintptr_t spawn);
bool EQ_IsSpawnMoving(uintptr_t spawn);
bool EQ_IsPlayerMoving();
bool EQ_IsTargetMoving();
bool EQ_IsSpawnBehindSpawn(uintptr_t spawn1, uintptr_t spawn2);
bool EQ_IsSpawnBehindSpawnEx(uintptr_t spawn1, uintptr_t spawn2, float angle);
bool EQ_IsPlayerBehindSpawn(uintptr_t spawn);
bool EQ_IsPlayerBehindTarget();
bool EQ_IsSpawnBehindPlayer(uintptr_t spawn);
bool EQ_IsTargetBehindPlayer();
int EQ_GetSpawnLevel(uintptr_t spawn);
std::string EQ_GetSpawnNameNumbered(uintptr_t spawn);
std::string EQ_GetSpawnName(uintptr_t spawn);
std::string EQ_GetSpawnLastName(uintptr_t spawn);
uintptr_t EQ_GetSpawnPreviousSpawn(uintptr_t spawn);
uintptr_t EQ_GetSpawnNextSpawn(uintptr_t spawn);
float EQ_GetSpawnJumpStrength(uintptr_t spawn);
float EQ_GetSpawnSwimStrength(uintptr_t spawn);
float EQ_GetSpawnMovementSpeedBonus(uintptr_t spawn);
float EQ_GetSpawnAreaFriction(uintptr_t spawn);
float EQ_GetSpawnAccelerationFriction(uintptr_t spawn);
uint32_t EQ_GetSpawnCollideWithCActorInterfaceType(uintptr_t spawn);
float EQ_GetSpawnFloorZ(uintptr_t spawn);
float EQ_GetSpawnY(uintptr_t spawn);
float EQ_GetSpawnX(uintptr_t spawn);
float EQ_GetSpawnZ(uintptr_t spawn);
float EQ_GetSpawnSpeedY(uintptr_t spawn);
float EQ_GetSpawnSpeedX(uintptr_t spawn);
float EQ_GetSpawnSpeedZ(uintptr_t spawn);
float EQ_GetSpawnMovementSpeed(uintptr_t spawn);
float EQ_GetSpawnHeading(uintptr_t spawn);
float EQ_GetSpawnHeadingSpeed(uintptr_t spawn);
float EQ_GetSpawnPitch(uintptr_t spawn);
uint32_t EQ_GetSpawnLastHeadEnvironmentType(uintptr_t spawn);
uint32_t EQ_GetSpawnLastBodyEnvironmentType(uintptr_t spawn);
uint32_t EQ_GetSpawnLastFeetEnvironmentType(uintptr_t spawn);
uint8_t EQ_GetSpawnHeadEnvironmentType(uintptr_t spawn);
uint8_t EQ_GetSpawnFeetEnvironmentType(uintptr_t spawn);
uint8_t EQ_GetSpawnBodyEnvironmentType(uintptr_t spawn);
uint8_t EQ_GetSpawnBodyEnvironmentType2(uintptr_t spawn);
uint8_t EQ_GetSpawnType(uintptr_t spawn);
float EQ_GetSpawnHeightZ(uintptr_t spawn);
float EQ_GetSpawnHeight(uintptr_t spawn);
float EQ_GetSpawnWidth(uintptr_t spawn);
float EQ_GetSpawnLength(uintptr_t spawn);
uint32_t EQ_GetSpawnID(uintptr_t spawn);
uint32_t EQ_GetSpawnState(uintptr_t spawn);
uintptr_t EQ_GetSpawnVehicleSpawn(uintptr_t spawn);
uintptr_t EQ_GetSpawnMountSpawn(uintptr_t spawn);
uintptr_t EQ_GetSpawnMountRiderSpawn(uintptr_t spawn);
uint32_t EQ_GetSpawnDirection(uintptr_t spawn);
void EQ_SetSpawnSpeedY(uintptr_t spawn, float speed);
void EQ_SetSpawnSpeedX(uintptr_t spawn, float speed);
void EQ_SetSpawnSpeedZ(uintptr_t spawn, float speed);
void EQ_SetSpawnAreaFriction(uintptr_t spawn, float friction);
void EQ_SetSpawnAccelerationFriction(uintptr_t spawn, float friction);
void EQ_SetSpawnHeading(uintptr_t spawn, float heading);
void EQ_SetSpawnPitch(uintptr_t spawn, float pitch);
void EQ_SetPlayerSpawnHeadingNorth();
void EQ_SetPlayerSpawnHeadingNorthWest();
void EQ_SetPlayerSpawnHeadingWest();
void EQ_SetPlayerSpawnHeadingSouthWest();
void EQ_SetPlayerSpawnHeadingSouth();
void EQ_SetPlayerSpawnHeadingSouthEast();
void EQ_SetPlayerSpawnHeadingEast();
void EQ_SetPlayerSpawnHeadingNorthEast();
void EQ_SetPlayerSpawnFrictionToDefault();
void EQ_TurnLeft();
void EQ_TurnRight();
void EQ_TurnAround();
void EQ_TurnCameraTowardsLocation(const eq::Location& location);
void EQ_TurnSpawnTowardsLocation(uintptr_t spawn, const eq::Location& location);
void EQ_TurnSpawnAwayFromLocation(uintptr_t spawn, const eq::Location& location);
void EQ_TurnSpawnTowardsSpawn(uintptr_t spawn1, uintptr_t spawn2);
void EQ_TurnSpawnAwayFromSpawn(uintptr_t spawn1, uintptr_t spawn2);
void EQ_TurnPlayerTowardsSpawn(uintptr_t spawn);
void EQ_TurnPlayerAwayFromSpawn(uintptr_t spawn);
void EQ_TurnPlayerTowardsTarget();
void EQ_TurnPlayerAwayFromTarget();
void EQ_LookCameraAtLocation(const eq::Location& location);
void EQ_LookCameraAtSpawn(uintptr_t spawn);
void EQ_LookCameraAtTarget();
void EQ_LookPlayerAtLocation(const eq::Location& location);
void EQ_LookPlayerAtSpawn(uintptr_t spawn);
void EQ_LookPlayerAtTarget();
void EQ_InterpretCommand(const std::string& text);
void EQ_ExecuteCommand(uint32_t commandID);
void EQ_ExecuteCommandEx(uint32_t commandID, bool keyDown);
void EQ_PrintTextToChat(const std::string& text);
void EQ_PrintTextToChatByColor(const std::string& text, uint32_t chatTextColor);
void EQ_DrawText(const std::string& text, uint32_t x, uint32_t y);
void EQ_DrawTextByColor(const std::string& text, uint32_t x, uint32_t y, uint32_t drawTextColor);
bool EQ_DrawLine2D(const eq::Vector3f& vector1, const eq::Vector3f& vector2, uint32_t colorARGB);
bool EQ_DrawLine3D(const eq::Location& location1, const eq::Location& location2, uint32_t colorARGB);
void EQ_SetCameraViewToFirstPerson();
eq::Location EQ_GetCameraLocation();
eq::Orientation EQ_GetCameraOrientation();
float EQ_GetCameraFieldOfView();
float EQ_GetCameraAspectRatio();
float EQ_GetCameraDrawDistance();
float EQ_GetCameraActorClipPlane();
float EQ_GetCameraShadowClipPlane();
float EQ_GetCameraFarClipPlane();
float EQ_GetCameraScreenWidthHalf();
float EQ_GetCameraScreenHeightHalf();
float EQ_GetCameraHeading();
float EQ_GetCameraPitch();
float EQ_GetCameraRotation();
void EQ_SetCameraLocation(const eq::Location& location);
void EQ_SetCameraOrientation(const eq::Orientation& orientation);
void EQ_SetCameraFieldOfView(float fieldOfView);
void EQ_SetCameraDrawDistance(float distance);
void EQ_SetCameraActorClipPlane(float distance);
void EQ_SetCameraShadowClipPlane(float distance);
void EQ_SetCameraFarClipPlane(float distance);
void EQ_SetCameraHeading(float heading);
void EQ_SetCameraPitch(float pitch);
void EQ_SetCameraRotation(float rotation);
bool EQ_GetScreenCoordinatesByLocation(eq::Location& location, eq::ScreenCoordinates& screenCoordinates);
bool EQ_IsLocationOnScreen(eq::Location& location);
bool EQ_IsSpawnOnScreen(uintptr_t spawn);
void EQ_PlaySound(const std::string& fileName);
void EQ_StopSound();
void EQ_CenterView();
void EQ_LookUp();
void EQ_LookDown();
void EQ_ClearTarget();
bool EQ_CastRay(const eq::Location& location1, const eq::Location& location2);
bool EQ_CastRayToTarget();
bool EQ_CanSpawnCastRayToLocation(uintptr_t spawn, const eq::Location& location);
bool EQ_CanSpawnCastRayToSpawn(uintptr_t spawn1, uintptr_t spawn2);
std::string EQ_StringMap_GetValueByKey(const std::unordered_map<uint32_t, std::string>& stringMap, uint32_t key);
uint32_t EQ_StringMap_GetKeyByValue(const std::unordered_map<uint32_t, std::string>& stringMap, const std::string& value);

}
