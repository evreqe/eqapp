module;

#include "common.h"
#include "utility.h"

export module eqapp_windowtitle;

export import eq;
export import eq_functions;

export import eqapp_log;
export import eqapp_timer;

export
{

namespace eqapp
{

class WindowTitle
{

public:

    WindowTitle();
    ~WindowTitle();

    static WindowTitle& GetInstance()
    {
        static WindowTitle instance;
        return instance;
    }

private:

    WindowTitle(const WindowTitle&) = delete;
    WindowTitle(WindowTitle&&) = delete;
    WindowTitle& operator=(const WindowTitle&) = delete;
    WindowTitle& operator=(WindowTitle&&) = delete;

public:

    void Load();
    void Unload();
    bool IsEnabled();
    bool IsLoaded();
    void Toggle();
    void Enable();
    void Disable();
    void Execute();
    bool HandleInterpetCommand(const std::string& commandText);

private:

    const std::string m_className = "Window Title";

    const std::string m_defaultWindowTitle = "EverQuest";
    const std::string m_modifiedWindowTitle = "EverQuest*";

    bool m_isEnabled = true;
    bool m_isLoaded = false;

    eqapp::Timer m_timer;
    eqapp::Timer::TimeInterval m_timerInterval = 3;

};

WindowTitle::WindowTitle()
{
    //
}

WindowTitle::~WindowTitle()
{
    //
}

void WindowTitle::Load()
{
    EQ_SetWindowTitle(m_modifiedWindowTitle);

    m_isLoaded = true;
}

void WindowTitle::Unload()
{
    EQ_SetWindowTitle(m_defaultWindowTitle);

    m_isLoaded = false;
}

bool WindowTitle::IsEnabled()
{
    return m_isEnabled;
}

bool WindowTitle::IsLoaded()
{
    return m_isLoaded;
}

void WindowTitle::Toggle()
{
    util::ToggleBool(m_isEnabled);
    std::print(std::cout, "{}: {}\n", m_className, m_isEnabled);

    if (m_isEnabled == false)
    {
        EQ_SetWindowTitle(m_defaultWindowTitle);
    }
}

void WindowTitle::Enable()
{
    if (m_isEnabled == false)
    {
        Toggle();
    }
}

void WindowTitle::Disable()
{
    if (m_isEnabled == true)
    {
        Toggle();
    }
}

void WindowTitle::Execute()
{
    if (m_timer.HasTimeElapsedInSeconds(m_timerInterval) == false)
    {
        return;
    }
    else
    {
        m_timer.Restart();
    }

    std::string playerSpawnName = EQ_GetPlayerSpawnName();

    if (playerSpawnName.empty() == true)
    {
        return;
    }

    std::string windowTitleText = std::format("EQ: {}", playerSpawnName);

    EQ_SetWindowTitle(windowTitleText);
}

bool WindowTitle::HandleInterpetCommand(const std::string& commandText)
{
    if (commandText == "//WindowTitleToggle" || commandText == "//WindowTitle")
    {
        Toggle();
        return true;
    }

    if (commandText == "//WindowTitleEnable" || commandText == "//WindowTitleOn")
    {
        Enable();
        return true;
    }

    if (commandText == "//WindowTitleDisable" || commandText == "//WindowTitleOff")
    {
        Disable();
        return true;
    }

    return false;
}

}

inline eqapp::WindowTitle& g_WindowTitle = eqapp::WindowTitle::GetInstance();

}
