module;

#include "common.h"
#include "utility.h"

export module eqapp_freecamera;

export import eq;
export import eq_functions;

export import eqapp_log;
export import eqapp_timer;

export
{

namespace eqapp
{

class FreeCamera
{

public:

    FreeCamera();
    ~FreeCamera();

    static FreeCamera& GetInstance()
    {
        static FreeCamera instance;
        return instance;
    }

private:

    FreeCamera(const FreeCamera&) = delete;
    FreeCamera(FreeCamera&&) = delete;
    FreeCamera& operator=(const FreeCamera&) = delete;
    FreeCamera& operator=(FreeCamera&&) = delete;

public:

    void Load();
    void Unload();
    bool IsEnabled();
    bool IsLoaded();
    void Toggle();
    void Enable();
    void Disable();
    std::string GetDisplayText();
    void Execute();
    bool HandleExecuteCommand(uint32_t commandID, bool keyDown);
    bool HandleInterpetCommand(const std::string& commandText);

private:

    const std::string m_className = "Free Camera";

    bool m_isEnabled = false;
    bool m_isLoaded = false;

    float m_speedMultiplierDefault = 1.0f;
    float m_speedMultiplierFast = 4.0f;

    float m_speedMultiplier = m_speedMultiplierDefault;

    int m_keyForward = VK_UP;
    int m_keyBackward = VK_DOWN;
    int m_keyLeftward = VK_LEFT;
    int m_keyRightward = VK_RIGHT;
    int m_keySpeedMultiplier = VK_LCONTROL;
    int m_keySpeedMultiplier2 = VK_RCONTROL;

};

FreeCamera::FreeCamera()
{
    //
}

FreeCamera::~FreeCamera()
{
    //
}

void FreeCamera::Load()
{
    //

    m_isLoaded = true;
}

void FreeCamera::Unload()
{
    if (m_isEnabled == true)
    {
        Disable();
    }

    m_isLoaded = false;
}

bool FreeCamera::IsEnabled()
{
    return m_isEnabled;
}

bool FreeCamera::IsLoaded()
{
    return m_isLoaded;
}

void FreeCamera::Toggle()
{
    EQ_SetCameraViewToFirstPerson();
    EQ_CenterView();

    util::ToggleBool(m_isEnabled);
    std::print(std::cout, "{}: {}\n", m_className, m_isEnabled);
}

void FreeCamera::Enable()
{
    if (m_isEnabled == false)
    {
        Toggle();
    }
}

void FreeCamera::Disable()
{
    if (m_isEnabled == true)
    {
        Toggle();
    }
}

std::string FreeCamera::GetDisplayText()
{
    std::string displayText;

    auto displayTextBackInserter = std::back_inserter(displayText);

    if (IsEnabled() == true)
    {
        std::format_to(displayTextBackInserter, "{} enabled\n", m_className);
    }
    else
    {
        std::format_to(displayTextBackInserter, "{} disabled\n", m_className);
    }

    return displayText;
}

void FreeCamera::Execute()
{
    EQ_SetCameraViewToFirstPerson();

    if (util::WinAPI::IsForegroundWindowCurrentProcessID() == false)
    {
        return;
    }

    if (GetAsyncKeyState(m_keySpeedMultiplier) || GetAsyncKeyState(m_keySpeedMultiplier2))
    {
        m_speedMultiplier = m_speedMultiplierFast;
    }
    else
    {
        m_speedMultiplier = m_speedMultiplierDefault;
    }

    if (GetAsyncKeyState(m_keyForward))
    {
        eq::Location cameraLocation = EQ_GetCameraLocation();
        eq::Orientation cameraOrientation = EQ_GetCameraOrientation();

        EQ_ApplyForwardMovementToLocation(cameraLocation, cameraOrientation, m_speedMultiplier);

        EQ_SetCameraLocation(cameraLocation);
    }

    if (GetAsyncKeyState(m_keyBackward))
    {
        eq::Location cameraLocation = EQ_GetCameraLocation();
        eq::Orientation cameraOrientation = EQ_GetCameraOrientation();

        EQ_ApplyBackwardMovementToLocation(cameraLocation, cameraOrientation, m_speedMultiplier);

        EQ_SetCameraLocation(cameraLocation);
    }

    if (GetAsyncKeyState(m_keyLeftward))
    {
        eq::Location cameraLocation = EQ_GetCameraLocation();
        eq::Orientation cameraOrientation = EQ_GetCameraOrientation();

        EQ_ApplyLeftwardMovementToLocation(cameraLocation, cameraOrientation, m_speedMultiplier);

        EQ_SetCameraLocation(cameraLocation);
    }

    if (GetAsyncKeyState(m_keyRightward))
    {
        eq::Location cameraLocation = EQ_GetCameraLocation();
        eq::Orientation cameraOrientation = EQ_GetCameraOrientation();

        EQ_ApplyRightwardMovementToLocation(cameraLocation, cameraOrientation, m_speedMultiplier);

        EQ_SetCameraLocation(cameraLocation);
    }
}

bool FreeCamera::HandleExecuteCommand(uint32_t commandID, bool keyDown)
{
    //if (keyDown == false)
    //{
        //return false;
    //}

    if
    (
        commandID == eq::Constants::ExecuteCommand::SIT_STAND    ||
        commandID == eq::Constants::ExecuteCommand::DUCK         ||
        commandID == eq::Constants::ExecuteCommand::JUMP
    )
    {
        Disable();
        return true;
    }

    // block these commands while enabled
    if
    (
        commandID == eq::Constants::ExecuteCommand::TOGGLECAM    ||
        commandID == eq::Constants::ExecuteCommand::FORWARD      ||
        commandID == eq::Constants::ExecuteCommand::BACK         ||
        commandID == eq::Constants::ExecuteCommand::LEFT         ||
        commandID == eq::Constants::ExecuteCommand::RIGHT        ||
        commandID == eq::Constants::ExecuteCommand::AUTORUN
    )
    {
        return true;
    }

    return false;
}

bool FreeCamera::HandleInterpetCommand(const std::string& commandText)
{
    if (commandText == "//FreeCameraToggle")
    {
        Toggle();
        return true;
    }

    if (commandText == "//FreeCameraEnable" || commandText == "//FreeCameraOn")
    {
        Enable();
        return true;
    }

    if (commandText == "//FreeCameraDisable" || commandText == "//FreeCameraOff")
    {
        Disable();
        return true;
    }

    return false;
}

}

inline eqapp::FreeCamera& g_FreeCamera = eqapp::FreeCamera::GetInstance();

}
